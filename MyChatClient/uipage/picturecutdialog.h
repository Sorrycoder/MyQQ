#ifndef PICTURECUTDIALOG_H
#define PICTURECUTDIALOG_H

#include <QDialog>
#include <QPixmap>

class QVBoxLayout;
class QLabel;
class QPushButton;

class PictureCutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PictureCutDialog(QWidget *parent = nullptr);
    void setImage(const QPixmap &pixmap);

signals:
    void signalClose();
    void signalCutHeadOk(const QPixmap &cutPixmap);

private slots:
    void acceptCrop();
    void cancelCrop();

private:
    QVBoxLayout *layout;
    QLabel *imageLabel;
    QPushButton *cropButton;
    QPushButton *cancelButton;

    QPixmap originalPixmap;
};

#endif // PICTURECUTDIALOG_H
