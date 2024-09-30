#include "facedialog.h"
#include "ui_facedialog.h"
#include<QDebug>
FaceDialog::FaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FaceDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    QString fileName;
    //加载表情
    for(int i=0;i<132;i++)
    {
        fileName=QString(":/resource/gif/%1.gif").arg(i+1);

        addEmojiItem(fileName);
    }

    m_selectFaceIndex=0;
}

FaceDialog::~FaceDialog()
{
    delete ui;
}



void FaceDialog::addEmojiItem(QString fileName)
{
    //计算表情添加到哪里
    int row=m_emojiList.size()/(ui->tableWidget->columnCount());//emoji应该所在的行，
    int column=m_emojiList.size()%(ui->tableWidget->columnCount());//emoji应该所在的列，

    //QTableWidgetItem *twItem=new QTableWidgetItem;
    //ui->tableWidget->setItem(row,column,twItem);

    EmojiItem *emojiItem=new EmojiItem(m_emojiList.size()+1,fileName);
    connect(emojiItem,&EmojiItem::sigSelectEmoji,this,[=](int faceIndex){
        m_selectFaceIndex=faceIndex;
        this->hide();
        //qDebug()<<"m_selectFaceIndex:"<<m_selectFaceIndex;
    });


    ui->tableWidget->setCellWidget(row,column,emojiItem);

    m_emojiList.append(emojiItem);
}


void FaceDialog::on_pushButton_clicked()
{
    this->hide();
}

int FaceDialog::selectFaceIndex()
{
    return m_selectFaceIndex;
}
void FaceDialog::setSelectFaceIndex(int index)
{
    m_selectFaceIndex=index;
}

void FaceDialog::moveFaceLocation(QPoint p)
{
    this->move(p);
}

