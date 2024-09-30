#ifndef COLORCHOOSE_H
#define COLORCHOOSE_H

#include <QDialog>

namespace Ui {
class ColorChoose;
}

class ColorChoose : public QDialog
{
    Q_OBJECT

public:
    explicit ColorChoose(QWidget *parent = nullptr);
    ~ColorChoose();

signals:

    void signalBackgroundColor(QString);
    void signalLeftColor(QString);
    void signalRightColor(QString);


private slots:


    void on_btnleftMsgColor_clicked();

    void on_btnRightMsgColor_clicked();

    void on_btnBackColor_clicked();

private:
    Ui::ColorChoose *ui;
};

#endif // COLORCHOOSE_H
