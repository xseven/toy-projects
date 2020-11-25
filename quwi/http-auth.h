#pragma once

#include <QObject>

#include <memory>
#include <queue>

class QNetworkAccessManager;
class QSslError;

class Project;

struct ReplyErrorWrapper;


class HttpAuthManager : public QObject {
	Q_OBJECT

public:
	HttpAuthManager();
	HttpAuthManager(QObject* parent);
	~HttpAuthManager();
	
signals:
	void errorOccured(const QString& errorString);
	void loginOccured();
	void projectsUpdated();

	void projectsReceived(const QList<Project>& projects);
	void projectsReceivedV(const QVariantList& projects);

public slots:
	void authorizeUser(const QString& login, const QString& password);
	void enlistUserProjects();
	void updateProjectName(const QString& projectId, const QString& name);

	void handleAuthorizeUserReply();
	void handleEnlistUserProjectsReply();
	void handleUpdateProjectNameReply();
		
	void handleReplyErrors(const ReplyErrorWrapper& replyErrorWrapper);

	void handleReplySslErrors(const QList<QSslError>& sslErrors);

private:
	void initialize();
	void setupConnections();

	void parseAuthorizationReply(const QByteArray& replyData);
	void parseEnlistUserProjectsReply(const QByteArray& replyData);

private:
	enum class AUTH_REQUESTS {
		  AUTHORIZE_USER = 0
		,
	};

	const QString apiUrlAddress{ QStringLiteral("https://api.quwi.com/v2") };

	std::unique_ptr<QNetworkAccessManager> networkManager;

	std::queue<QString> unknownContentErrors;
	
	struct {
		QString token;
		int companyId;
	} data;
};