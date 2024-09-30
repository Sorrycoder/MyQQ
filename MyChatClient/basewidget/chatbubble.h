#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QTimer>
#include <QMenu>
#include <QMovie>
#include "unit.h"


class ItemInfo;
class BubbleListPrivate;
class BubbleList: public QWidget
{
    Q_OBJECT
public:
    BubbleList(QWidget* parent = 0);
    ~BubbleList();

public:
    /// proxy public interfaces for PYFlashListPrivate
    //添加消息气泡
    void addItem(ItemInfo *item);
    void addItems(QVector<ItemInfo*> items);

    void clear();

    void render();

    void setCurrItem(const int &index) {
        scrollbar->setValue(index);


    }
    void setBackgroundColorPublic(QString colorName);//设置背景颜色
    void setLeftColorPublic(QString colorName);//设置左气泡颜色
    void setRightColorPublic(QString colorName);//设置右气泡颜色
protected:
    QSize sizeHint() const
    {
        return QSize(SIZE_HINT);
    }

    void resizeEvent(QResizeEvent *);


private:
    /// private utility functoins
    void initVars();
    void initWgts();
    void initStgs();
    void initConns();

    void calcGeo();

private Q_SLOTS:
    void setMaximum(int max);

private:
    QHBoxLayout* mainLayout;
    QScrollBar* scrollbar;
    BubbleListPrivate* d;

Q_SIGNALS:
    void sig_setCurrentIndex(int currIndex);
    void sig_itemClicked(const QString& str);
    void signalDownloadFile(const QString &fileName);
};

class BubbleListPrivate : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleListPrivate(QWidget *parent = 0);
    ~BubbleListPrivate();

public:
    /// public interfaces
    void addItem(ItemInfo *item);
    void addItems(QVector<ItemInfo*> items);

    void clear();

    void render();

    int itemCount() const
    {
        return m_IIVec.count();
    }

public:
    void setBackgroundColorPrivate(QString colorName);//设置背景色，私有
    void setLeftColorPrivate(QString colorName);//设置左气泡颜色
    void setRightColorPrivate(QString colorName);//设置右气泡颜色
public Q_SLOTS:
    void setCurrentIndex(int curIndex);

protected:
    void paintEvent(QPaintEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent *);

    void mouseDoubleClickEvent(QMouseEvent *e);

    void resizeEvent(QResizeEvent *);

    void leaveEvent(QEvent *);

    void showEvent(QShowEvent *);

    void wheelEvent(QWheelEvent *);

private:
    /// painting functions
    void drawBackground(QPainter* painter);

    void drawItems(QPainter* painter);

    void drawHoverRect(QPainter* painter);




private:
    /// private utility functoins
    void initVars();
    void initSettings();
    void calcGeo();
    void wheelUp();
    void wheelDown();

private:
    // 如果消息内容多了，不方便修改！~
    QVector<ItemInfo*> m_IIVec;//消息队列

    int m_currIndex;
    int m_selectedIndex;
    int m_VisibleItemCnt;
    int m_ItemCounter;

    bool m_bAllJobsDone;

    QRectF m_HoverRect;

    QString m_strHoverText;

    QFont m_font;

    bool    m_bHover;
    int     m_nHoverItemIndex;

    QString  m_backgroundColor;//背景颜色
    QString  m_leftColor;//左气泡颜色
    QString  m_rightColor;//右气泡颜色



private Q_SLOTS:
    void SltFileMenuClicked(QAction *action);
Q_SIGNALS:
    void sig_setMaximum(int max);
    void sig_setCurrentIndex(int currIndex);
    void sig_itemClicked(const QString& str);
    void signalDownloadFile(const QString &fileName);

private:
    QMenu *picRightButtonMenu;
    QMenu *fileRightButtonMenu;

};
#endif // CHATBUBBLE_H
