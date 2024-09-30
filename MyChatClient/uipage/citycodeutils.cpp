#include "citycodeutils.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include<QDebug>
CityCodeUtils::CityCodeUtils() {}

QString CityCodeUtils::getCityCodeFromName(QString name)//点击后先判断是否空的，空的就填充map
{

    if(cityMap.isEmpty()){//map是空的，打开，吃实话
        initCityMap();
    }
    QMap<QString,QString>::iterator it=cityMap.find(name);
    if(it==cityMap.end()){//如果找不到
        it=cityMap.find(name+"市");//加个市再找
        if(it==cityMap.end()){//还找不到
            it=cityMap.find(name+"县");
        }if(it==cityMap.end()){//价格县
            it=cityMap.find(name+"区");
        }if(it==cityMap.end()){//全找不到
            return "";
        }
    }
    return it.value();


}

void CityCodeUtils::initCityMap()//初始化map
{
    //打开json文件，保存到QByteArray里
    QFile file(":/resource/citycode.json");
    file.open(QIODevice::ReadOnly);
    QByteArray rawData=file.readAll();
    file.close();
    //qDebug()<<"rawData"<<rawData;
    //转成QJsonDocument
    QJsonDocument jsonDoc=QJsonDocument::fromJson(rawData);
    if(jsonDoc.isArray()){//如果是数组
        QJsonArray citys=jsonDoc.array();//转数组
        for(QJsonValue value:citys){//轮询
            if(value.isObject()){//插入map里
                QString cityname=value["city_name"].toString();
                QString citycode=value["city_code"].toString();
                cityMap.insert(cityname,citycode);
            }
        }

    }
}
