#include "widget.h"
#include "ms_web_client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    ms_web_client cli1;
//    cli1.func_register_back = [=](long long account,string passwd,bool ok){
//        vlog<<account<<"|"<<passwd<<"|"<<ok<<endl;
//    };
//    cli1.func_login_back = [=](bool ok){
//        vlog<<ok<<endl;
//    };
//    cli1.open_client();




//    while(true){}


    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
