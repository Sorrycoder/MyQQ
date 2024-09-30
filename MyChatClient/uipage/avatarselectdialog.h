// avatarselectdialog.h
#ifndef AVATARSELECTDIALOG_H
#define AVATARSELECTDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class AvatarSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AvatarSelectDialog(QWidget *parent = nullptr);
    void setAvatars(const QList<QPixmap> &avatars);

signals:
    void avatarSelected(const QPixmap &pixmap);

private slots:
    void onAvatarClicked(const QPixmap &pixmap);

private:
    QGridLayout *layout;
};

#endif // AVATARSELECTDIALOG_H
