#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>

class ChatStackedWidget : public QStackedWidget
{
    Q_OBJECT
    //宏定义，qt提供的属性系统，独立于编译器
    //Qt属性基于元数据对象系统
    //用moc进行编译，需要继承Q_OBJECT

    Q_PROPERTY(float RotateVal READ RotateVal WRITE setRotateVal)
public:
    ChatStackedWidget(QWidget *paretn = 0);
    ~ChatStackedWidget();

    //动画效果
    void animation(int pageIndex);

    //设置旋转值
    void setRotateVal(float val)
    {
        iRotateVal=val;
    }
    //返回旋转值
    float RotateVal()
    {
        return iRotateVal;
    }

    //设置m_startVal
    void setStartVal(int val)
    {
        m_startVal=val;
    }
    //返回m_startVal
    int StartVal()
    {
        return m_startVal;
    }

    //设置m_endVal
    void setEndVal(int val)
    {
        m_endVal=val;
    }
    //返回m_endVal
    int EndVal()
    {
        return m_endVal;
    }


private:
    float iRotateVal;//旋转值

    int m_startVal;//起始值
    int m_endVal;//结束值

    int m_nextpageIndex;//下一页

    bool m_isAnimation;

private slots:

    void on_ValueChanged();

    void on_Finished();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // ANIMATIONSTACKEDWIDGET_H
