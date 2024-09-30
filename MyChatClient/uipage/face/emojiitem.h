#ifndef EMOJIITEM_H
#define EMOJIITEM_H

#include <QLabel>



class EmojiItem : public QLabel
{
    Q_OBJECT

public:
    EmojiItem(int index,const QString &fileName,QWidget *parent = nullptr);
    ~EmojiItem();

signals:
    void sigSelectEmoji(int faceIndex);

private:
    int m_faceIndex; // 表情索引

    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // EMOJIITEM_H
