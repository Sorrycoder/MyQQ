#ifndef WEATHERMODE_H
#define WEATHERMODE_H

#include "citycodeutils.h"

#include <QWidget>
#include<QContextMenuEvent>
#include<QJsonDocument>
#include<QJsonObject>
#include<QFile>
#include<QMessageBox>
#include<QJsonArray>
#include<QList>
#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QNetworkReply>
#include <QMenu>
#include"day.h"

namespace Ui {
class WeatherMode;
}

class WeatherMode : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherMode(QWidget *parent = nullptr);
    ~WeatherMode();

private:
    Ui::WeatherMode *ui;
    QMenu *m_menu;//右键菜单
    QAction *m_exit;//退出键

    QPoint m_offset;//窗口移动时候，鼠标和左上角偏移

    QNetworkReply *reply;//负责接受回复的变量

    QString strurl;//申请的网址

    QNetworkAccessManager *manager;//网络管理工具

    CityCodeUtils citycodeutils;//城市和代码初始化的工具类

    QMap<QString,QString> mTypeMap;//天气类型和天气图标的映射

public:


    Day days[7];//七天数据
    //更新每一列时
    QList <QLabel *>mDateList;
    QList <QLabel *>mWeekList;
    QList <QLabel*>mIconList;
    QList <QLabel *>mTypeList;
    QList <QLabel *>mAirList;
    QList <QLabel *>mFXList;
    QList <QLabel *>mFLList;



private:
    void readHttpReply(QNetworkReply *reply);//读取请求来的http数据



    void parseWeatherJsonData7(QByteArray rawData);//分析7天的天气情况，解析json数据

    void updateUI();//把解析来的数据更新到ui上

    void drawTempHighLine();//画高温曲线
    void drawTempLowLine();//画低温曲线
    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event);



private slots:
    void on_pushButton_Search_clicked();

    void on_lineEdit_City_returnPressed();
};

#endif // WEATHERMODE_H
