#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>

class QSplitter;
class QTextEdit;
class QHBoxLayout;
class QPushButton;
class QTextDocument;
class QTextCursor;

class MainChatWidget : public QWidget
{
    Q_OBJECT

public:
    MainChatWidget(QWidget *parent = 0);
    ~MainChatWidget();

    struct ChatMessage {
      QStringList text;
      QMap<qint64, int> placeholders;
    };

public slots:
    void sendMessage() const noexcept;

private:
    void setupUi();
    void setupConnections();

    ChatMessage serializeToMessage(const QTextDocument* doc) const noexcept;
    void applyToChatHistory(const ChatMessage& message) const noexcept;
    void insertEmojiIntoCursor(qint64 emojiId, const QSize& emojiSize, QTextCursor* cursor) const noexcept;

private:
    struct {
        QSplitter* verticalSplitter;

        QTextEdit* chatHistory;

        struct {
            QWidget* containerWidget;
            QHBoxLayout* containerLayout;
            QPushButton* emojiButton;
            QTextEdit* textEdit;
            QPushButton* sendButton;
        } message = {};

    } ui = {};

    QWidget* emojiWindow {};
};

#endif // WIDGET_H
