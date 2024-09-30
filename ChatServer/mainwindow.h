/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : mainwindow.h --- MainWindow
 作 者    :
 编写日期 : 2017
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>

*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "customwidget.h"

#include <QButtonGroup>
#include <QSystemTrayIcon>
#include <QStandardItemModel>

class TcpMsgServer;
class TcpFileServer;

namespace Ui {
class MainWindow;
}

class PictureEdit : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit PictureEdit(QWidget *parent = 0);
    ~PictureEdit();

protected:
    void changeEvent(QEvent *e);

private slots:

    // 系统菜单
    void SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void SltTrayIconMenuClicked(QAction *action);

    void on_btnListen_clicked();

    void on_btnWinClose_clicked();

    void on_comboBoxAddress_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;

    // 服务配置
    TcpMsgServer *tcpMsgServer;
    TcpFileServer *tcpFileServer;

    // 系统菜单
    QSystemTrayIcon *systemTrayIcon;
private:
    void InitNetwork();
    void SetUserIdentity(const int &identity);
protected:
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
