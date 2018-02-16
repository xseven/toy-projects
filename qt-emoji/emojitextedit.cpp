#include <QMimeData>

#include "emojitextedit.h"

TextEditEmoji::TextEditEmoji(QWidget *parent)
    : QTextEdit(parent)
{

}

void TextEditEmoji::insertFromMimeData(const QMimeData* source)
{

    //[TBI] Here should be placed test for emojis in pasted string
#if 0
    QChar h = source->text().at(0);
    QChar l = source->text().at(1);

    h.isHighSurrogate();
    h.isLowSurrogate();

    l.isHighSurrogate();
    l.isLowSurrogate();

    QChar::surrogateToUcs4(h, l);
#endif

    QTextEdit::insertFromMimeData(source);
}
