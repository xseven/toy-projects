#include "stubmodel.h"
#include "eviltypeeditor.h"

#include <QApplication>
#include <QTableView>
#include <QItemEditorFactory>

namespace
{
    
    void overrideEditorFactory(void)
    {
        auto dFactory = new QItemEditorFactory;

        const auto typeID = QVariant::fromValue(evilType()).userType();

        auto evilCreator = new QStandardItemEditorCreator<EvilTypeEditor>();

        dFactory->registerEditor(typeID, evilCreator);

        QItemEditorFactory::setDefaultFactory(dFactory);
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
