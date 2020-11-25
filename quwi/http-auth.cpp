#include "http-auth.h"
#include "projectsmodel.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

struct ReplyErrorWrapper
{
	QNetworkReply::NetworkError networkError;
};

HttpAuthManager::HttpAuthManager()
	: HttpAuthManager(nullptr)
{

}

HttpAuthManager::HttpAuthManager(QObject* parent = nullptr)
	: QObject(parent)
{
	initialize();
	setupConnections();	
}

HttpAuthManager::~HttpAuthManager()
{

}

void HttpAuthManager::authorizeUser(const QString& login, const QString& password)
{
#if 0
	QVariantList projects;

	for (int i = 0; i < 10; ++i) {
		Project project("https://s3-us-west-2.amazonaws.com/token-daily-bucket/user-images/4a2359b48887048317100f5e5d28d0d6.jpeg", QString("project %1").arg(i), QString::number(i % 2), 0, 0, 0);

		projects << QVariant::fromValue(project);
	}

	loginOccured();
	projectsReceivedV(projects);

	return;
#endif

	QNetworkRequest request;

	QString endUrlAddress = QStringLiteral("%1/auth/login")
		.arg(apiUrlAddress);
		
	request.setUrl(QUrl(endUrlAddress));
	request.setRawHeader("Client-Device", "DESKTOP");
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QJsonDocument jsonDocument;

	QJsonObject authObject {
		  {QStringLiteral("email"), login}
		, {QStringLiteral("password"), password}
	};

	jsonDocument.setObject(authObject);
	
	auto reply = networkManager.get()->post(request, jsonDocument.toJson());

	QObject::connect(reply, &QIODevice::readyRead, this, &HttpAuthManager::handleAuthorizeUserReply);
	QObject::connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError replyNetworkError) { handleReplyErrors(ReplyErrorWrapper{ replyNetworkError }); });
	QObject::connect(reply, &QNetworkReply::sslErrors, this, &HttpAuthManager::handleReplySslErrors);
}

void HttpAuthManager::updateProjectName(const QString& projectId, const QString& name)
{
	QNetworkRequest request;
		
	QString endUrlAddress = QStringLiteral("%1/projects-manage/update?id=%2")
		.arg(apiUrlAddress)
		.arg(projectId);

	auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart namePart;
	namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QStringLiteral("form-data; name=\"name\""));
	namePart.setBody(name.toUtf8());

	QHttpPart activePart;
	activePart.setHeader(QNetworkRequest::ContentDispositionHeader, QStringLiteral("form-data; name=\"is_active\""));
	activePart.setBody(QByteArray::number(1));

	QHttpPart ownerPart;
	ownerPart.setHeader(QNetworkRequest::ContentDispositionHeader, QStringLiteral("form-data; name=\"is_owner_watcher\""));
	ownerPart.setBody(QByteArray::number(1));

	multiPart->append(namePart);
	multiPart->append(activePart);
	multiPart->append(ownerPart);

	request.setUrl(QUrl(endUrlAddress));
	request.setRawHeader("Client-Device", "DESKTOP");
	request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(data.token).toUtf8());	
	
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");

	auto reply = networkManager.get()->post(request, multiPart);

	QObject::connect(reply, &QIODevice::readyRead, this, &HttpAuthManager::handleUpdateProjectNameReply);
	QObject::connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError replyNetworkError) { handleReplyErrors(ReplyErrorWrapper{ replyNetworkError }); });
	QObject::connect(reply, &QNetworkReply::sslErrors, this, &HttpAuthManager::handleReplySslErrors);
}

void HttpAuthManager::enlistUserProjects()
{
#if 0
	return;
#endif

	QNetworkRequest request;

	QString endUrlAddress = QStringLiteral("%1/projects-manage/index")
		.arg(apiUrlAddress);

	request.setUrl(QUrl(endUrlAddress));
	request.setRawHeader("Client-Device", "DESKTOP");
	request.setRawHeader("Authorization", QStringLiteral("Bearer %1").arg(data.token).toUtf8());
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");	

	auto reply = networkManager.get()->get(request);

	QObject::connect(reply, &QIODevice::readyRead, this, &HttpAuthManager::handleEnlistUserProjectsReply);
	QObject::connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError replyNetworkError) { handleReplyErrors(ReplyErrorWrapper{ replyNetworkError }); });
	QObject::connect(reply, &QNetworkReply::sslErrors, this, &HttpAuthManager::handleReplySslErrors);
}

void HttpAuthManager::initialize()
{
	networkManager = std::make_unique<QNetworkAccessManager>(this);	
}

