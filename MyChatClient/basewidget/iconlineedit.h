#ifndef ICONLINEEDIT_H
#define ICONLINEEDIT_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include<QPixmap>
class IconLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IconLineEdit(QWidget *parent=0);
    ~IconLineEdit();
    void setIcon(const QPixmap &pixmap);




private:



    QLabel *labelPixmap;






};

#endif // ICONLINEEDIT_H
