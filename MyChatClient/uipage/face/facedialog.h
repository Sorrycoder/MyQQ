#ifndef FACEDIALOG_H
#define FACEDIALOG_H

#include <QDialog>
#include<QList>
#include"emojiitem.h"
#include<QPoint>
QT_BEGIN_NAMESPACE
namespace Ui { class FaceDialog; }
QT_END_NAMESPACE

class FaceDialog : public QDialog
{
    Q_OBJECT

public:
    FaceDialog(QWidget *parent = nullptr);
    ~FaceDialog();

    int selectFaceIndex();
    void setSelectFaceIndex(int index);

    void moveFaceLocation(QPoint p);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FaceDialog *ui;

    QList<EmojiItem*>m_emojiList;

    int m_selectFaceIndex;//当前索引
    int m_preRow;
    int m_preColumn;

    void addEmojiItem(QString fileName);
};
#endif // FACEDIALOG_H
