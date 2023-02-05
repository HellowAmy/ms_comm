#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    edit = new QLineEdit(this);
    edit->resize(300,30);
    edit->move(300,0);
    edit->setText("5511780295");
    edit->show();

    edit_f = new QLineEdit(this);
    edit_f->resize(300,30);
    edit_f->move(300,100);
    edit_f->setText("/home/red/test/qq.txt");
    edit_f->show();

    edit_account = new QLineEdit(this);
    edit_account->resize(300,30);
    edit_account->move(300,50);//4093996320
    edit_account->show();

    edit_in = new QLineEdit(this);
    edit_in->resize(300,30);
    edit_in->move(300,100);
    edit_in->setText("4093996320");
    edit_in->show();


    butt = new QPushButton(this);
    butt->setText("ok");
    butt->show();

    butt_f = new QPushButton(this);
    butt_f->move(0,40);
    butt_f->setText("ok_f");
    butt_f->show();

    butt_in = new QPushButton(this);
    butt_in->move(300-100,100);
    butt_in->setText("ok_in");
    butt_in->show();

    lab = new QLabel(this);
    lab->resize(200,30);
    lab->setFrameShape(QFrame::Box);
    lab->move(100,200);
    lab->show();

    lab_t = new QLabel(this);
    lab_t->resize(200,30);
    lab_t->setFrameShape(QFrame::Box);
    lab_t->move(100,200+50);
    lab_t->setText("QLabel");
    lab_t->show();

//    net = new net_connect;
//    connect(net,&net_connect::fa_login_status,this,[=](bool ok){
//        lab->setText("login: "+QString::number(ok));
//    });
//    connect(net,&net_connect::fa_swap_txt,this,[=](long long from,QString txt){
//        lab_t->setText("swap_txt: "+QString::number(from)+"|"+txt);
//    });
//    connect(net,&net_connect::fa_swap_files,this,[=](long long from,QString filename){
//        lab_t->setText("swap_txt: "+QString::number(from)+"|"+filename);
//    });

//    connect(butt,&QPushButton::clicked,this,[=](){
//        net->send_txt(edit->text().toLongLong(),this->windowTitle().toStdString());
//    });
//    connect(butt_f,&QPushButton::clicked,this,[=](){
//        butt_f->setText("butt_f");
//        net->send_file(edit->text().toLongLong(),edit_f->text().toStdString());
//    });
//    connect(butt_in,&QPushButton::clicked,this,[=](){
//        open_c(edit_in->text().toLongLong(),"123qwe");
////        butt_f->setText("butt_f");
////        net->send_file(edit->text().toLongLong(),edit_f->text().toStdString());
//    });

}

Widget::~Widget()
{

}

//void Widget::open_c(long long ac, std::string pa)
//{
//    edit_account->setText(QString::number(ac));
//    this->setWindowTitle(QString::number(ac));
//    net->open_client(ac,pa);
//}

