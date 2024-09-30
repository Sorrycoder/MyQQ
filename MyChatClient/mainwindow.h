#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"loginwidget.h"
#include"customwidget.h"
#include<QButtonGroup>
#include <QSystemTrayIcon>
#include"clientsocket.h"
#include"qqcell.h"
#include"chatwindow.h"
#include"systemsetting.h"
#include"weathermode.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public CustomMoveWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //设置客户端的Socket和用户的名称
    void SetSocket(ClientSocket *tcpSocket, const QString &name);
private slots:
    // 主界面切换
    void SltMainPageChanged(int index);

    // 用户接受处理
    void SltReadMessages(const QJsonValue &json, const int &id);

    // 头像裁剪ok
    void SltHeadPicCutOk();
    void SltUpdateUserHead(const int &userId, const QString &strHead);




    // 托盘菜单处理

    void SltTrayIcoClicked(QSystemTrayIcon::ActivationReason reason);
    void SltTrayIconMenuClicked(QAction *action);

    // 程序退出处理
    void SltQuitApp();

    // 右键菜单
    void onAddFriendMenuDidSelected(QAction *action);
    void onGroupPopMenuDidSelected(QAction *action);
    void onChildPopMenuDidSelected(QAction *action);

    //tcp套接字要关联的槽函数

    void SltTcpReply(const quint8 &type, const QJsonValue &dataVal);// 解析Socket的消息
    void SltTcpStatus(const quint8 &state);


    void on_btnWinClose_clicked();

    //分类型解析消息
    void ParseAddFriendReply(const QJsonValue &dataVal);
    void ParseUpFriendHead(const QJsonValue &dataVal);

    void ParseAddGroupReply(const QJsonValue &dataVal);
    void ParseCreateGroupReply(const QJsonValue &dataVal);


    // 获取
    void ParseGetFriendsReply(const QJsonValue &dataVal);
    void ParseGetGroupFriendsReply(const QJsonValue &dataVal);
    // 刷新
    void ParseRefreshFriendsReply(const QJsonValue &dataVal);
    void ParseRefreshGroupFriendsReply(const QJsonValue &dataVal);

    // 头像图片的获取
    QString GetHeadPixmap(const QString &name) const;
    void DownloadFriendHead(const int &userId, const QString &strHead);

    // 好友添加请求确认
    void ParseAddFriendRequest(const QJsonValue &dataVal);
    void ParseAddGroupRequest(const QJsonValue &dataVal);

    // 好友列表点击
    void SltFriendsClicked(QQCell *cell);
    // 群组列表点击
    void SltGroupsClicked(QQCell *cell);
    //系统菜单点击
    void SltSysmenuCliecked(QAction *action);



    // 消息窗口管理，关闭窗口
    void SltFriendChatWindowClose();
    void SltGroupChatWindowClose();

    // 消息处理
    void ParseFriendMessageReply(const QJsonValue &dataVal);
    void ParseFaceMessageReply(const QJsonValue &dataVal);
    void ParseGroupMessageReply(const QJsonValue &dataVal);

    void AddMyGroups(const QJsonValue &dataVal);
    void UpdateFriendStatus(const quint8 &nStatus, const QJsonValue &dataVal);



    void on_tbtnWeather_clicked();

private:
    Ui::MainWindow *ui;

    // 按钮切换界面
    QButtonGroup *m_btnGroup;

    // 聊天窗管理
    QList<ChatWindow *> m_chatFriendWindows;//维持好友聊天窗口
    QList<ChatWindow *> m_chatGroupWindows;//维持群聊聊天窗口

    // socket通信类
    ClientSocket    *m_tcpSocket;
    QSystemTrayIcon *systemTrayIcon;

    WeatherMode *m_weather;//天气界面
    bool m_isWeatherShow=false;





    // 主动退出操作时不进行断线匹配
    bool m_bQuit;

private:

    void InitQQListMenu();//初始化好友菜单
    void InitSysTrayIcon();//初始化系统托盘
    void InitSysMenu();
/*
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    bool is_mousepress;//鼠标是否按下
    QPoint m_offset;
*/
};
#endif // MAINWINDOW_H
