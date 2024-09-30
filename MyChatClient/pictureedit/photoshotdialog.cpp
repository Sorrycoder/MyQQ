#include "photoshotdialog.h"

#include <QPainter>


PhotoShotDialog::PhotoShotDialog(QWidget *parent) :
    QWidget(parent)

{
    //设置窗口透明和位置
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,parentWidget()->width(),parentWidget()->height());
    m_cutDialog=new CutDialog(this);



}

PhotoShotDialog::~PhotoShotDialog()
{

}

QRect PhotoShotDialog::getCutGeometry()
{
    return  m_cutDialog->geometry();
}


void PhotoShotDialog::paintEvent(QPaintEvent *event)
{
    QPainterPath painterPath;
    QPainterPath p;
    p.addRect(x(),y(),this->rect().width(),this->rect().height());//头像显示窗口
    painterPath.addRect(m_cutDialog->geometry());//头像剪切窗口矩形

    QPainterPath drawPath= p.subtracted(painterPath);//获取painterPath以外的区域
    QPainter painter(this);
    painter.setOpacity(0.5);//设置透明度
    painter.fillPath(drawPath,QBrush(Qt::black));
}
