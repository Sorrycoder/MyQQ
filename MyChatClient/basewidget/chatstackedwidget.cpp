#include"chatstackedwidget.h"
#include <QPainter>
#include <QPropertyAnimation>
#include<QTransform>
ChatStackedWidget::ChatStackedWidget(QWidget *parent)
    :QStackedWidget(parent)
{
    m_nextpageIndex=0;
    m_isAnimation=false;

}
ChatStackedWidget::~ChatStackedWidget()
{

}

void ChatStackedWidget::paintEvent(QPaintEvent *event)
{
    if(m_isAnimation){

        QPainter p(this);
        if(iRotateVal > 0&&iRotateVal <= 90)
        {
            QPixmap pix(currentWidget()->size());
            currentWidget()->render(&pix);
            //旋转
            QTransform transform;
            transform.translate(width()/2,0);//先平移到中心再开始旋转
            transform.rotate(iRotateVal,Qt::YAxis);
            p.setTransform(transform);
            p.drawPixmap(-1*width()/2,0,pix);
        }
        else  if(iRotateVal > 90 && iRotateVal <= 180)
        {
            QPixmap pix(widget(m_nextpageIndex)->size());
            widget(m_nextpageIndex)->render(&pix);
            //旋转
            QTransform transform;
            transform.translate(width()/2,0);//先平移到中心再开始旋转
            transform.rotate(iRotateVal+180,Qt::YAxis);
            p.setTransform(transform);
            p.drawPixmap(-1*width()/2,0,pix);
        }
        else if(iRotateVal > -180 &&iRotateVal <= -90)
        {

            QPixmap pix(widget(m_nextpageIndex)->size());
            widget(m_nextpageIndex)->render(&pix);
            //旋转
            QTransform transform;
            transform.translate(width()/2,0);//先平移到中心再开始旋转
            transform.rotate(iRotateVal+180,Qt::YAxis);
            p.setTransform(transform);
            p.drawPixmap(-1*width()/2,0,pix);
        }
        else  if(iRotateVal > -90 && iRotateVal <=0)
        {
            QPixmap pix(currentWidget()->size());
            currentWidget()->render(&pix);
            //旋转
            QTransform transform;
            transform.translate(width()/2,0);//先平移到中心再开始旋转
            transform.rotate(iRotateVal,Qt::YAxis);
            p.setTransform(transform);
            p.drawPixmap(-1*width()/2,0,pix);
        }
    }else
    {
        QWidget::paintEvent(event);
    }
}

void ChatStackedWidget::animation(int pageIndex)//动画效果
{
     if(m_isAnimation)//正在反转
     {
         return;
     }
    m_nextpageIndex=pageIndex;


    int offsetX=frameRect().width();
    int offsetY=frameRect().height();
    widget(pageIndex)->setGeometry(0,0,offsetX,offsetY);

    QPropertyAnimation *animation=new QPropertyAnimation(this,"RotateVal");
    //持续时间
    animation->setDuration(700);
    //设置动画缓和曲线
    animation->setEasingCurve(QEasingCurve::Linear);
    //设置动画起始值
    animation->setStartValue(m_startVal);
    //设置动画结束值
    animation->setEndValue(m_endVal);

    connect(animation,&QPropertyAnimation::valueChanged,this,&ChatStackedWidget::on_ValueChanged);
    connect(animation,&QPropertyAnimation::finished,this,&ChatStackedWidget::on_Finished);

    m_isAnimation=true;

    currentWidget()->hide();
    animation->start();

}

void ChatStackedWidget::on_ValueChanged()
{
    //值改变了以后
    repaint();
}

void ChatStackedWidget::on_Finished()
{
    //动画结束
    m_isAnimation=false;
    widget(m_nextpageIndex)->show();
    widget(m_nextpageIndex)->raise();

    setCurrentWidget(widget(m_nextpageIndex));
    repaint();
}


