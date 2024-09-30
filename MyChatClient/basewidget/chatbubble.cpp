#include "chatbubble.h"
#include <QDebug>

#include "global.h"
#include "myapp.h"

#include <QFileInfo>
#include <QFileIconProvider>
#include <QDesktopServices>

#include "iteminfo.h"

BubbleList::BubbleList(QWidget *parent) :
    QWidget(parent)
{
    //初始化一些变量，右键菜单
    initVars();
    //初始化主布局
    initWgts();
    initStgs();
    //初始化连接
    initConns();

}

BubbleList::~BubbleList()
{
    d->clear();

}

/*!
  private utility functions
*/
void BubbleList::initVars()
{
}
//初始化主布局
void BubbleList::initWgts()
{
    mainLayout = new QHBoxLayout(this);
    scrollbar = new QScrollBar(this);
    d = new BubbleListPrivate(this);
    this->setMinimumWidth(300);

    mainLayout->addWidget(d);
    mainLayout->addWidget(scrollbar);
    setLayout(mainLayout);

}

void BubbleList::initStgs()
{
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    scrollbar->setRange(0,0);
    scrollbar->hide();
}

void BubbleList::initConns()
{
    //滚动条和索引同步
    connect(scrollbar,SIGNAL(valueChanged(int)),d,SLOT(setCurrentIndex(int)));
    connect(d,SIGNAL(sig_setMaximum(int)),this,SLOT(setMaximum(int)));
    connect(d,SIGNAL(sig_setCurrentIndex(int)),scrollbar,SLOT(setValue(int)));
    connect(d,SIGNAL(sig_itemClicked(QString)),this,SIGNAL(sig_itemClicked(QString)));
    connect(d,SIGNAL(signalDownloadFile(QString)),this,SIGNAL(signalDownloadFile(QString)));


}

void BubbleList::setBackgroundColorPublic( QString colorName)
{
    d->setBackgroundColorPrivate(colorName); // 调用私有类中的方法
}
void BubbleList::setLeftColorPublic (QString colorName)
{
    d->setLeftColorPrivate(colorName); // 调用私有类中的方法
}
void BubbleList::setRightColorPublic( QString colorName)
{
    d->setRightColorPrivate(colorName); // 调用私有类中的方法
}
void BubbleList::calcGeo()
{

}

/*!
  private slot functions
*/
void BubbleList::setMaximum(int max)
{
    if(max == 0)
    {
        scrollbar->hide();
    }else{
        scrollbar->show();
    }
    scrollbar->setRange(0,max);
}

/*!
  protected functions of PYFlashList
*/
void BubbleList::resizeEvent(QResizeEvent *)
{

}

/*!
  public interfaces of PYFlashList
*/
void BubbleList::addItem(ItemInfo *item)
{
    d->addItem(item);
}

void BubbleList::addItems(QVector<ItemInfo *> items)
{
    if (items.size() < 1) return;
    d->addItems(items);
}

void BubbleList::clear()
{
    d->clear();
}

void BubbleList::render()
{
    d->render();
}


/*************************************/
/*!   BubbleListPrivate section    !*/
/*************************************/
BubbleListPrivate::BubbleListPrivate(QWidget* parent):QWidget(parent)
{
    initVars();
    initSettings();
}

BubbleListPrivate::~BubbleListPrivate()
{

}
//初始化右键菜单和变量
void BubbleListPrivate::initVars()
{
    m_currIndex = 0;
    m_VisibleItemCnt = 0;
    m_ItemCounter = 0;

    m_bAllJobsDone = false;
    m_bHover       =  false;

    m_font = QFont("楷体", 12);

    m_backgroundColor=QString("#D3ECE4");
    m_leftColor=QString("#FFFFFF");
    m_rightColor=QString("#9EE656");


    // 右键菜单
    picRightButtonMenu = new QMenu(this);
    picRightButtonMenu->addAction("保存图片");
    picRightButtonMenu->addSeparator();
    picRightButtonMenu->addAction("复制到粘贴板");
    picRightButtonMenu->hide();

    // 右键菜单
    fileRightButtonMenu = new QMenu(this);
    fileRightButtonMenu->addAction("下载文件");
    fileRightButtonMenu->addSeparator();
    fileRightButtonMenu->addAction("打开文件");
    fileRightButtonMenu->addAction("打开文件目录");
    fileRightButtonMenu->hide();

    connect(fileRightButtonMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltFileMenuClicked(QAction*)));
}

