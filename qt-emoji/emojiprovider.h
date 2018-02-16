#ifndef EMOJIPROVIDER_H
#define EMOJIPROVIDER_H

#include <QCache>
#include <QPixmap>

class EmojiProvider
{
public:
    EmojiProvider(const EmojiProvider&) = delete;
    EmojiProvider(const EmojiProvider&&) = delete;

    static EmojiProvider* instance();

    QPixmap getEmoji(qint64 id, const QSize& size);

private:
    EmojiProvider();

private:
    QCache<qint64, QPixmap> emojiCache;
};

#endif // EMOJIPROVIDER_H
