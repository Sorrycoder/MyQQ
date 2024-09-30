#include "loginwidget.h"
#include "ui_loginwidget.h"
#include"mainwindow.h"
#include<QFile>
#include <QMouseEvent>
#include <QMovie>

#include "databasemagr.h"
LoginWidget::LoginWidget(QWidget *parent)
    : CustomMoveWidget(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //myHelper::setStyle("default");//设置样式，上面的
    ui->stackedWidget->setCurrentIndex(0);


    InitWidget();

    //初始化ClientSocket
    m_tcpSocket=new ClientSocket;
    //connect(m_tcpSocket,&ClientSocket::signalMessage,this,&LoginWidget::onSignalMessage);
    connect(m_tcpSocket,&ClientSocket::signalStatus,this,&LoginWidget::SltTcpStatus);
    //检查是否连接到服务器
    m_tcpSocket->CheckConnected();


}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_btnWinMenu_clicked()
{
    ui->stackedWidget->setStartVal(0);//翻转起始值
    ui->stackedWidget->setEndVal(180);//翻转结束值.0~180度，翻转

    ui->stackedWidget->animation(1);
}


void LoginWidget::on_btnCancel_clicked()
{
    ui->stackedWidget->setStartVal(0);//翻转起始值
    ui->stackedWidget->setEndVal(-180);//翻转结束值.

    ui->stackedWidget->animation(0);
}


void LoginWidget::on_btnLogin_clicked()
{

    //检查是否连接到服务器
    m_tcpSocket->CheckConnected();
    QString username=ui->lineEditUser->text();
    QString password=ui->lineEditPasswd->text();
    //构建Json对象
    QJsonObject json;
    json.insert("name", username);
    json.insert("passwd", password);

    m_tcpSocket->SltSendMessage(Login,json);




}

//void LoginWidget::onSignalMessage(const quint8 &type, const QJsonValue &dataVal)
//{

//}
void LoginWidget::SltTcpStatus(const quint8 &state)
{
    switch (state) {

    case ConnectedHost://01
    {
        m_bConnected = true;
        //ui->labelWinTitle->setText("已连接服务器");
    }
        break;
    case DisConnectedHost: {//02
        m_bConnected = false;
//        ui->labelWinTitle->setText(tr("服务器已断开"));
    }
        break;
        // 登陆验证成功
    case LoginSuccess://03
    {
        //成功后取消信号槽
        disconnect(m_tcpSocket, &ClientSocket::signalStatus, this, &LoginWidget::SltTcpStatus);
        //disconnect(m_tcpSocket,&ClientSocket::signalMessage,this,&LoginWidget::onSignalMessage);

        // 登录成功后，保存当前用户
        MyApp::m_strUserName = ui->lineEditUser->text();
        MyApp::m_strPassword = ui->lineEditPasswd->text();
        MyApp::SaveConfig();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow();
        if (!QFile::exists(MyApp::m_strHeadFile)) {
            myHelper::Sleep(100);
            QJsonObject jsonObj;
            jsonObj.insert("from", MyApp::m_nId);
            jsonObj.insert("id",  -2);
            jsonObj.insert("msg", myHelper::GetFileNameWithExtension(MyApp::m_strHeadFile));
            m_tcpSocket->SltSendMessage(GetFile, jsonObj);
            myHelper::Sleep(100);
        }

        //emit signalLoginSuccess(MyApp::m_strUserName, MyApp::m_strPassword);

        // 居中显示
        myHelper::FormInCenter(mainWindow);
        mainWindow->SetSocket(m_tcpSocket, MyApp::m_strUserName);
        mainWindow->show();
        this->close();
    }
        break;
    case LoginPasswdError:
    {
        CMessageBox::Infomation(this, "登录失败，请检查用户名和密码！");
    }
        break;
    case LoginRepeat:
    {
        CMessageBox::Infomation(this, "登录失败，该账户已登录！");
    }
        break;

    case RegisterOk:
    {
        CMessageBox::Infomation(this, "该账号注册成功！请点击登录！");
    }
        break;
    case RegisterFailed:
    {
        CMessageBox::Infomation(this, "该账号已经注册！请点击登录！");
    }
        break;
    default:
        break;
    }

}

/**
 * @brief LoginWidget::InitWidget
 * 界面初始化
 */
void LoginWidget::InitWidget()
{
    QMovie *movie = new QMovie(this);

    movie->setFileName(":/resource/background/back.gif");
    ui->labelGif->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->labelGif->setMovie(movie);
    movie->start();
    // 头像
    ui->labelUserHead->setPixmap(QPixmap(":/resource/head/HeadImage.png"));

    // 加载配置信息
    ui->lineEditUser->SetIcon(QPixmap(":/resource/common/ic_user.png"));
    ui->lineEditUser->setText(MyApp::m_strUserName);
    ui->lineEditPasswd->SetIcon(QPixmap(":/resource/common/ic_lock.png"));    // 加载之前的配置

    if (ui->checkBoxPasswd->isChecked())
    {
        ui->lineEditPasswd->setText(MyApp::m_strPassword);
    }

    QStringList valueList;
    valueList << "xiaoni" << "zhangsan" << "dengmc" << "wangwu";

    //自动完成建议栏
    QCompleter *completer = new QCompleter(valueList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEditUser->setCompleter(completer);

    // 信号槽
    connect(ui->lineEditUser, SIGNAL(editingFinished()), this, SLOT(SltEditFinished()));
    connect(ui->lineEditUser, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));
    connect(ui->lineEditPasswd, SIGNAL(returnPressed()), this, SLOT(on_btnLogin_clicked()));
}
/*
//拖动窗口实现
void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        is_mousepress=true;
    }
    QPoint globalPos=event->globalPos();
    QPoint windowPos=this->pos();
    m_offset=globalPos-windowPos;

}
void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(is_mousepress==true)
        this->move(event->globalPos()-m_offset);

}
void LoginWidget::mouseReleaseEvent(QMouseEvent *event)
{
    is_mousepress=false;
}
*/
/**
 * @brief LoginWidget::SltEditFinished
 * 编辑完成
 */
void LoginWidget::SltEditFinished()
{
    QString text = ui->lineEditUser->text();
    if (QString::compare(text, QString("")) != 0) {
//        bool flag = valueList.contains(text, Qt::CaseInsensitive);
//        if (!flag) {
////            ui->lineEditUser->setText(text);
//        }
        qDebug() << "text edit finished" << text;
    }
}




void LoginWidget::on_btnRegister_clicked()
{
    QJsonObject json;
    json.insert("name", ui->lineEditUser->text());
    json.insert("passwd", ui->lineEditPasswd->text());

    m_tcpSocket->SltSendMessage(Register, json);
}

void LoginWidget::on_btnSaveFCfg_clicked()
{
    MyApp::m_strHostAddr=ui->lineEditHostAddr->text();
    MyApp::m_nMsgPort=ui->lineEdit_HostMsgPort->text().toInt();
    MyApp::m_nFilePort=ui->lineEdit_HostFilePort->text().toInt();
    m_tcpSocket->CheckConnected();
    //检查并连接tcp
//qDebug()<<"MyApp::m_strHostAddr"<<MyApp::m_strHostAddr;
//qDebug()<<"MyApp::m_nMsgPort"<<MyApp::m_nMsgPort;
    // 保存配置
    MyApp::SaveConfig();
    CMessageBox::Infomation(this, tr("配置保存成功，重启生效!"));
    ui->stackedWidget->setStartVal(0);//翻转起始值
    ui->stackedWidget->setEndVal(-180);//翻转结束值.

    ui->stackedWidget->animation(0);

}
