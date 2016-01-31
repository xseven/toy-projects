#include "stubmodel.h"

#include <QApplication>
#include <QTableView>
#include <QItemEditorFactory>

namespace
{
    void overrideEditorFactory(void)
    {
        auto factory = new QItemEditorFactory;

        QItemEditorFactory::setDefaultFactory(factory);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    overrideEditorFactory();

    auto view = new QTableView();

    view->setModel(new StubModel);

    view->show();


    return a.exec();
}
