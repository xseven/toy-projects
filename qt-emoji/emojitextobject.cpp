#include <QPainter>

#include "emojipopupwindow.h"

#include "emojitextobject.h"

EmojiTextObject::EmojiTextObject(QObject *parent) : QObject(parent)
{

}

QSizeF EmojiTextObject::intrinsicSize(QTextDocument *doc, int posInDocument,
                     const QTextFormat &format)
{
    return QSize(16, 16);
}

void EmojiTextObject::drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc,
                int posInDocument, const QTextFormat &format)
{
    QImage emojiImage = qvariant_cast<QImage>(format.property(EmojiTextObject::EmojiData));

    painter->drawImage(rect, emojiImage);
}