void BubbleListPrivate::initSettings()
{
    setMouseTracking(true);
}

void BubbleListPrivate::calcGeo()
{
#if 0
    // 计算当前可以刷新的item数量
    qDebug() << "calc Geo" << m_currIndex; // 输出当前索引状态到调试控制台
    int nCnt = 0; // 可见项计数器初始化为0
    int nHeight = 0; // 当前累积高度初始化为0
    int nCurrIndex = m_currIndex; // 保存当前索引

    // 从当前索引开始向上查找可见的消息项
    while (nCurrIndex > 1) {
        nHeight = m_IIVec.at(nCurrIndex)->getItemHeight(); // 获取当前项的高度
        nHeight += ITEM_SPACE; // 加上项之间的空间

        // 如果累积高度超过组件可见高度，停止计算
        if (nHeight + 20 >= this->height())
            break;

        nCnt++; // 增加可见项计数
        nCurrIndex--; // 移动到上一个项
    }

    m_VisibleItemCnt = nCnt; // 更新当前可见项数量
//    m_currIndex = nCurrIndex - nCnt; // 可选：更新当前索引 - 这行被注释掉
#else
    // 计算可见项，假设所有项高度相同
    m_VisibleItemCnt = height() / (ITEM_HEIGHT + ITEM_SPACE + 10) + 1; // 通过整体高度计算可见项数
#endif

    // 计算不可见项数量
    int InvisibleItemCnt = m_IIVec.count() - m_VisibleItemCnt;

    // 如果有不可见项，则更新滚动条的最大值
    if (InvisibleItemCnt >= 0)
    {
        emit sig_setMaximum(InvisibleItemCnt); // 发出信号，更新最大滚动范围
    }
}

//向上滚动，更新可见索引
void BubbleListPrivate::wheelUp()
{

    m_currIndex++;
    if (m_currIndex > m_IIVec.size()) {
        m_currIndex = m_IIVec.size() - 1;
    }

    update();
    emit sig_setCurrentIndex(m_currIndex);
}
//向下滚动
void BubbleListPrivate::wheelDown()
{
    m_currIndex--;

    // 没有记录的时候按0算
    if (m_currIndex < 0)
    {
        m_currIndex = 0;
    }

    update();
    emit sig_setCurrentIndex(m_currIndex);
}

/**
 * @brief BubbleListPrivate::SltFileMenuClicked
 * @param acction
 */
void BubbleListPrivate::SltFileMenuClicked(QAction *action)
{
    QString strText = m_IIVec.at(m_selectedIndex)->GetText();
    QFileInfo fileInfo(strText);

    // 菜单操作
    if (action->text() == "下载文件")
    {
        // 如果文件存在，直接打开目录
        if (QFile::exists(strText)) {
            QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
            return;
        }

        // 获取文件拓展名，因为服务器下载文件只需要名字即可
        Q_EMIT signalDownloadFile(fileInfo.fileName());
    }
    else if (action->text() == "打开文件") {
        // 如果文件存在，直接打开目录
        if (QFile::exists(strText)) {
            QDesktopServices::openUrl(QUrl(strText));
            return;
        }
    }
    else if (action->text() == "打开文件目录") {
        QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
    }
}

/*!
  protected functions of BubbleListPrivate
*/
void BubbleListPrivate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing);
    drawBackground(&painter);
    drawItems(&painter);
}

