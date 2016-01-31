#include "stubmodel.h"

StubModel::StubModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    _data << "hello model world";
    _data << 0xABC;
    _data << true;
}


int StubModel::rowCount(const QModelIndex &parent) const
{
    return _data.size();
}

int StubModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant StubModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(Qt::DisplayRole == role || Qt::EditRole == role)
    {
        return _data.at(index.row());
    }

    return QVariant();
}

bool StubModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(Qt::EditRole == role)
    {
        _data[index.row()] = value;

        return true;
    }

    return false;
}

Qt::ItemFlags StubModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
