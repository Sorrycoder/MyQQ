#include "weathermode.h"
#include "ui_weathermode.h"

WeatherMode::WeatherMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherMode)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);//无边框
    //this->setFixedSize(width(),height());//固定大小


    //右键菜单
    m_menu=new QMenu(this);
    m_exit=new QAction();
    m_exit->setText("退出");
    m_exit->setIcon(QIcon(":/res/close.png"));
    m_menu->addAction(m_exit);

    connect(m_exit,&QAction::triggered,[=](){
        this->close();
    });//101010100
    //由manager发起get请求
    manager=new QNetworkAccessManager(this);
    //strurl="http://v1.yiketianqi.com/api?unescape=1&version=v61&appid=67985825&appsecret=ObPT1i2e";
    strurl="http://v1.yiketianqi.com/api?unescape=1&version=v9&appid=67985825&appsecret=ObPT1i2e";
    QUrl urltianqi(strurl);
    QNetworkRequest res(urltianqi);
    reply=manager->get(res);
    //请求后进行读取
    connect(manager,&QNetworkAccessManager::finished,this,&WeatherMode::readHttpReply);



    mDateList<<ui->lblDay0<<ui->lblDay1<<ui->lblDay2<<ui->lblDay3<<ui->lblDay4<<ui->lblDay5;
    mWeekList<<ui->lblWeek0<<ui->lblWeek1<<ui->lblWeek2<<ui->lblWeek3<<ui->lblWeek4<<ui->lblWeek5;
    mIconList<<ui->lblTypeIcon0<<ui->lblTypeIcon1<<ui->lblTypeIcon2<<ui->lblTypeIcon3<<ui->lblTypeIcon4<<ui->lblTypeIcon5;
    mTypeList<<ui->lblType0<<ui->lblType1<<ui->lblType2<<ui->lblType3<<ui->lblType4<<ui->lblType5;
    mAirList<<ui->lblRank0<<ui->lblRank1<<ui->lblRank2<<ui->lblRank3<<ui->lblRank4<<ui->lblRank5;
    mFXList<<ui->lblwind0<<ui->lblwind1<<ui->lblwind2<<ui->lblwind3<<ui->lblwind4<<ui->lblwind5;
    mFLList<<ui->lblwindNum0<<ui->lblwindNum1<<ui->lblwindNum2<<ui->lblwindNum3<<ui->lblwindNum4<<ui->lblwindNum5;



    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu.png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮尘",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("无",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");


    ui->widget_0404->installEventFilter(this);
    ui->widget_0405->installEventFilter(this);
}

WeatherMode::~WeatherMode()
{
    delete ui;
}
//重写右键菜单
void WeatherMode::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu->exec(QCursor::pos());//在鼠标右键单机位置弹出
    event->accept();

}

void WeatherMode::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-m_offset);

}
void WeatherMode::mousePressEvent(QMouseEvent *event)
{
    QPoint globalPos=event->globalPos();//鼠标在全局坐标
    QPoint windowPos=this->pos();//窗口位置
    m_offset=globalPos-windowPos;//相对位移

}


bool WeatherMode::eventFilter(QObject *watched, QEvent *event)
{

        if(watched==ui->widget_0404&&event->type()==QEvent::Paint){//如果监听到0404窗口，并且是绘画事件
            drawTempHighLine();
        }
        if(watched==ui->widget_0405&&event->type()==QEvent::Paint){//如果监听到0404窗口，并且是绘画事件
            drawTempLowLine();
        }
         return QWidget::eventFilter(watched, event);//返回事件，不能截断掉事件

}