void BubbleListPrivate::mouseMoveEvent(QMouseEvent *e)
{
    int nItemY = ITEM_SPACE;
    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
    {
        if (nItemY > this->height()) {
            break;
        }

        int nY = this->height() - nItemY;
        QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();
        if ((e->pos().y() < (nY) && (e->pos().y() > (nY - bubbleRect.height()))) &&
                ((e->pos().x() > bubbleRect.x()) &&
                 (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))))
        {
            quint8 nType = m_IIVec.at(nIndex)->GetMsgType();

            // 如果是图片或文件，可以直接打开
            if (Picture == nType || Files == nType) {
                this->setCursor(Qt::PointingHandCursor);
                m_bHover = true;
                m_nHoverItemIndex = nIndex;
                update();
                return;
            }
        }

        // 如果有改变，需要改变回来
        if (m_bHover) {
            m_bHover = false;
            m_nHoverItemIndex = -1;
            update();
            return;
        }
        nItemY += bubbleRect.height() + ITEM_SPACE;
    }

    this->setCursor(Qt::ArrowCursor);
}

/**
 * @brief BubbleListPrivate::mousePressEvent
 * 鼠标右键按下
 * @param e
 */
void BubbleListPrivate::mousePressEvent(QMouseEvent *e)
{
    if (Qt::RightButton == e->button()) {
        int nItemY = ITEM_SPACE;
        for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
        {
            if (nItemY > this->height()) {
                break;
            }

            int nY = this->height() - nItemY;
            QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();
            if ((e->pos().y() < (nY) && (e->pos().y() > (nY - bubbleRect.height()))) &&
                    ((e->pos().x() > bubbleRect.x()) &&
                     (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))))
            {
                // 当前选中item
                m_selectedIndex = nIndex;

                // 如果是图片或文件，可以直接打开
                if (Picture == m_IIVec.at(nIndex)->GetMsgType()) {
                    picRightButtonMenu->popup(e->globalPos());
                    return;
                }
                else if (Files == m_IIVec.at(nIndex)->GetMsgType()) {
                    fileRightButtonMenu->popup(e->globalPos());
                    return;
                }
            }
            nItemY += bubbleRect.height() + ITEM_SPACE;
        }
    }
}

/**
 * @brief BubbleListPrivate::mouseDoubleClickEvent
 * 鼠标双击item
 * @param e
 */
void BubbleListPrivate::mouseDoubleClickEvent(QMouseEvent *e)
{
    int nItemY = ITEM_SPACE;
    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
    {
        if (nItemY > this->height()) {
            break;
        }

        int nY = this->height() - nItemY;
        QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();
        if ((e->pos().y() < (nY) && (e->pos().y() > (nY - bubbleRect.height()))) &&
                ((e->pos().x() > bubbleRect.x()) &&
                 (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))))
        {
            // 如果是图片或文件，可以直接打开
            if (Picture == m_IIVec.at(nIndex)->GetMsgType() /*|| Files == m_IIVec.at(nIndex)->GetMsgType()*/)
            {
                QString strFile = m_IIVec.at(nIndex)->GetText();
                // 如果文件存在，可以打开
                if (QFile::exists(strFile)) {
                    QDesktopServices::openUrl(QUrl(strFile));
                }
                return;
            }

            Q_EMIT sig_itemClicked(m_IIVec.at(nIndex)->GetText());
            break;
        }
        nItemY += bubbleRect.height() + ITEM_SPACE;
    }
}

// resize
void BubbleListPrivate::resizeEvent(QResizeEvent *)
{
    calcGeo();
}

void BubbleListPrivate::leaveEvent(QEvent *)
{
    m_HoverRect = QRectF();

//    update();
}
void BubbleListPrivate::showEvent(QShowEvent *)
{
    calcGeo();
}

void BubbleListPrivate::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
    {
        wheelUp();
    }else{
        wheelDown();
    }
}

