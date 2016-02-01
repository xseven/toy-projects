#ifndef EVILTYPEEDITOR_H
#define EVILTYPEEDITOR_H

#include "stubmodel.h"

#include <QWidget>

class QComboBox;

class EvilTypeEditor : public QWidget
{   
    Q_OBJECT

    Q_PROPERTY(evilType data READ data WRITE setData USER true)

public:
    explicit EvilTypeEditor(QWidget *parent = 0);

    evilType data(void) const;
    void setData(evilType data);


signals:

public slots:

private:
    QComboBox*  _cb;
    evilType    _data;
};

#endif // EVILTYPEEDITOR_H
