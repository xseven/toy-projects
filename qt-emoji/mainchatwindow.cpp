#include <QBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QTextDocument>
#include <QPushButton>
#include <QAbstractTextDocumentLayout>
#include <QTextFrame>

#include <cassert>

#include "emojiprovider.h"
#include "emojitextedit.h"
#include "emojitextobject.h"
#include "emojipopupwindow.h"

#include "mainchatwindow.h"

MainChatWidget::MainChatWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

MainChatWidget::~MainChatWidget()
{

}

void MainChatWidget::setupConnections()
{
    //[TBI] Possible content check for emojis
#if 0
    connect(ui.message.textEdit->document(), &QTextDocument::contentsChange, this, [](int pos, int rem,int add) noexcept {

    });
#endif

    connect(ui.message.emojiButton, &QPushButton::clicked, this, [=]() noexcept {
       if(!emojiWindow->isVisible()) {
        emojiWindow->setVisible(true);

        auto globalEmojiButtonCenter = ui.message.emojiButton->mapToGlobal(ui.message.emojiButton->rect().center());
        auto emojiWindowRect = emojiWindow->rect();
        emojiWindowRect.moveCenter(globalEmojiButtonCenter);

        emojiWindow->setGeometry(emojiWindowRect);
       }
    });

    connect(qobject_cast<EmojiPopupWindow*>(emojiWindow), &EmojiPopupWindow::emojiInserted, this, [=](qint64 id) noexcept {
		auto cursor = ui.message.textEdit->textCursor();
        insertEmojiIntoCursor(id, QSize(16, 16), &cursor);
		ui.message.textEdit->setTextCursor(cursor);
        ui.message.textEdit->setFocus();
    });

    connect(ui.message.sendButton, &QPushButton::clicked, this, &MainChatWidget::sendMessage);
}

void MainChatWidget::setupUi()
{
    //[Central layout]
    setLayout(new QVBoxLayout);

    //[Splitter]
    {
        ui.verticalSplitter = new QSplitter(this);
        ui.verticalSplitter->setOrientation(Qt::Vertical);

        layout()->addWidget(ui.verticalSplitter);
    }

    //[ChatView & EditLayout]
    {
        ui.chatHistory = new QTextEdit(ui.verticalSplitter);
        ui.chatHistory->setReadOnly(true);

        ui.verticalSplitter->addWidget(ui.chatHistory);

        ui.message.containerWidget = new QWidget(ui.verticalSplitter);
        ui.message.containerLayout = new QHBoxLayout;
        ui.message.containerWidget->setLayout(ui.message.containerLayout);

        ui.verticalSplitter->addWidget(ui.message.containerWidget);
    }

    //[]
    {
        ui.message.textEdit = new TextEditEmoji(ui.message.containerWidget);
        ui.message.textEdit->setMinimumHeight(40);

        ui.message.emojiButton = new QPushButton(ui.message.containerWidget);
        ui.message.emojiButton->setFixedSize(32, 32);
        ui.message.emojiButton->setIcon(QIcon("://emoji-button.png"));

        ui.message.sendButton = new QPushButton(ui.message.containerWidget);
        ui.message.sendButton->setFixedSize(32, 32);
        ui.message.sendButton->setIcon(QIcon("://send-button.png"));

        ui.message.containerLayout->addWidget(ui.message.textEdit, 1);
        ui.message.containerLayout->addWidget(ui.message.emojiButton);        
        ui.message.containerLayout->addWidget(ui.message.sendButton);
    }

    //[]
    ui.verticalSplitter->setStretchFactor(0, 4);
    ui.verticalSplitter->setStretchFactor(1, 1);

    ui.verticalSplitter->setCollapsible(1, false);

    //[]
    emojiWindow = new EmojiPopupWindow(this);
    emojiWindow->setFixedSize(172, 128);
    emojiWindow->setWindowFlags(Qt::Popup);
    emojiWindow->setVisible(false);

    //[]
    auto emojiInterface = new EmojiTextObject(this);
    ui.message.textEdit->document()->documentLayout()->registerHandler(EmojiTextObject::EmojiTextFormat
                                                                       , emojiInterface);

    ui.chatHistory->document()->documentLayout()->registerHandler(EmojiTextObject::EmojiTextFormat
                                                                       , emojiInterface);
}

void MainChatWidget::sendMessage() const noexcept
{
    if(!ui.message.textEdit->document()->isEmpty()) {
        auto msg = serializeToMessage(ui.message.textEdit->document());
        applyToChatHistory(msg);

        ui.message.textEdit->clear();        
    }

    ui.message.textEdit->setFocus();
}

//Artificial one. In real life better to use protobuf/thrift to serialize messageetc
MainChatWidget::ChatMessage MainChatWidget::serializeToMessage(const QTextDocument* doc) const noexcept
{
    ChatMessage msg;

    auto block = ui.message.textEdit->document()->firstBlock();

    while(block.isValid()) {

        if(!msg.text.isEmpty()) {
            msg.text += "\n";
        }

        for (auto i = block.begin(); i != block.end(); ++i) {
            if(i.fragment().charFormat().objectType() == EmojiTextObject::EmojiTextFormat) {
                auto emojiCode = i.fragment().charFormat().property(EmojiTextObject::EmojiCode).value<qint64>();

                int placeholder = 0;
                if(msg.placeholders.contains(emojiCode)) {
                    placeholder = msg.placeholders.value(emojiCode);
                } else {
                    placeholder = msg.placeholders.size() + 1;
                    msg.placeholders.insert(emojiCode, placeholder);
                }

                assert(placeholder != 0);

                msg.text += QStringLiteral("@%1").arg(QString::number(placeholder));
            } else {
                msg.text += i.fragment().text();
            }
        }

        block = block.next();
    }

	return std::move(msg);
}


//Function is a quite artificial one in order to simulate send/recv/apply chain
void MainChatWidget::applyToChatHistory(const MainChatWidget::ChatMessage& message) const noexcept
{
    auto cursor = ui.chatHistory->textCursor();

    if(!ui.chatHistory->document()->isEmpty()) {
        cursor.insertBlock();
    }

    cursor.beginEditBlock();
    cursor.insertText(tr("<You>: "));

    for(const auto& text : message.text) {
        auto rawText = text;
        if(!rawText.isEmpty()) {
            if(rawText.front() == '@') {
                rawText.remove(0, 1);
                int placeholder = rawText.toLong();

                if(message.placeholders.values().contains(placeholder)) {
                    auto emojiId = message.placeholders.key(placeholder);

					insertEmojiIntoCursor(emojiId, QSize(16, 16), &cursor);
                }

            } else {
                cursor.insertText(text);
            }
        }
    }
    
    cursor.endEditBlock();
}

void MainChatWidget::insertEmojiIntoCursor(qint64 emojiId, const QSize& emojiSize, QTextCursor* cursor) const noexcept
{
    auto provider = EmojiProvider::instance();

    auto emojiImage = provider->getEmoji(emojiId, emojiSize);
    assert(!emojiImage.isNull());

    if(!emojiImage.isNull()) {
        QTextCharFormat emojiCharFormat;

        emojiCharFormat.setObjectType(EmojiTextObject::EmojiTextFormat);
        emojiCharFormat.setProperty(EmojiTextObject::EmojiData, emojiImage);
        emojiCharFormat.setProperty(EmojiTextObject::EmojiCode, QString::number(emojiId));
		        
        cursor->insertText(QString(QChar::ObjectReplacementCharacter), emojiCharFormat);        
    }
}
