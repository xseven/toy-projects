#include <QTableView>
#include <QBoxLayout>
#include <QHeaderView>

#include "emojitextobject.h"
#include "emojitablemodel.h"

#include "emojipopupwindow.h"

EmojiPopupWindow::EmojiPopupWindow(QWidget *parent) : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void EmojiPopupWindow::setupUi()
{
    setLayout(new QVBoxLayout);

    ui.emojiView = new QTableView(this);
    ui.emojiView->setModel(new EmojiTableModel(this));
    ui.emojiView->verticalHeader()->setVisible(false);
    ui.emojiView->horizontalHeader()->setVisible(false);

    {
        auto verticalHeader = ui.emojiView->verticalHeader();
        verticalHeader->setDefaultSectionSize(38);
    }

    {
        auto horizontalHeader = ui.emojiView->horizontalHeader();
        horizontalHeader->setDefaultSectionSize(38);
    }

    layout()->addWidget(ui.emojiView);

    layout()->setMargin(0);    
}


void EmojiPopupWindow::setupConnections()
{
    connect(ui.emojiView, &QTableView::clicked, this, [=](const QModelIndex& index) noexcept {
        setVisible(false);
        emit emojiInserted(index.data(EmojiTableModel::EmojiIdRole).value<qint64>());
    });
}
