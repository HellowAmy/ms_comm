#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>



//#include "net_connect.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

//    void open_c(long long ac,string pa);

    QPushButton *butt;
    QPushButton *butt_f;
    QPushButton *butt_in;
    QLabel *lab;
    QLabel *lab_t;
    QLineEdit *edit;
    QLineEdit *edit_f;
    QLineEdit *edit_account;
    QLineEdit *edit_in;
//    net_connect *net;
};
#endif // WIDGET_H
