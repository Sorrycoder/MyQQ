#ifndef PICTUREEDIT_H
#define PICTUREEDIT_H

#include <QWidget>
#include"photoshotdialog.h"
#include<QPixmap>
#include"commonapi/myapp.h"
QT_BEGIN_NAMESPACE
namespace Ui { class PictureEdit; }
QT_END_NAMESPACE

class PictureEdit : public QWidget
{
    Q_OBJECT

public:
    PictureEdit(QWidget *parent = nullptr);
    ~PictureEdit();
signals:
    void signalCutHeadOk();
    void signalClose();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_btnLoad_clicked();

private:
    Ui::PictureEdit *ui;

    QPixmap m_srcPix;//原图像

    QPixmap m_scaledPix;//缩小的

    PhotoShotDialog *m_psDialog;//活动窗口

private:
    //加载图片
    //void loadPicture();
};
#endif // PICTUREEDIT_H