void HttpAuthManager::setupConnections()
{
	
}

void HttpAuthManager::handleAuthorizeUserReply()
{
	auto authorizeUserReply = qobject_cast<QNetworkReply*>(sender());
	
	parseAuthorizationReply(authorizeUserReply->readAll());

	authorizeUserReply->deleteLater();
}

void HttpAuthManager::handleEnlistUserProjectsReply()
{
	auto enlistUserProjectsReply = qobject_cast<QNetworkReply*>(sender());

	parseEnlistUserProjectsReply(enlistUserProjectsReply->readAll());

	enlistUserProjectsReply->deleteLater();
}

void HttpAuthManager::handleUpdateProjectNameReply()
{
	auto updateProjectNameReply = qobject_cast<QNetworkReply*>(sender());

	qDebug() << updateProjectNameReply->readAll();	

	updateProjectNameReply->deleteLater();

	projectsUpdated();
}

void HttpAuthManager::handleReplyErrors(const ReplyErrorWrapper& replyErrorWrapper)
{
	qDebug() << replyErrorWrapper.networkError;
	
	QString contentError = QStringLiteral("Unknown content error");

	if (!unknownContentErrors.empty()) {
		contentError = unknownContentErrors.back();
		unknownContentErrors.pop();
	}

	errorOccured(contentError);	
}

void HttpAuthManager::handleReplySslErrors(const QList<QSslError>& sslErrors)
{
	for (const auto& sslError : sslErrors) {
		qDebug() << sslError;
	}
}

void HttpAuthManager::parseAuthorizationReply(const QByteArray& replyData)
{
	qDebug() << replyData;

	auto jsonReplyDocument = QJsonDocument::fromJson(replyData);

	auto jsonReplyObject = jsonReplyDocument.object();

	if (jsonReplyObject.contains(QStringLiteral("token")) && jsonReplyObject.contains(QStringLiteral("app_init"))) {

		data.token = jsonReplyObject.value(QStringLiteral("token")).toString();

		auto appInitObject = jsonReplyObject.value(QStringLiteral("app_init")).toObject();
		auto userObject = appInitObject.value(QStringLiteral("user")).toObject();

		data.companyId = userObject.value(QStringLiteral("id_company")).toInt();

		loginOccured();
	} else if (jsonReplyObject.contains(QStringLiteral("first_errors"))) {
		auto firstErrorsObject = jsonReplyObject.value(QStringLiteral("first_errors")).toObject();

		for (const auto& firstErrorElement : firstErrorsObject) {			
			unknownContentErrors.push(firstErrorElement.toString());
			qDebug() << firstErrorElement.toString();
		}
	}
}

void HttpAuthManager::parseEnlistUserProjectsReply(const QByteArray& replyData)
{
	qDebug() << replyData;

	QVariantList projects;

	auto jsonReplyDocument = QJsonDocument::fromJson(replyData);

	auto jsonReplyObject = jsonReplyDocument.object();

	if (jsonReplyObject.contains(QStringLiteral("projects"))) {
		auto jsonProjectsArray = jsonReplyObject.value(QStringLiteral("projects")).toArray();

		for (const auto& jsonProjectItem : jsonProjectsArray) {
			const auto& jsonProject = jsonProjectItem.toObject();
					
			auto id = jsonProject.value(QStringLiteral("id")).toInt();
			auto projectName = jsonProject.value(QStringLiteral("name")).toString();
			auto logoUrl = jsonProject.value(QStringLiteral("logo_url")).toString();
			auto isActive = jsonProject.value(QStringLiteral("is_active")).toInt();
			auto timeSpentWeek = jsonProject.value(QStringLiteral("spent_time_week")).toInt();
			auto timeSpentMonth = jsonProject.value(QStringLiteral("spent_time_month")).toInt();
			auto timeSpentTotal = jsonProject.value(QStringLiteral("spent_time_all")).toInt();

			Project project(id, logoUrl, projectName, QString::number(isActive), timeSpentWeek, timeSpentMonth, timeSpentTotal);

			projects.push_back(QVariant::fromValue(project));
		}
	}
	else if (jsonReplyObject.contains(QStringLiteral("first_errors"))) {
		auto firstErrorsObject = jsonReplyObject.value(QStringLiteral("first_errors")).toObject();

		for (const auto& firstErrorElement : firstErrorsObject) {
			unknownContentErrors.push(firstErrorElement.toString());
			qDebug() << firstErrorElement.toString();
		}
	}

	if (!projects.empty()) {
		projectsReceivedV(projects);
	}
}
