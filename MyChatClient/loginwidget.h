#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include"clientsocket.h"
#include"global.h"
#include"customwidget.h"
#include"animationstackedwidget.h"
#include"iplineeidt.h"
#include<QCompleter>
namespace Ui {
class LoginWidget;
}

class LoginWidget : public CustomMoveWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void on_btnWinMenu_clicked();

    void on_btnCancel_clicked();

    void on_btnLogin_clicked();


    //void onSignalMessage(const quint8 &type, const QJsonValue &dataVal);
    //void onSignalStatus(const quint8 &state);
    // 服务器信息返回处理
    void SltTcpStatus(const quint8 &state);


    void SltEditFinished();

    void on_btnRegister_clicked();

    void on_btnSaveFCfg_clicked();

private:
    Ui::LoginWidget *ui;

    ClientSocket *m_tcpSocket;

    bool m_bConnected;

    void InitWidget();
/*
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    bool is_mousepress;//鼠标是否按下
    QPoint m_offset;
*/
signals:
    void signalLoginSuccess( QString username, QString password);

};

#endif // LOGINWIDGET_H
