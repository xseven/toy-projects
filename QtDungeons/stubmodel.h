#ifndef STUBMODEL_H
#define STUBMODEL_H

#include <QAbstractTableModel>

class StubModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StubModel(QObject *parent = 0);

signals:

public slots:

};

#endif // STUBMODEL_H
