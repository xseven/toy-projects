#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QImageReader>

#include <utility>
#include <cassert>

#include "emojiprovider.h"

#include "emojitablemodel.h"

namespace
{
    const std::pair<qint64, qint64> emojiRange = {0x1F600, 0x1F61C};

    EmojiProvider* provider = EmojiProvider::instance();
}

EmojiTableModel::EmojiTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    rows = (emojiRange.second - emojiRange.first) / cols;    
}

int EmojiTableModel::rowCount(const QModelIndex &parent) const
{
    return rows;
}

int EmojiTableModel::columnCount(const QModelIndex &parent) const
{
    return cols;
}

QVariant EmojiTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }

    qint64 id = emojiRange.first + (index.row() * cols) + index.column();

    if(role == Qt::DecorationRole) {        
        return provider->getEmoji(id, {32, 32});			
    } else if (role == EmojiIdRole) {
        return id;
    }

    return QVariant();
}
