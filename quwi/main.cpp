#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "http-auth.h"
#include "projectsmodel.h"

int main(int argc, char** argv)
{
	qRegisterMetaType<Project>("Project");	

	qmlRegisterType<HttpAuthManager>("com.quwi.api", 1, 0, "APIManager");
	qmlRegisterType<ProjectModel>("com.quwi.components", 1, 0, "ProjectModel");


	QGuiApplication app(argc, argv);
	
#if 0
	auto httpAuth = new HttpAuthManager(&app);
	httpAuth->authorizeUser(QStringLiteral("vitaliibondtest@gmail.com"), QStringLiteral("vitaliibondtest"));
#endif

	QQmlApplicationEngine qmlEngine;	
	
	qmlEngine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));

	app.exec();
}