void WeatherMode::parseWeatherJsonData7(QByteArray rawData)
{
    QJsonDocument jsonDoc=QJsonDocument::fromJson(rawData);//
    if(!jsonDoc.isNull()&&jsonDoc.isObject())//最外围是个对象
    {
        QJsonObject jsonRoot=jsonDoc.object();//转为对象
        days[0].mCity=jsonRoot["city"].toString();
        days[0].mPM25=jsonRoot["aqi"].toObject()["pm25"].toString();
        if(jsonRoot.contains("data")&&jsonRoot["data"].isArray()){//找到data项并判断是个数组
            QJsonArray weaArray= jsonRoot["data"].toArray();//转数组
            for(int i=0;i<weaArray.size();i++){//遍历data每一项
                QJsonObject obj= weaArray[i].toObject();//每项是一个对象


                days[i].mDate=obj["date"].toString();
                days[i].mWeek=obj["week"].toString();
                days[i].mWeathType=obj["wea"].toString();
                days[i].mTemp=obj["tem"].toString();
                days[i].mTempLow=obj["tem2"].toString();
                days[i].mTempHigh=obj["tem1"].toString();
                days[i].mFX=obj["win"].toArray()[0].toString();
                days[i].mFL=obj["win_speed"].toString();
                days[i].AirQu=obj["air_level"].toString();
                days[i].mTips=obj["air_tips"].toString();
                days[i].HU=obj["humidity"].toString();





            }
        }
    }
updateUI();
}

void WeatherMode::updateUI()
{
    QPixmap pixmap;
    //解析日期
    ui->label_Date->setText(days[0].mDate+" "+days[0].mWeek);
    //解析城市名称
    ui->label_city->setText(days[0].mCity+"市");
    //解析温度
    ui->label_wendu->setText(days[0].mTemp+"℃");
    //解析天气
    ui->label_tianqi->setText(days[0].mWeathType+" "+days[0].mTempLow+"℃~"+days[0].mTempHigh+"℃");
    //根据map里key找图标，key即为wea的类型
    ui->label_tianqiicon->setPixmap(mTypeMap[days[0].mWeathType]);
    //tips
    ui->label_tips->setText(days[0].mTips);
    //风向
    ui->label_win->setText(days[0].mFX);
    ui->lblWinNum->setText(days[0].mFL);
    //PM2.5
    ui->lblPM25->setText(days[0].mPM25);
    //湿度
    ui->label_shidu->setText(days[0].HU);
    //空气质量
    ui->label_air->setText(days[0].AirQu);

    ui->lblType0->setText(days[0].mWeathType);

    for(int i=0;i<6;i++){

        mWeekList[i]->setText(days[i].mWeek);//周一周二。。
        mWeekList[0]->setText("今天");
        mWeekList[1]->setText("明天");
        mWeekList[2]->setText("后天");

        //2024-09-03 分割
        QStringList dayList=days[i].mDate.split("-");
        mDateList[i]->setText(dayList[1]+"/"+dayList[2]);//

        //资源里晴转多云，想让label显示左半部分文字图标
        //找到转的位置
        int index=days[i].mWeathType.indexOf("转");
        if(index!=-1){//有转
            pixmap=mTypeMap[days[i].mWeathType.left(index)];
        }else{
            pixmap=mTypeMap[days[i].mWeathType];
        }

        QString iconpath=days[i].mWeathType;//

        mTypeList[i]->setText(days[i].mWeathType);
        mIconList[i]->setMaximumHeight(40);

        pixmap.scaled(mIconList[i]->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        mIconList[i]->setPixmap(pixmap);

        QString airQ=days[i].AirQu;
        mAirList[i]->setText(airQ);
        if(airQ=="优"){
            mAirList[i]->setStyleSheet("background-color:rgb(150, 213, 32);border-radius: 7px");
        }
        if(airQ=="良"){
            mAirList[i]->setStyleSheet("background-color:rgb(241, 224, 103);border-radius: 7px");
        }
        if(airQ=="轻度污染"){
            mAirList[i]->setStyleSheet("background-color: rgb(203, 107, 39);border-radius: 7px");
        }
        if(airQ=="中度污染"){
            mAirList[i]->setStyleSheet("background-color:rgb(255, 17, 17);border-radius: 7px");
        }
        if(airQ=="重度污染"){
            mAirList[i]->setStyleSheet("background-color:rgb(153, 0, 0);border-radius: 7px");
        }


        //风相
        mFXList[i]->setText(days[i].mFX);
        //风力，去掉转
        QString fl;
        index=days[i].mFL.indexOf("转");
        if(index!=-1){
            fl=days[i].mFL.left(index);
        }else{
            fl=days[i].mFL;
        }
        mFLList[i]->setText(fl);


    }
    update();


}

void WeatherMode::drawTempHighLine()
{
    QPainter painter(ui->widget_0404);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::yellow);//实心
    painter.setPen(Qt::yellow);
    int ave;//平均温度
    int sum=0;
    int offSet=0;//偏移值
    //平均温度
    int middle=ui->widget_0404->height()/2;//窗口的纵位置，y用
    for(int i=0;i<6;i++){
        sum+=days[i].mTempHigh.toInt();
    }
    ave=sum/6;


    //定义6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAirList[i]->x()+mAirList[i]->width()/2);//6个点x坐标对准6个label中间
        offSet=(days[i].mTempHigh.toInt()-ave)*3;
        //qDebug()<<"offset:"<<offSet;

        points[i].setY(middle-offSet);//y在平均温度上下偏移
        //画出6个点
        painter.drawEllipse(points[i],3,3);
        //画出温度
        painter.drawText(points[i].x()-15,points[i].y()-15,days[i].mTempHigh+"℃");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }



}

