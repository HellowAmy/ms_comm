#include "net_connect.h"

#define qfrs(str) QString::fromStdString(str)
#define qtos(str) str.toStdString()

net_connect::net_connect(QObject *parent) : QObject(parent)
{
    QDir dir(qfrs(ms.get_file_path()));
    if(dir.exists() == false) dir.mkpath(qfrs(ms.get_file_path()));

    //注册Qt的信号槽类型支持
    qRegisterMetaType<en_build_file>("en_build_file");
    qRegisterMetaType<QMap<long long,string>>("QMap<long long,string>");
    qRegisterMetaType<en_swap_error>("en_swap_error");

    ms.func_open = [=](){ emit fa_open(); };

    ms.func_close = [=](){ emit fa_close(); is_login = false; }; //状态记录--未连接

    ms.func_register_back = [=](long long account,string passwd,bool ok)
    { emit fa_register_back(account,qfrs(passwd),ok); }; //qfrs(passwd)

    ms.func_login_back = [=](long long account,bool ok)
    { emit fa_login_back(account,ok); if(ok){ is_login = true; } }; //状态记录--以连接

    ms.func_logout_back = [=](long long account,bool ok)
    { if(ok){ ms.close_client(); } emit fa_logout_back(account,ok); };

    ms.func_recover_passwd_back = [=](long long account,string passwd,bool ok)
    { emit fa_recover_passwd_back(account,qfrs(passwd),ok); };

    ms.func_friends_list_back = [=](long long account,string name,bool end) //容器会连续接收信息
    { if(end){emit fa_friends_list_back(map);} else{map.insert(account,name);} };

    ms.func_add_ret_back= [=](long long account,bool ok,bool self)
    { emit fa_add_ret_back(account,ok,self); };


    //== 交换部分 ==
    ms.func_swap_txt = [=](long long account_from,string txt)
    { emit fa_swap_txt(account_from,qfrs(txt)); };

    ms.func_swap_add_friend = [=](long long account_from)
    { emit fa_swap_add_friend(account_from); };

    ms.func_swap_build = [=](long long account_from,string filename,en_build_file type)
    { vlogf(vv(account_from) vv(filename)); emit fa_swap_build(account_from,qfrs(filename),type); };

    ms.func_swap_file_finish = [=](long long account_from,string filename,en_build_file type,bool is_ok)
    { emit fa_swap_file_finish(account_from,qfrs(filename),type,is_ok); };

    ms.func_swap_file_ret = [=](long long account_from,string filename,en_build_file type,bool is_ok)
    { emit fa_swap_file_ret(account_from,qfrs(filename),type,is_ok); };

    ms.func_swap_error = [=](long long account_from,en_swap_error err)
    { emit fa_swap_error(account_from,err); };

    ms.func_prog_send = [=](string filename,long long account_from,int now)
    { emit fa_prog_send(qfrs(filename),account_from,now); };

    ms.func_prog_recv = [=](string filename,long long account_from,int now)
    { emit fa_prog_recv(qfrs(filename),account_from,now); };
}

int net_connect::open_connect(QString ip ,int port,QString txt)
{ return ms.open(qtos(ip),port,qtos(txt)); }

bool net_connect::get_status()
{ return is_login; }

void net_connect::close_connect()
{ ms.close_client(); }

void net_connect::set_file_path(QString path)
{ ms.set_file_path(qtos(path)); }

void net_connect::ask_register(QString passwd,QString name)
{ ms.ask_register(qtos(passwd),qtos(name)); }

void net_connect::ask_login(long long account,QString passwd)
{
    if(is_login) {return;}
    v_account=account;
    v_passwd = passwd;
    ms.ask_login(account,qtos(passwd));
}

void net_connect::ask_logout()
{ ms.ask_logout(v_account); }

void net_connect::ask_recover_passwd(long long account)
{ ms.ask_recover_passwd(account); }

void net_connect::ask_friends_list(long long account)
{ ms.ask_friends_list(account); }

void net_connect::ask_add_ret(long long account_to, bool is_agree)
{ ms.ask_add_ret(v_account,account_to,is_agree); }

void net_connect::ask_swap_add_friend(long long account_to)
{ ms.ask_swap_add_friend(v_account,account_to); }

void net_connect::ask_swap_txt(long long account_to, QString txt)
{ ms.ask_swap_txt(v_account,account_to,qtos(txt)); }

void net_connect::ask_swap_file
    (long long account_to, QString filename, QString file_path, en_build_file type)
{ ms. ask_swap_file(v_account,account_to,qtos(filename),qtos(file_path),type); }

long long net_connect::get_account()
{ return v_account; }

QString net_connect::get_passwd()
{ return v_passwd; }




