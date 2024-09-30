#include "emojiitem.h"
#include <QMouseEvent>
#include<QMovie>
EmojiItem::EmojiItem(int index,const QString &fileName,QWidget *parent)
    : QLabel(parent)
{
    m_faceIndex=index;

    QMovie *iconMovie=new QMovie;
    iconMovie->setFileName(fileName);
    this->setMovie(iconMovie);
    this->setMargin(4);

    iconMovie->start();
    iconMovie->stop();

    //this->setPixmap(QPixmap(fileName));

}

EmojiItem::~EmojiItem()
{

}

void EmojiItem::enterEvent(QEvent *event)
{
     QMovie *iconMovie=this->movie();
     iconMovie->start();
     return QLabel::enterEvent(event);

}

void EmojiItem::leaveEvent(QEvent *event)
{
    QMovie *iconMovie=this->movie();
    iconMovie->jumpToFrame(0);//跳转到第一帧
    iconMovie->stop();
    return QLabel::enterEvent(event);
}

void EmojiItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
            emit sigSelectEmoji(m_faceIndex); // 发射信号，传递表情索引
    }
        QLabel::mousePressEvent(event);

}

