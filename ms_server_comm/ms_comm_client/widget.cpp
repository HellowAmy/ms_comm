#include "widget.h"

#include "net_connect.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    net_connect *n1 = new net_connect(this);





    net_connect *n2 = new net_connect(this);



    //    bool hok = false;
        connect(n1,&net_connect::fa_login_status,this,[=](bool ok){
    //        hok = ok;
            cout<<"cout::fa_login_status"<<endl;
            vlog<<"n1 :fa_login_status"<<ok<<endl;
        },Qt::DirectConnection);

        connect(n2,&net_connect::fa_login_status,this,[=](bool ok){
    //        hok = ok;
            cout<<"cout::fa_login_status"<<endl;
            vlog<<"n2 :fa_login_status"<<ok<<endl;
        },Qt::DirectConnection);

        connect(n1,&net_connect::fa_swap_txt,this,[=](long long from,string txt){
            vlog<<"po=====222==========kko: "<<from<<" | "<<txt<<endl;
        },Qt::QueuedConnection);
        connect(n1,&net_connect::fa_swap_txt,this,[=](long long from,string txt){
            vlog<<"po=====111==========kko: "<<from<<" | "<<txt<<endl;
        },Qt::DirectConnection);



        connect(n2,&net_connect::fa_login_status,this,[=](bool ok){


            if(ok)
            {
                vlog<<"fa_login_status: send_txt"<<endl;
//                sleep(1);
                n2->send_txt(5511780295,"-------hhhays87318hf72gbasyeufg8732v23f7623ufbsf3uw38fwh37rpppp");

                sleep(1);
                n2->send_txt(5511780295,"-------asd21");
                sleep(1);
                n2->send_txt(5511780295,"-------sd3fsef");
                sleep(1);
                n2->send_txt(5511780295,"------rrrasdjhawggggpppaloooo");
            }
            else vlog<<"hok false"<<endl;

//            connect(n1,&net_connect::fa_swap_txt,this,[=](long long from,string txt){
//                vlog<<"po===============kko: "<<from<<" | "<<txt<<endl;
//            },Qt::DirectConnection);

        },Qt::DirectConnection);



        n1->open(5511780295,"123qwe");
        n1->init();

              n2->open(4093996320,"123qwe");

             n2->init();

            sleep(1);



//            connect(n1,&net_connect::fa_swap_txt,this,[=](long long from,string txt){
//                vlog<<"po===============kko: "<<from<<" | "<<txt<<endl;
//            },Qt::DirectConnection);





    while (1) {

    }

//    3359116136123qwe
//    4093996320123qwe
//    5511780295123qwe

}

Widget::~Widget()
{
}

