#ifndef TEXTEDITEMOJI_H
#define TEXTEDITEMOJI_H

#include <QTextEdit>

class TextEditEmoji : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditEmoji(QWidget *parent = nullptr);

protected:
    void insertFromMimeData(const QMimeData* source) override;

signals:

public slots:
};

#endif // TEXTEDITEMOJI_H
