#include "http-auth.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

struct ReplyErrorWrapper
{
	QNetworkReply::NetworkError networkError;
};

HttpAuthManager::HttpAuthManager(QObject* parent = nullptr)
	: QObject(parent)
{
	initialize();
	setupConnections();	
}

void HttpAuthManager::authorizeUser(const QString& login, const QString password)
{
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
	
	qDebug() << authorizeUserReply->readAll();

	auto jsonReplyDocument = QJsonDocument::fromBinaryData("");

	qDebug() << jsonReplyDocument;
}

void HttpAuthManager::handleReplyErrors(const ReplyErrorWrapper& replyErrorWrapper)
{
	qDebug() << replyErrorWrapper.networkError;
}

void HttpAuthManager::handleReplySslErrors(const QList<QSslError>& sslErrors)
{
	for (const auto& sslError : sslErrors) {
		qDebug() << sslError;
	}
}