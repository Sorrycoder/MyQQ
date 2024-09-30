#include "mainwindow.h"
#include"loginwidget.h"
#include <QApplication>
#include "databasemagr.h"
#include"global.h"
#include<QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    MyApp::InitApp(a.applicationDirPath());

//    QFile file(":/qss/resource/qss/default.css");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = file.readAll();
//    qDebug()<<styleSheet;
//    qApp->setStyleSheet(styleSheet);
//    file.close();
    myHelper::setStyle("default");
    // 加载数据库
    DataBaseMagr::Instance()->OpenUserDb(MyApp::m_strDatabasePath + "user.db");
    DataBaseMagr::Instance()->OpenMessageDb(MyApp::m_strDatabasePath + "msg.db");


    LoginWidget w;
    myHelper::FormInCenter(&w);
    w.show();

    return a.exec();
}
