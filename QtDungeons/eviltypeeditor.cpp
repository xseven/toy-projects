#include "eviltypeeditor.h"

#include <QLayout>
#include <QComboBox>

EvilTypeEditor::EvilTypeEditor(QWidget *parent) 
: QWidget(parent)
    
{
    setLayout(new QHBoxLayout);

    auto cb = new QComboBox;

    cb->addItem("a");
    cb->addItem("b");

    layout()->addWidget(cb);

    layout()->setMargin(0);
    layout()->setSpacing(0);
}
