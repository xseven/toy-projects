#include "mainchatwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);

    MainChatWidget window;

    window.show();

    return a.exec();
}
