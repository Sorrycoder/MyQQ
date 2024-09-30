#ifndef PHOTOSHOTDIALOG_H
#define PHOTOSHOTDIALOG_H

#include <QWidget>
#include"cutdialog.h"

//头像裁剪活动窗口
class PhotoShotDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoShotDialog(QWidget *parent = nullptr);
    ~PhotoShotDialog();
    //获取头像剪切窗口的矩形
    QRect getCutGeometry();

private:
    CutDialog *m_cutDialog;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // PHOTOSHOTDIALOG_H
