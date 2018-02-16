#include <cassert>

#include "emojiprovider.h"

EmojiProvider::EmojiProvider()
{

}

//Meyer's singleton. Valid since c++11
EmojiProvider* EmojiProvider::instance()
{
    static EmojiProvider provider;
    return &provider;
}

QPixmap EmojiProvider::getEmoji(qint64 id, const QSize& size)
{
    QPixmap emojiPixmap;

    if(emojiCache.contains(id)) {
		emojiPixmap = *emojiCache.take(id);
    } else {
        static const QString emojiFormat("PNG");
        static QString emojiPattern("://EmojiOne_3.1.1_32x32_png/%1.png");

        auto emojiId = QString::number(id, 16);        

		emojiPixmap = QPixmap(emojiPattern.arg(emojiId)
                            , emojiFormat.toStdString().c_str());
    }

    assert(!emojiPixmap.isNull());

	if (!emojiPixmap.isNull()) {
		if (size != emojiPixmap.size()) {
			emojiPixmap = emojiPixmap.scaled(size, Qt::KeepAspectRatio);
		}
	}

    return emojiPixmap;
}
