#include "pictureedit.h"
#include "ui_pictureedit.h"
#include<QDebug>
#include <QFileDialog>


PictureEdit::PictureEdit(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PictureEdit)
{
    ui->setupUi(this);

    //裁剪窗口显示
    m_psDialog=new PhotoShotDialog(ui->label);
    //m_psDialog->show();

    //加载图片
    on_btnLoad_clicked();


}

PictureEdit::~PictureEdit()
{
    delete ui;
}

//void PictureEdit::loadPicture()
//{
//    QString filename = QFileDialog::getOpenFileName(this, "选择图片", "", "image (*.png *.jpg)");
//    //m_srcPix.load("D:\\Qt\\QtProject\\PictureEdit\\images\\1.jpg");
//    m_srcPix.load(filename);

//    m_scaledPix=m_srcPix.scaled(ui->label->width(),ui->label->height());

//    ui->label->setPixmap(m_scaledPix);

//    connect(ui->btnCancel,&QPushButton::clicked,this,&PictureEdit::signalClose);

//}


void PictureEdit::on_btnSave_clicked()
{
    //截图

    QPixmap pixmap= m_scaledPix.copy(m_psDialog->getCutGeometry()).scaled(64,64);
    if(!pixmap.isNull())
    {
        pixmap.save(MyApp::m_strHeadPath+QString("%1_head_64.png").arg(MyApp::m_nId),"png");
        MyApp::m_strHeadFile=MyApp::m_strHeadPath+QString("%1_head_64.png").arg(MyApp::m_nId);
    }
    //聊天窗口需要的图片
    pixmap=m_scaledPix.copy(m_psDialog->getCutGeometry()).scaled(48,48);
    if(!pixmap.isNull())
    {
        pixmap.save(MyApp::m_strHeadPath+QString("%1_head_64.png").arg(MyApp::m_nId),"png");
    }
    emit signalCutHeadOk();

}

void PictureEdit::on_btnLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "选择图片", "", "image (*.png *.jpg)");
    //m_srcPix.load("D:\\Qt\\QtProject\\PictureEdit\\images\\1.jpg");
    m_srcPix.load(filename);

    m_scaledPix=m_srcPix.scaled(ui->label->width(),ui->label->height());

    ui->label->setPixmap(m_scaledPix);

    connect(ui->btnCancel,&QPushButton::clicked,this,&PictureEdit::signalClose);
}

void PictureEdit::on_btnCancel_clicked()
{
    emit signalClose();
}