void WeatherMode::drawTempLowLine()
{
    QPainter painter(ui->widget_0405);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(70, 195, 203));//实心
    painter.setPen(QColor(70, 195, 203));
    int ave;//平均温度
    int sum=0;
    int offSet=0;//偏移值
    //平均温度
    int middle=ui->widget_0405->height()/2;//窗口的纵位置，y用
    for(int i=0;i<6;i++){
        sum+=days[i].mTempHigh.toInt();
    }
    ave=sum/6;


    //定义6个点
    QPoint points[6];
    for(int i=0;i<6;i++){
        points[i].setX(mAirList[i]->x()+mAirList[i]->width()/2);//6个点x坐标对准6个label中间
        offSet=(days[i].mTempLow.toInt()-ave)*3;
        //qDebug()<<"offset:"<<offSet;

        points[i].setY(middle-offSet);//y在平均温度上下偏移
        //画出6个点
        painter.drawEllipse(points[i],3,3);
        //画出温度
        painter.drawText(points[i].x()-15,points[i].y()-15,days[i].mTempLow+"℃");
    }
    for(int i=0;i<5;i++){
        painter.drawLine(points[i],points[i+1]);
    }
}




void WeatherMode::readHttpReply(QNetworkReply *reply)
{

    int status=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//获取响应码
    if(reply->error()==QNetworkReply::NoError&&status==200){
        QByteArray data= reply->readAll();
        //parseWeatherJsonData(data);
        parseWeatherJsonData7(data);
        //qDebug()<<QString::fromUtf8(data);
    }else{//出错了
        qDebug()<<reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
    }

}

void WeatherMode::on_pushButton_Search_clicked()//通过输入城市名查询更新界面
{
    QString cityname=ui->lineEdit_City->text();//
    QString cityCode=citycodeutils.getCityCodeFromName(cityname);//通过城市名获取城市编码
    qDebug()<<"cityCode:"<<cityCode;
    if(cityCode!= ""){
        strurl+="&cityid="+cityCode;//更新查询的网址

        manager->get(QNetworkRequest(QUrl(strurl)));//manager执行一次get查询
    }else{
        QMessageBox::warning(this,"城市","没有找到这个城市",QMessageBox::Ok);
    }

}


void WeatherMode::on_lineEdit_City_returnPressed()//按下enter，
{
    on_pushButton_Search_clicked();
}

