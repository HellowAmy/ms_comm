#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    edit = new QLineEdit(this);
    edit->resize(300,30);
    edit->move(300,0);
    edit->setText("5511780295");
    edit->show();

    edit_account = new QLineEdit(this);
    edit_account->resize(300,30);
    edit_account->move(300,50);
    edit_account->show();

    butt = new QPushButton(this);
    butt->setText("ok");
    butt->show();

    lab = new QLabel(this);
    lab->resize(100,100);
    lab->move(100,100);
    lab->show();

    lab_t = new QLabel(this);
    lab_t->resize(200,100);
    lab_t->move(200,100);
    lab_t->setText("QLabel");
    lab_t->show();

    net = new net_connect;
    connect(net,&net_connect::fa_login_status,this,[=](bool ok){
        lab->setText("login: "+QString::number(ok));
    });
    connect(net,&net_connect::fa_swap_txt,this,[=](long long from,QString txt){
        lab_t->setText("swap_txt: "+QString::number(from)+"|"+txt);
    });

    connect(butt,&QPushButton::clicked,this,[=](){
        net->send_txt(edit->text().toLongLong(),this->windowTitle().toStdString());
    });

}

Widget::~Widget()
{
}

void Widget::open_c(long long ac, std::string pa)
{
    edit_account->setText(QString::number(ac));
    this->setWindowTitle(QString::number(ac));
    net->open_client(ac,pa);
}

