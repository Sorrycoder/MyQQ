#include "iplineeidt.h"
#include<QSizePolicy>
#include<QHBoxLayout>
#include <QLabel>
#include<QRegularExpression>
#include<QRegularExpressionValidator>
#include <QEvent>
#include <QKeyEvent>
IPLineEidt::IPLineEidt(QWidget *parent)
    :QLineEdit(parent)
{
    QHBoxLayout *Hlayout=new QHBoxLayout(this);
    Hlayout->setContentsMargins(1,1,1,1);

    QRegularExpression regexp("(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})");

    QLabel *labelDot[3];
    for(int i=0;i<IP_INPUT_SIZE;i++)//4栏
    {
        m_lineEdit[i]=new QLineEdit(this);
        m_lineEdit[i]->setProperty("ip",true);
        //保留线条编辑是否使用边框绘制本身
        m_lineEdit[i]->setFrame(false);
        //输入框最大长度为3
        m_lineEdit[i]->setMaxLength(3);
        //居中
        m_lineEdit[i]->setAlignment(Qt::AlignCenter);
        //保存小部件默认布局
        m_lineEdit[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);//根据内容使用空间
        //给edit设置值一个验证规则
        m_lineEdit[i]->setValidator(new QRegularExpressionValidator(regexp,this));

        //安装事件过滤器
        m_lineEdit[i]->installEventFilter(this);

        Hlayout->addWidget(m_lineEdit[i]);

        if(i<3)
        {
            labelDot[i]=new QLabel(this);
            labelDot[i]->setText(".");
            labelDot[i]->setFixedWidth(2);
            Hlayout->addWidget(labelDot[i]);
        }
        //最外层的Lineedit仅可读---IPLineEidt
        this->setReadOnly(true);
        //第一格获得焦点
        m_lineEdit[0]->setFocus();
        m_lineEdit[0]->selectAll();
    }

}


IPLineEidt::~IPLineEidt()
{

}


bool IPLineEidt::eventFilter(QObject *watched, QEvent *event)
{
    if(children().contains(watched)&&QEvent::KeyPress==event->type())//只有对当前对象的子对象且键盘事件进行事件处理
    {
        QKeyEvent *keyEvent=dynamic_cast<QKeyEvent*>(event);//获得当前输入键
        QLineEdit *pCurrentEdit=qobject_cast<QLineEdit*>(watched);//转为lineedit的指针以便进行操作

        switch (keyEvent->key())
        {
            case Qt::Key_0:
            case Qt::Key_1:
            case Qt::Key_2:
            case Qt::Key_3:
            case Qt::Key_4:
            case Qt::Key_5:
            case Qt::Key_6:
            case Qt::Key_7:
            case Qt::Key_8:
            case Qt::Key_9:
            {
                QString strtext= pCurrentEdit->text();
                if(strtext.length()<=3&&strtext.toInt()*10>255)//当前edit栏输入太大，>255
                {
                    int index=getIndex(pCurrentEdit);
                    if(index!=-1&&index!=3)//index合法，跳到下一个lineEdit
                    {
                        m_lineEdit[index+1]->setFocus();
                        m_lineEdit[index+1]->selectAll();
                    }
                }
                return QLineEdit::eventFilter(watched, event);
            }
            break;
            case Qt::Key_Left:
            {
                if(!pCurrentEdit->cursorPosition())
                {
                    int index=getIndex(pCurrentEdit);
                    if(index != -1 && index != 0)
                    {
                        m_lineEdit[index - 1]->setFocus();
                        int length=m_lineEdit[index - 1]->text().length();
                        m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(watched, event);
            }
            case Qt::Key_Right:
            {
                if(!pCurrentEdit->cursorPosition())
                {
                    int index=getIndex(pCurrentEdit);
                    if(index != -1 && index != 3)
                    {
                        m_lineEdit[index + 1]->setFocus();
                        int length=m_lineEdit[index + 1]->text().length();
                        m_lineEdit[index + 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(watched, event);
            }
            break;
            case Qt::Key_Backspace://后退键
            {
                QString strtext= pCurrentEdit->text();
                if(strtext.isEmpty()){
                    int index=getIndex(pCurrentEdit);
                    if(index != -1 && index != 0)
                    {
                        m_lineEdit[index - 1]->setFocus();
                        int length=m_lineEdit[index - 1]->text().length();
                        m_lineEdit[index - 1]->setCursorPosition(length ? length : 0);
                    }
                }
                return QLineEdit::eventFilter(watched, event);
            }
            break;
            case Qt::Key_Period://.键,如果输入(.)，跳到下一个
            {
                int index=getIndex(pCurrentEdit);
                if(index != -1 && index != 3)
                {
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index + 1]->selectAll();
                }

                return QLineEdit::eventFilter(watched, event);
            }
            break;
        }
    }
    return QLineEdit::eventFilter(watched, event);
}

int IPLineEidt::getIndex(QLineEdit *pLineEdit)
{
    int index=-1;
    for(int i=0;i<IP_INPUT_SIZE;i++)
    {
        if(pLineEdit==m_lineEdit[i])
        {
            index=i;
        }
    }
    return index;
}

QString IPLineEidt::text()
{
    //获取IP地址
    QString ip;
    for(int i=0;i<IP_INPUT_SIZE;i++)
    {
        ip.append(m_lineEdit[i]->text());
        if(i!=3)
        {
            ip.append(".");
        }
    }
    return ip;
}
