#include "stubmodel.h"
#include "eviltypeeditor.h"

#include <QApplication>
#include <QTableView>
#include <QItemEditorFactory>

namespace
{    
    void overrideEditorFactory(void)
    {
        class OverridenEditorFactory : public QItemEditorFactory
        {
            public:
                explicit OverridenEditorFactory(const QItemEditorFactory* dFactory)
                    : _dFactory(dFactory)
                {
                    auto creator = new QStandardItemEditorCreator<EvilTypeEditor>();

                    const auto evilTypeID = QVariant::fromValue(evilType()).userType();

                    registerEditor(evilTypeID, creator);
                }

                QWidget* createEditor(int userType, QWidget *parent) const
                {
                    const auto evilTypeID = QVariant::fromValue(evilType()).userType();

                    if(evilTypeID == userType)
                    {
                        return QItemEditorFactory::createEditor(userType, parent);
                    }

                    return _dFactory->createEditor(userType, parent);
                }

            private:
                const QItemEditorFactory*   _dFactory;
        };

        QItemEditorFactory::setDefaultFactory(new OverridenEditorFactory(QItemEditorFactory::defaultFactory()));
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
