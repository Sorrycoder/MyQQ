#include "iconlineedit.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

IconLineEdit::IconLineEdit(QWidget *parent)
    :QLineEdit(parent)
{


    labelPixmap = new QLabel(this);
    labelPixmap->setMinimumSize(16, 16);
    labelPixmap->setVisible(false);


}

IconLineEdit::~IconLineEdit()
{

}

void IconLineEdit::setIcon(const QPixmap &pixmap)
{
    if (pixmap.isNull()) return;

    labelPixmap->setPixmap(pixmap);
    labelPixmap->setVisible(true);
    labelPixmap->setGeometry(5,(this->height() - pixmap.height()) / 2, 16, 16);
    this->setTextMargins(25, 1, 1, 1);
}


