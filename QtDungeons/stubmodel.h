#ifndef STUBMODEL_H
#define STUBMODEL_H

#include <QAbstractTableModel>

typedef QPair<QString, QString> evilType;
Q_DECLARE_METATYPE(evilType);

class StubModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StubModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

signals:

public slots:

private:
    QVariantList    _data;

};

#endif // STUBMODEL_H
