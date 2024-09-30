/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : mainwindow.cpp --- MainWindow
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasemagr.h"
#include "myapp.h"
#include "unit.h"

#include "tcpserver.h"

#include <QApplication>
#include <QMenu>
#include <QNetworkInterface>
#include <QDateTime>
#include <QSystemTrayIcon>
#include <QHostAddress>
#include <QHostInfo>

#include <QMessageBox>
#include <QFileDialog>

#include <QCloseEvent>
#include <QTimerEvent>

#include <QDebug>

PictureEdit::PictureEdit(QWidget *parent) :
    CustomMoveWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelWinTitle->setText(tr("即时通信后台管理平台"));

    // 初始化网络
    InitNetwork();
}

PictureEdit::~PictureEdit()
{
    delete ui;
}

void PictureEdit::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


// 初始化网络
void PictureEdit::InitNetwork()
{
    tcpMsgServer = new TcpMsgServer(this);
    tcpFileServer = new TcpFileServer(this);

    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/resource/images/ic_app.png"));

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        // 我们使用IPv4地址
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            // 过滤127.0.0.1
            if (QString::compare("127.0.0.1", address.toString()))
            {
                ui->comboBoxAddress->addItem(address.toString());
            }
        }
    }

    QMenu *m_trayMenu = new QMenu(this);
    m_trayMenu->addAction("显示主面板");
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("退出");

    systemTrayIcon->setContextMenu(m_trayMenu);
    systemTrayIcon->show();

    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(SltTrayIcoClicked(QSystemTrayIcon::ActivationReason)));
    connect(m_trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltTrayIconMenuClicked(QAction*)));

    connect(tcpMsgServer, SIGNAL(signalDownloadFile(QJsonValue)), tcpFileServer, SLOT(SltClientDownloadFile(QJsonValue)));
}


/**
 * @brief MainWindow::SltTrayIcoClicked
 * 托盘菜单
 * @param reason
 */
void PictureEdit::SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    {
    }
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        if (!this->isVisible())
            this->show();
    }
        break;
    default:
        break;
    }
}

void PictureEdit::SltTrayIconMenuClicked(QAction *action)
{
    if ("退出" == action->text()) {
        tcpMsgServer->CloseListen();
        tcpFileServer->CloseListen();
        qApp->quit();
    }
    else if ("显示主面板" == action->text()) {
        this->show();
    }
}

/**
 * @brief MainWindow::closeEvent
 * 拦截关闭事件
 * @param event
 */
void PictureEdit::closeEvent(QCloseEvent *event) {
#if 1
    this->hide();
    event->ignore();
#else
    QWidget::closeEvent(event);
#endif
}

void PictureEdit::on_btnListen_clicked()
{
    bool bOk = tcpMsgServer->StartListen(ui->lineEditChatPort->text().toInt());
    if (!bOk)
    {
        ui->labelChatState->setText("监听失败,换个端口试试！");
        return;
    }

    bOk = tcpFileServer->StartListen(ui->lineEditFilePort->text().toInt());
    if (!bOk)
    {
        ui->labelFileState->setText("监听失败,换个端口试试！");
        return;
    }

    ui->btnListen->setText("监听成功");
}

void PictureEdit::on_btnWinClose_clicked()
{
    this->hide();
    qApp->quit();
}

void PictureEdit::on_comboBoxAddress_activated(const QString &arg1)
{

}
