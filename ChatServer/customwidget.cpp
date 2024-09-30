#include "customwidget.h"
#include "myapp.h"

#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QMouseEvent>

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{

}

void CustomWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//////////////////////////////////////////////////////////////////////
/// \brief CustomMoveWidget::CustomMoveWidget
/// \param parent
/// 可移动窗体
CustomMoveWidget::CustomMoveWidget(QWidget *parent) :
    CustomWidget(parent)
{
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
}

CustomMoveWidget::~CustomMoveWidget()
{

}

/**
 * @brief CustomMoveWidget::mouseMoveEvent 鼠标点击移动
 * @param e
 */
void CustomMoveWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void CustomMoveWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void CustomMoveWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}

