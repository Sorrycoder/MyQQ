#ifndef IPLINEEIDT_H
#define IPLINEEIDT_H

#include <QLineEdit>
#include <QWidget>
#define IP_INPUT_SIZE 4
class IPLineEidt : public QLineEdit
{
    Q_OBJECT
public:
    IPLineEidt(QWidget *parent=0);

    ~IPLineEidt();

    QString text();
private:

    QLineEdit *m_lineEdit[IP_INPUT_SIZE];

    int getIndex(QLineEdit *pLineEdit);

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // IPLINEEIDT_H
