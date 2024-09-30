#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include <QLabel>
#include <QWidget>


//头像剪切窗口
class CutDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CutDialog(QWidget *parent = nullptr);
    ~CutDialog();

private:

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;


private:
private:
    QPoint m_startpoint;
    QPoint m_old_pos;
    bool m_isMoving;
    bool m_mouse_down;
    bool m_left;
    bool m_right;
    bool m_top;
    bool m_bottom;
    QLabel *label;

};

#endif // CUTDIALOG_H
