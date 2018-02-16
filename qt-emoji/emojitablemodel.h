#ifndef EMOJITABLEMODEL_H
#define EMOJITABLEMODEL_H

#include <QAbstractTableModel>

class EmojiTableModel : public QAbstractTableModel
{
public:
    EmojiTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    enum Roles {
        EmojiIdRole = Qt::UserRole + 1,
    };

private:
    const int cols {4};
    int rows;
};

#endif // EMOJITABLEMODEL_H
