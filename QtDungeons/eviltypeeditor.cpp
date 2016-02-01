#include "eviltypeeditor.h"

#include <QLayout>
#include <QComboBox>

EvilTypeEditor::EvilTypeEditor(QWidget *parent) 
: QWidget(parent)
    
{
    setLayout(new QHBoxLayout);

    _cb = new QComboBox;

    layout()->addWidget(_cb);

    layout()->setMargin(0);
    layout()->setSpacing(0);
}

void EvilTypeEditor::setData(evilType data)
{
    _cb->addItem(data.first);
    _cb->addItem(data.second);

    _data = data;
}

evilType EvilTypeEditor::data(void) const
{
    if(_cb->currentText() != _data.first)
        return evilType(_data.second, _data.first);

    return _data;
}