// 绘制背景
void BubbleListPrivate::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(m_backgroundColor))); // 使用更新后的背景颜色


    painter->drawRect(rect());
    painter->restore();
}


// 绘制ITEM
void BubbleListPrivate::drawItems(QPainter *painter)
{

    if(m_IIVec.count() == 0)//当前没有消息
    {
        return ;
    }

    // 绘制item区域
    painter->save();

    qreal nItemY = 0;
    int nWidth = this->width();
    nWidth = (0 == nWidth % 2) ? nWidth : nWidth + 1;

    // 起始位置抬高
    nItemY = ITEM_SPACE + ITEM_TITLE_HEIGHT;

    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)//遍历消息
    {
        if (nItemY > this->height()) {//超出组件高度
            break;
        }

        // 解析消息内容
        QString strMsg = m_IIVec.at(nIndex)->GetText();
        quint8 msgType = m_IIVec.at(nIndex)->GetMsgType();
        int faceIndex = m_IIVec.at(nIndex)->GetFace();//获取表情索引
        quint8 nOrientation = m_IIVec.at(nIndex)->GetOrientation();
        QString strPixmap   = m_IIVec.at(nIndex)->GetStrPixmap();



        qreal bubbleWidth = 0;
        qreal bubbleHeight = 0;
        // 计算右边的宽度x
        int nX = 0;
        int nY = 0;

        QPixmap pixmap;
        QPainterPath path;
        // 气泡和内容的矩形框
        QRectF msgRect   ;
        QRectF bobbleRect;
        // 计算文字的宽度
        QFontMetrics fm(m_font);

        // --------------------计算气泡和文字的绘制范围 ----------------------------//
        switch (msgType) {
        case Text://文字消息
        {
            bubbleWidth = fm.width(strMsg);
            bubbleHeight = fm.height();

            QStringList strMsgList = strMsg.split("\n");
            qreal nLineWidth = 0;//最大行宽
            qreal nLineHeight = 0;

            int nTextWidth = 0;//当前行宽
            // 换行符会影响计算宽度和高度，按行算
            for (int i = 0; i < strMsgList.size(); i++)
            {
                nTextWidth = fm.width(strMsgList.at(i));
                if ( nTextWidth>nLineWidth )
                {
                    nLineWidth = nTextWidth;
                    nLineWidth = nLineWidth < (nWidth * 2 / 3) ? nLineWidth : (nWidth * 2 / 3);//气泡宽度不能太大
                }
                nLineHeight = nLineWidth < (nWidth * 2 / 3) ? fm.height() * 1.5 : (((nTextWidth / ((nWidth * 2 / 3))) + 1) * ITEM_HEIGHT / 3 + 20);
                bubbleHeight += nLineHeight;
            }

            bubbleWidth = nLineWidth;


            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;
        }
            break;
        case Audio:

            break;
        case Picture:
        {
            pixmap = QPixmap(strMsg);
            if (pixmap.isNull()) {
                pixmap = QPixmap(":/resource/background/ic_picture.png");
            }
            // 图片过大限制
            if (pixmap.width() > 200 || pixmap.height() > 100) {
                pixmap = pixmap.scaled(200, 100);
            }

            bubbleWidth = pixmap.width();
            bubbleHeight = pixmap.height() + 10;

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;
        }
            break;
        case Face://表情消息
        {

            QString facePath=QString(":/resource/gif/%1.gif").arg(faceIndex);

            //qDebug()<<"Facepath:"<<facePath;
            pixmap = QPixmap(facePath);
            if (pixmap.isNull()) {
                pixmap = QPixmap(":/resource/gif/1.gif");
            }

            bubbleWidth = pixmap.width();
            bubbleHeight = pixmap.height() + 10;

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;


        }
            break;
        case Files:
        {
            QFileInfo fileInfo(strMsg);
            QString strSize = m_IIVec.at(nIndex)->GetFileSizeString();

            // 文件图标

            pixmap = QPixmap(":/resource/images/ic_zip.png");

            // 先判断文件是否存在
            if (QFile::exists(strMsg)) {
                strSize = "文件大小: ";
                strSize += myHelper::CalcSize(fileInfo.size());
#ifdef Q_OS_WIN32
                QFileIconProvider provider;
                QIcon icon = provider.icon(fileInfo);
                pixmap = icon.pixmap(48, 48);
#endif
            }
            else {
                strSize = "文件未下载";
            }

            // 计算文字的宽度
            QFont font("微软雅黑", 10);
            QFontMetrics fm(font);
            // 显示文件名，路径不显示
            strMsg = fileInfo.fileName();
            // 计算气泡的size
            if (fm.width(strMsg) < fm.width(strSize)) {
                bubbleWidth = fm.width(strSize) + pixmap.width() + 20;
            }
            else {
                bubbleWidth = fm.width(strMsg) + pixmap.width() + 20;
            }

            bubbleHeight = 60;

            bubbleWidth = bubbleWidth < (nWidth * 2 / 3) ? bubbleWidth: (nWidth * 2 / 3);
            bubbleWidth = bubbleWidth < 60 ? 60 : bubbleWidth;

            strMsg += "\n";
            strMsg += strSize;

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;
        }
            break;
        default:
            break;
        }

        // --------------------绘制气泡和文字 ----------------------------//
        if (  nOrientation==Right) {
            painter->save();
            painter->setPen(QPen(QColor("#D3D3D3"), 1));
            // 绘制边框
            painter->drawRoundedRect(nWidth - 55, nY, 50, 50, 2, 2);
            // 绘制头像
            QPixmap pixmap(strPixmap);
            if (!pixmap.isNull()) {
                painter->drawPixmap(nWidth - 55 + 1, nY + 1, 48, 48, pixmap);
            }
            painter->restore();

            // 绘制名字和当前时间
            painter->save();
            // 计算文字的宽度
            QFont font("微软雅黑", 8);
            QFontMetrics fm(font);
            QString strTitle = m_IIVec.at(nIndex)->GetDatetime() + "  ";
            strTitle += m_IIVec.at(nIndex)->GetName();

            painter->setPen(QColor("#666666"));
            painter->setFont(font);
            painter->drawText(nWidth - 55 - fm.width(strTitle) - 10, nY + fm.height() / 2, strTitle);
            painter->restore();

            // 根据消息内容计算气泡区域
            nX = (nWidth ) - 95 - bubbleWidth;
            if (nX < 0) {
                nX = -bubbleWidth - 95 + nWidth ;
            }

            // 计算气泡框
            nY += ITEM_TITLE_HEIGHT;
            bobbleRect = QRectF(nX, nY, bubbleWidth + 20, bubbleHeight);
            msgRect    = QRectF(nX + 10, nY, bubbleWidth, bubbleHeight);

            painter->save();
            // 计算气泡右边的三角
            path.addRoundedRect(bobbleRect, 10, 10);
            path.moveTo(nWidth - 75, nY + 15);
            path.lineTo(nWidth - 65, nY + 20);
            path.lineTo(nWidth - 75, nY + 25);
            //            painter->fillPath(path, itemGradient);
            painter->setPen(Qt::red);

            //painter->fillPath(path, QColor("#9EE656"));
            painter->fillPath(path, QColor(m_rightColor));
            painter->restore();
        }
        else {//消息来源左边
            // 绘制头像
            painter->save();
            painter->setPen(QColor("#D3D3D3"));
            // 绘制边框
            painter->drawRoundedRect(5, nY, 50, 50, 2, 2);

            // 绘制头像
            QPixmap pixmap(strPixmap);
            if (!pixmap.isNull()) {
                painter->drawPixmap(6, nY + 1, 48, 48, pixmap);
            }

            painter->restore();

            nX = 75;
            // 绘制名字和当前时间
            painter->save();
            // 计算文字的宽度
            QFont font("微软雅黑", 8);
            QFontMetrics fm(font);

            QString strTitle = m_IIVec.at(nIndex)->GetName() + " ";
            strTitle += m_IIVec.at(nIndex)->GetDatetime();

            painter->setPen(QColor("#666666"));
            painter->setFont(font);
            painter->drawText(nX, nY + fm.height() / 2, strTitle);
            painter->restore();

            // 绘制气泡区域
            painter->save();
            nY += ITEM_TITLE_HEIGHT;
            bobbleRect = QRectF(nX, nY, bubbleWidth + 20, bubbleHeight);
            // 绘制文字区域
            msgRect = QRectF(nX + 10, nY, bubbleWidth, bubbleHeight);

            // 左边三角
            path.addRoundedRect(bobbleRect, 5, 5);
            path.moveTo(nX, nY + 15);
            path.lineTo(nX - 10, nY + 20);
            path.lineTo(nX, nY + 25);
            // 背景色
            //painter->fillPath(path, QColor("#FFFFFF"));
            painter->fillPath(path, QColor(m_leftColor));
            painter->restore();
        }

        painter->setPen(QColor("#000000"));
        painter->setFont(m_font);

        if (Text == msgType) {

            painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);

        }
        else if (msgType==Picture  ) {
            painter->drawPixmap(nX + 10, nY + 5, pixmap.width(), pixmap.height(), pixmap);
        }
        else if (msgType==Face  ) {
            painter->drawPixmap(nX + 10, nY + 5, pixmap.width(), pixmap.height(), pixmap);
        }
        else if ( msgType==Files ) {
            painter->drawPixmap(nX + 10, nY + 5 + (48 - pixmap.height()) / 2, pixmap.width(), pixmap.height(), pixmap);
            msgRect.setX(msgRect.x() + pixmap.width() + 15);
            painter->save();
            painter->setPen(Qt::blue);
            QFont font("微软雅黑", 10);
            font.setUnderline(m_bHover && (nIndex == m_nHoverItemIndex));
            painter->setFont(font);
            painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);
            painter->restore();
        }

        m_IIVec.at(nIndex)->SetBobbleRect(bobbleRect);
        nItemY += (bobbleRect.height()) + ITEM_SPACE + ITEM_TITLE_HEIGHT;
    }
    painter->restore();
}




