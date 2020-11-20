#pragma once

#include <QObject>

#include <memory>

class QNetworkAccessManager;
class QSslError;

struct ReplyErrorWrapper;


class HttpAuthManager : public QObject {
	Q_OBJECT

public:
	HttpAuthManager(QObject* parent);

	void authorizeUser(const QString& login, const QString password);

public slots:
	void handleAuthorizeUserReply();
		
	void handleReplyErrors(const ReplyErrorWrapper& replyErrorWrapper);

	void handleReplySslErrors(const QList<QSslError>& sslErrors);

private:
	void initialize();
	void setupConnections();

private:
	enum class AUTH_REQUESTS {
		  AUTHORIZE_USER = 0
		,
	};

	const QString apiUrlAddress{ QStringLiteral("https://api.quwi.com/v2") };

	std::unique_ptr<QNetworkAccessManager> networkManager;
	//std::map<>
};
