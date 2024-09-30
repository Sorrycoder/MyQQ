#include "picturecutdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QCursor>
#include <QResizeEvent>

PictureCutDialog::PictureCutDialog(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout(this);
    imageLabel = new QLabel(this);
    cropButton = new QPushButton("裁剪", this);
    cancelButton = new QPushButton("取消", this);

    layout->addWidget(imageLabel);
    layout->addWidget(cropButton);
    layout->addWidget(cancelButton);

    connect(cropButton, &QPushButton::clicked, this, &PictureCutDialog::acceptCrop);
    connect(cancelButton, &QPushButton::clicked, this, &PictureCutDialog::cancelCrop);

    setFixedSize(300, 300); // 设置对话框大小
}

void PictureCutDialog::setImage(const QPixmap &pixmap)
{
    originalPixmap = pixmap;
    imageLabel->setPixmap(originalPixmap);
}

void PictureCutDialog::acceptCrop()
{
    // 这里是裁剪逻辑，根据需要选择需要裁剪的区域。
    // 这里只是简单演示，直接返回原始图像。
    emit signalCutHeadOk(originalPixmap);
    this->accept();
}

void PictureCutDialog::cancelCrop()
{
    emit signalClose();
    this->reject();
}
