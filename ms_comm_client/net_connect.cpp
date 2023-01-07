﻿#include "net_connect.h"
#include "lib/vlog.hpp"

net_connect::net_connect(QObject *parent)
{
    vflog::instance()->init(vflog::e_info);
}

int net_connect::open_client
    (long long account, std::string passwd, std::string ip, int port)
{
    v_account = account;
    v_passwd = passwd;

    web_c.func_open = [=](){
        if(web_c.ask_login(v_account,v_passwd) <= 0)
            { vlogf<<"ask_login false"<<endl; }
    };
    web_c.func_login_back = [=]( bool ok){
        vlogf<<"func_login_back: "<<ok<<endl;
        emit fa_login_status(ok);
    };
    web_c.func_swap_txt = [=](long long from,long long to,string txt){
        vlogf<<"func_swap_txt: "<<from<<"|"<<to<<"|"<<txt<<endl;
        emit fa_swap_txt(from,QString::fromStdString(txt));
    };
    web_c.func_swap_file = [=](long long from,long long to,string filename){
        vlogf<<"func_swap_file: "<<from<<"|"<<to<<"|"<<filename<<endl;
        emit fa_swap_files(from,QString::fromStdString(filename));
    };

    return web_c.open(ip,port);
}

bool net_connect::send_txt(long long account_to, std::string txt)
{
    if(web_c.ask_swap_txt(v_account,account_to,txt) <= 0) return true;
    else return false;
}

bool net_connect::send_file(long long account_to, std::string filename)
{

//    if(is_connect) web_c.ask_swap_file(v_account,account_to,filename);
//    else return false;
    return true;
}

//bool net_connect::isf_connect()
//{
////    return is_connect;
//}

//void net_connect::work_thread()
//{
//    vlog<<"work_thread - thread id: "<<std::this_thread::get_id()<<endl;

//    while(is_working)
//    {
//        function<void()> func;
//        {
//            //上锁--避免多线程竞争,函数返回真则启动唤醒
//            std::unique_lock<std::mutex> lock (cv_lock);
//            while(queue_task.empty()) cv_var.wait(lock);

//            func = queue_task.front();
//            queue_task.pop();
//        }
//        func();
//        vlog<<"func"<<endl;
//    }
//}

