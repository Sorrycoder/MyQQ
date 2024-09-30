#include "colorchoose.h"
#include "ui_colorchoose.h"

#include <QColorDialog>
#include<QDebug>
ColorChoose::ColorChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorChoose)
{
    ui->setupUi(this);
}

ColorChoose::~ColorChoose()
{
    delete ui;
}

void ColorChoose::on_btnBackColor_clicked()
{
    QString strClr = ui->lineEditClrBackground->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("背景色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditClrBackground->setText(color.name().toUpper());
    ui->labelBackground->setStyleSheet(QString("background-color: %1").arg(color.name()));
    emit signalBackgroundColor(color.name());

    //qDebug()<<"strClr"<<strClr;
}

void ColorChoose::on_btnleftMsgColor_clicked()
{
    QString strClr = ui->lineEditLeftBubble->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("左边气泡颜色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditLeftBubble->setText(color.name().toUpper());
    ui->labelLeftBubbleClr->setStyleSheet(QString("background-color: %1").arg(color.name()));
    emit signalLeftColor(color.name());
}

void ColorChoose::on_btnRightMsgColor_clicked()
{
    QString strClr = ui->lineEditRightBubble->text();

    QColor color = QColorDialog::getColor(QColor(strClr), this, tr("右边气泡颜色配置"));
    if (!color.isValid()) {
        return;
    }

    ui->lineEditRightBubble->setText(color.name().toUpper());
    ui->labelRightBubbleClr->setStyleSheet(QString("background-color: %1").arg(color.name()));
    emit signalRightColor(color.name());
}


