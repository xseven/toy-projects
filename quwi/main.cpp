#include <QGuiApplication>

#include "http-auth.h"

int main(int argc, char** argv)
{
	QGuiApplication app(argc, argv);

	auto httpAuth = new HttpAuthManager(&app);

	httpAuth->authorizeUser(QStringLiteral("vitaliibondtest@gmail.com"), QStringLiteral("vitaliibondtest"));

	app.exec();
}
