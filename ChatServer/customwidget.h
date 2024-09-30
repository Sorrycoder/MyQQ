#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QMutex>
#include <QTimer>

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
};

/////////////////////////////////////////////////////////
/// \brief The CustomMoveWidget class
/// 可移动窗口
class CustomMoveWidget : public CustomWidget {
    Q_OBJECT
public:
    explicit CustomMoveWidget(QWidget *parent = 0);
    ~CustomMoveWidget();

protected:
    QPoint mousePoint;
    bool m_mousePressed;

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
};

#endif // CUSTOMWIDGET_H
