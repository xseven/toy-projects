#include "stubmodel.h"

StubModel::StubModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    _data << "hello model world";
    _data << 0xABC;
    _data << true;    
    _data << QVariant::fromValue(evilType("once", "twice"));
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

    if(Qt::DisplayRole == role)
    {
        auto item = _data.at(index.row());

        if(QVariant::fromValue(evilType()).userType() == item.userType())
        {
            auto parsedItem = item.value<evilType>();

            return QVariant(parsedItem.first);
        }

        return item;
    }
    else if(Qt::EditRole == role)
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