/*!
  public interfaces of BubbleListPrivate
 */
void BubbleListPrivate::addItem(ItemInfo *item)//添加聊天记录
{
    m_IIVec.push_front(item);
    m_currIndex = 0;
    update();
}

void BubbleListPrivate::addItems(QVector<ItemInfo *> items)//添加历史记录
{
    m_IIVec.clear();
    m_IIVec = items;
    m_currIndex = 0;
    update();
}

void BubbleListPrivate::clear()
{
    m_IIVec.clear();
    m_currIndex = 0;
    m_VisibleItemCnt = 0;
    m_ItemCounter = 0;

    m_bAllJobsDone = false;

    update();
}

void BubbleListPrivate::render()
{
    update();
}

/*!
  public slot functions of BubbleListPrivate
*/
void BubbleListPrivate::setCurrentIndex(int curIndex)
{
    if(curIndex == m_currIndex)
    {
        return ;
    }


    m_currIndex = curIndex;
}

void BubbleListPrivate::setBackgroundColorPrivate(QString colorName)
{
    m_backgroundColor = colorName; // 保存颜色
    update(); // 更新以重新绘制
}
void BubbleListPrivate::setLeftColorPrivate(QString colorName)
{
    m_leftColor = colorName; // 保存颜色
    update(); // 更新以重新绘制
}void BubbleListPrivate::setRightColorPrivate(QString colorName)
{
    m_rightColor = colorName; // 保存颜色
    update(); // 更新以重新绘制
}
