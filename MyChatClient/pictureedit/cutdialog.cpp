#include "cutdialog.h"

#include <QMouseEvent>


CutDialog::CutDialog(QWidget *parent) :
    QWidget(parent)
{

    m_mouse_down=false;
    //setMouseTracking(true);
    setGeometry(0,0,150,150);

}

CutDialog::~CutDialog()
{

}

void CutDialog::mousePressEvent(QMouseEvent *event)
{
   m_startpoint=event->pos();
   //鼠标左键按下
   m_mouse_down=event->button()==Qt::LeftButton;

}
void CutDialog::mouseMoveEvent(QMouseEvent *event)
{
   //获取到拖动位置
    QPoint dragPoint=event->pos();
    if(m_mouse_down)
    {
        QPoint p=QPoint(pos().x()+dragPoint.x()-m_startpoint.x(),pos().y()+dragPoint.y()-m_startpoint.y());
        QPoint dragEnd=p;
        dragEnd.setX(dragEnd.x()+rect().width());
        dragEnd.setY(dragEnd.y()+rect().height());

        p.setX(p.x()<0 ?:p.x());
        p.setY(p.y()<0 ?:p.y());
        p.setX(dragEnd.x()>parentWidget()->width()?parentWidget()->width()-rect().width():p.x());
        p.setY(dragEnd.y()>parentWidget()->height()?parentWidget()->height()-rect().height():p.y());
        move(p);
    }
}
void CutDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse_down=false;
}
