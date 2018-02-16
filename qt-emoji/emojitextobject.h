#ifndef EMOJITEXTOBJECT_H
#define EMOJITEXTOBJECT_H

#include <QObject>
#include <QTextObjectInterface>

class EmojiTextObject
        : public QObject, public QTextObjectInterface
{
    Q_OBJECT

	Q_INTERFACES(QTextObjectInterface)

public:
    explicit EmojiTextObject(QObject *parent = nullptr);

    QSizeF intrinsicSize(QTextDocument *doc, int posInDocument,
                         const QTextFormat &format) override;

    void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc,
                    int posInDocument, const QTextFormat &format) override;

    enum {
        EmojiTextFormat = QTextFormat::UserObject + 1,
    };

    enum EmojiProperties {
        EmojiData = 1,
        EmojiCode,
    };

signals:

public slots:
};

#endif // EMOJITEXTOBJECT_H

