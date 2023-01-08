#include "ux_web_server.h"
#include "lib/vlog.hpp"

ux_web_server::ux_web_server()
{
    vflog::instance()->init(vflog::e_info);//日志初始化

    //加载登录账号文件
    file_account = "../data/account.txt";
    ux_manage::load_account(file_account,map_account);

    //===== 添加任务函数到容器 =====
    //账号注册
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_register,bind(&ux_web_server::task_register,this,_1,_2)));
    //登录请求
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_login,bind(&ux_web_server::task_login,this,_1,_2)));
    //错误反馈
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
                    (e_error,bind(&ux_web_server::task_error,this,_1,_2)));
    //数据交换
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_swap_txt,bind(&ux_web_server::task_swap_txt,this,_1,_2)));
    //文件交换
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
                    (e_swap_file,bind(&ux_web_server::task_swap_file,this,_1,_2)));
    //===== 添加任务函数到容器 =====

}

void ux_web_server::on_open(const web_sock &sock, const web_http &http)
{
    vlogf<<"on_open"<<endl;
}

void ux_web_server::on_message(const web_sock &sock, const string &meg)
{
    //执行匹配的任务函数
    enum_transmit cmd = *(enum_transmit*)
            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
    auto it_find = map_func.find(cmd);
    if(it_find != map_func.end())
    {
//        vflog::instance()->close_log(false,false);
//        vlogf<<"on_message:type:"<<cmd<<endl;
//        vflog::instance()->close_log(false,true);
        (std::bind(it_find->second,sock,meg))();//执行任务
//        vflog::instance()->close_log(false,false);
    }
    else vlogw<<"on_message: not find"<<endl;
}

void ux_web_server::on_close(const web_sock &sock)
{
//    vflog::instance()->close_log(false,true);
    unique_lock<mutex> lock(lock_connect);
    for_it(it,map_connect)
    {
        if(it->second != nullptr)
        {
            if(it->second->isConnected() == false)
                { map_connect.erase(it); break; }
        }
        else map_connect.erase(it);
    }
    vlogd<<"on_close:map_connect size: "<<map_connect.size()<<endl;
//    vflog::instance()->close_log(false,false);
}

void ux_web_server::task_register(const web_sock &sock, const std::string &meg)
{
    vlogf<<"task_register:in"<<endl;

    ct_register ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_register>(meg,cmd,ct);

    //add_account: 账号插入到容器，并返回账号
    ct_register_back ct_back;
    unique_lock<mutex> lock(lock_account);
    if(ux_manage::add_account(map_account,ct.passwd,ct_back.account))
    {
        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
        ct_back.flg = ux_manage::save_account(file_account,map_account);
    }
    else ct_back.flg = 0;

    //反馈到客户端
    string str = ux_manage::to_str<enum_transmit,ct_register_back>
            (enum_transmit::e_register_back,ct_back);
    if(send_back(sock,str) <= 0) vlogw<<"send error"<<endl;
}

void ux_web_server::task_login(const web_sock &sock, const std::string &meg)
{
    vlogf<<"task_login:in | isConnected: "<<sock->isConnected()<<endl;
    ct_login ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_login>(meg,cmd,ct);
    vlogf<<"|account: "<<ct.account<<"|passwd: "<<ct.passwd<<endl;

    //添加到连接队列
    ct_login_back ct_back;
    unique_lock<mutex> lock(lock_connect);
    if(ux_manage::add_connect<web_sock>
            (map_connect,ct.account,sock)) { ct_back.flg = 1; }
    else ct_back.flg = 0;

    //反馈到客户端
    string str = ux_manage::to_str<enum_transmit,ct_login_back>
            (enum_transmit::e_login_back,ct_back);
    if(send_back(sock,str) <= 0) vlogw<<"send error"<<endl;

    vlogf<<"task_login size: "<<map_connect.size()<<endl;
}

void ux_web_server::task_error(const web_sock &sock, const std::string &meg)
{

}

void ux_web_server::task_swap_txt(const web_sock &sock, const std::string &meg)
{
    vlogf<<"task_swap_txt"<<endl;
    ct_swap_txt ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);
    vlogf<<"|to: "<<ct.account_to<<"|from: "<<ct.account_from<<"|data: "<<ct.data<<endl;

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end())
        { vlogf<<"it->second: "<<it->second->isConnected()<<endl;
        if(send_back(it->second,meg) <= 0){vlogw<<"send error"<<endl;} }
    else
    {
        vlogf<<"task_swap_txt:not find account"<<endl;
        ct_error err;
        err.account = ct.account_to;
        strncpy(err.error,"error: task_swap_txt",sizeof(err.error));

        //错误反馈
        string str = ux_manage::to_str<enum_transmit,ct_error>
                (enum_transmit::e_error,err);
        if(send_back(sock,str) <= 0) vlogw<<"send error"<<endl;
    }
}

void ux_web_server::task_swap_file(const web_sock &sock, const std::string &meg)
{
    vflog::instance()->close_log(false,false);
    vlogf<<"task_swap_file"<<endl;
    ct_swap_file ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);
    vlogf<<"|to: "<<ct.account_to<<"|from: "<<ct.account_from<<endl;

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end())
        { if(send_back(it->second,meg) <= 0){vlogw<<"send error"<<endl;} }
    else
    {
        vlogf<<"task_swap_file:not find account"<<endl;
        ct_error err;
        err.account = ct.account_to;
        strncpy(err.error,"error: task_swap_file",sizeof(err.error));

        //错误反馈
        string str = ux_manage::to_str<enum_transmit,ct_error>
                (enum_transmit::e_error,err);
        if(send_back(sock,str) <= 0) vlogw<<"send error"<<endl;
    }
    vflog::instance()->close_log(false,true);
}

int ux_web_server::send_back(const web_sock &sock,const std::string &str)
{
    std::unique_lock<mutex> lock(lock_send);
    int ret = 0;
    if(sock->isConnected()) ret = sock->send(str.c_str(),str.size());
    else vlogw<<"isConnected is false"<<endl;
    return ret;
}
