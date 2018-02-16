#ifndef EMOJIPOPUPWINDOW_H
#define EMOJIPOPUPWINDOW_H

#include <QWidget>

class QTableView;

class EmojiPopupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EmojiPopupWindow(QWidget *parent = nullptr);    

signals:
    void emojiInserted(qint64 id);

public slots:

private:
    void setupUi();
    void setupConnections();

private:
    struct {
        QTableView* emojiView;
    } ui = {};
};

#endif // EMOJIPOPUPWINDOW_H
