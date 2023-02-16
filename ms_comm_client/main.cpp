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
//    if(argc < 2) { cout<<"argc < 2"<<endl; return -1;}

//    bool is_open = true;
//    stringstream ss;
//    long long account;

//    ss<<argv[1];
//    ss>>account;
//    ss.clear();

//    if(account == 1)
//    {
//        ms_web_client ms;
//        ms.func_open = [&](){
//            ms.ask_login(1045809651,"123qwe");
//        };
//        ms.func_close = [&](){
//            is_open = false;
//            vlogf("func_close");
//        };
//        ms.func_login_back = [](long long account,bool ok){
//            vlogf("func_login_back:" vv(account) vv(ok));
//        };
//        ms.func_swap_txt = [](long long account,string txt){
//            vlogf("func_swap_txt:" vv(account) vv(txt));
//        };
//        ms.func_swap_file_finish = [](long long account,string filename,en_build_file type,bool is_ok){
//            vlogd("func_swap_file_finish:" vv(account) vv(filename) vv(type) vv(is_ok) );
//        };

//        ms.open();
//        while(is_open){}

//        return 0;
//    }
//    else if(account == 2)
//    {
//        ms_web_client ms;
//        ms.func_open = [&](){
//            ms.ask_login(1934865572,"123qwe");
//        };
//        ms.func_close = [&](){
//            is_open = false;
//            vlogf("func_close");
//        };
//        ms.func_login_back = [](long long account,bool ok){
//            vlogf("func_login_back:" vv(account) vv(ok));
//        };
//        ms.func_swap_txt = [](long long account,string txt){
//            vlogf("func_swap_txt:" vv(account) vv(txt));
//        };
//        ms.func_swap_file_ret = [](long long account,string filename,bool is_ok){
//            vlogf("func_swap_file_ret:" vv(account) vv(filename) vv(is_ok) );
//        };

//        ms.open();
//        while(is_open){
//            int po;
//            cout<<"input number:"<<endl;
//            cin>>po;
//            ms.ask_swap_file(1934865572,1045809651,"123tt.tar.gz","/home/red/open/tools/123.tar.gz",en_build_file::e_spic);
//        }

//        return 0;
//    }

    return 0;
}
