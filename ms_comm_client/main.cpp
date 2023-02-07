#include "widget.h"
#include <QApplication>

#include "ms_web_client.h"

#include <iostream>
#include <fstream>
#include <map>
using namespace std;

map<string,fstream> map_o;


int main(int argc, char *argv[])
{
    if(argc < 3) { cout<<"argc < 2"<<endl; return -1;}

    stringstream ss;
    long long account;
    string passwd;

    ss<<argv[1];
    ss>>account;
    ss.clear();

    ss<<argv[2];
    ss>>passwd;
    ss.clear();

    ms_web_client ms;
    ms.func_open = [&](){
        cout<<"ask_login:"<<account<<"|"<<passwd<<endl;
        ms.ask_login(account,passwd);
    };
    ms.func_login_back = [](long long account,bool ok){
        cout<<"func_login_back:"<<account<<"|"<<ok<<endl;
    };

    ms.open();
    while(1){}

    return 0;

//    it->second.
//    it->second.

//    cout<<"asda"<<endl;
//    tools::stm("\\")()
//    string filename = "D:\\test\\11\\11.PNG";
//    string temp = tools::stm(filename)("\\",-1,-1);
//    std::cout<<temp<<endl;
//    return 0;

    //    3359116136123qwe
    //    4093996320123qwe
    //    5511780295123qwe
//    QApplication a(argc, argv);

//#if 0
//    Widget w;
//    w.open_c(3359116136,"123qwe");
//    w.show();
//#else
//    Widget w1;
////    w1.open_c(4093996320,"123qwe");
//    w1.show();
//#endif
////    Widget w2;
////    w2.open_c(5511780295,"123qwe");
////    w2.show();

//    return a.exec();

//            return 0;
}
