﻿#include "widget.h"
#include <QApplication>

#include "ms_web_client.h"

#include <iostream>
#include <fstream>
#include <map>
using namespace std;

map<string,fstream> map_o;


int main(int argc, char *argv[])
{
    if(argc < 2) { cout<<"argc < 2"<<endl; return -1;}

    bool is_open = true;
    stringstream ss;
    long long account;

    ss<<argv[1];
    ss>>account;
    ss.clear();

    if(account == 1)
    {
        ms_web_client ms;
        ms.func_open = [&](){
            cout<<"ask_login:"<<1045809651<<endl;
            ms.ask_login(1045809651,"123qwe");
        };
        ms.func_close = [&](){
            is_open = false;
            cout<<"func_close"<<endl;
        };
        ms.func_login_back = [](long long account,bool ok){
            cout<<"func_login_back:"<<account<<"|"<<ok<<endl;
        };
        ms.func_swap_txt = [](long long account,string txt){
            cout<<"func_swap_txt:"<<account<<"|"<<txt<<endl;
        };
        ms.func_swap_file = [](long long account,string filename){
            cout<<"func_swap_file:"<<account<<"|"<<filename<<endl;
        };

        ms.open();
        while(is_open){}

        return 0;
    }
    else if(account == 2)
    {
        ms_web_client ms;
        ms.func_open = [&](){
            cout<<"ask_login:"<<1934865572<<endl;
            ms.ask_login(1934865572,"123qwe");
        };
        ms.func_close = [&](){
            is_open = false;
            cout<<"func_close"<<endl;
        };
        ms.func_login_back = [](long long account,bool ok){
            cout<<"func_login_back:"<<account<<"|"<<ok<<endl;
        };
        ms.func_swap_txt = [](long long account,string txt){
            cout<<"func_swap_txt:"<<account<<"|"<<txt<<endl;
        };
        ms.func_swap_file = [](long long account,string filename){
            cout<<"func_swap_file:"<<account<<"|"<<filename<<endl;
        };

        ms.open();
        while(is_open){
            int po;
            cout<<"input number"<<endl;
            cin>>po;
            ms.ask_swap_file(1934865572,1045809651,"123.txt");
        }

        return 0;
    }

    return 0;
}
