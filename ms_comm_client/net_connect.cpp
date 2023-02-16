#include "net_connect.h"

net_connect::net_connect(QObject *parent) : QObject(parent)
{
    ms.func_open = [=](){ emit fa_open(); };

    ms.func_close = [=](){ emit fa_close(); is_login = false; }; //状态记录--未连接

    ms.func_register_back = [=](long long account,string passwd,bool ok)
    { emit fa_register_back(account,passwd,ok); };

    ms.func_login_back = [=](long long account,bool ok)
    { emit fa_login_back(account,ok); if(ok){ is_login = true; } }; //状态记录--以连接

    ms.func_logout_back = [=](long long account,bool ok)
    { if(ok){ ms.close_client(); } emit fa_logout_back(account,ok); };

    ms.func_recover_passwd_back = [=](long long account,string passwd,bool ok)
    { emit fa_recover_passwd_back(account,passwd,ok); };

    ms.func_swap_txt = [=](long long account_from,string txt)
    { vlogf(vv(account_from) vv(txt)); emit fa_swap_txt(account_from,txt); };

    ms.func_swap_file_finish = [=](long long account_from,string filename,en_build_file type,bool is_ok)
    { emit fa_swap_file_finish(account_from,filename,type,is_ok); };

    ms.func_swap_file_ret = [=](long long account_from,string filename,en_build_file type,bool is_ok)
    { emit fa_swap_file_ret(account_from,filename,type,is_ok); };

    ms.func_swap_error = [=](long long account_from,en_swap_error err)
    { emit fa_swap_error(account_from,err); };

    ms.func_prog_send = [=](string filename,long long account_from,int now)
    { emit fa_prog_send(filename,account_from,now); };

    ms.func_prog_recv = [=](string filename,long long account_from,int now)
    { emit fa_prog_recv(filename,account_from,now); };
}

int net_connect::open_connect(string ip ,int port,string txt)
{ return ms.open(ip,port,txt); }

bool net_connect::get_status()
{ return is_login; }

void net_connect::close_connect()
{ ms.close_client(); }

void net_connect::set_file_path(std::string path)
{ ms.set_file_path(path); }

void net_connect::ask_register(std::string passwd)
{ ms.ask_register(passwd); }

void net_connect::ask_login(long long account,string passwd)
{ if(is_login) {return;} v_account=account; v_passwd=passwd; ms.ask_login(account,passwd); }

void net_connect::ask_logout()
{ ms.ask_logout(v_account); }

void net_connect::ask_recover_passwd(long long account)
{ ms.ask_recover_passwd(account); }

void net_connect::ask_swap_txt(long long account_to, std::string txt)
{ ms.ask_swap_txt(v_account,account_to,txt); }

void net_connect::ask_swap_file
    (long long account_to, std::string filename, std::string file_path, en_build_file type)
{ ms. ask_swap_file(v_account,account_to,filename,file_path,type); }

long long net_connect::get_account()
{ return v_account; }

std::string net_connect::get_passwd()
{ return v_passwd; }




