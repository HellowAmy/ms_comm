#include "net_connect.h"

net_connect::net_connect(QObject *parent)
{
    vinit_c(vlog::e_info);
}

int net_connect::open_client
    (long long account, std::string passwd, std::string ip, int port)
{
    v_account = account;
    v_passwd = passwd;

    web_c.func_open = [=](){
        web_c.ask_login(v_account,v_passwd);
    };
    web_c.func_close = [=](){
        vlogw("func_close");
        emit fa_connect_close();
    };
    web_c.func_login_back = [=]( bool ok){
        vlogf("func_login_back" v(ok));
        emit fa_login_status(ok);
    };
    web_c.func_swap_txt = [=](long long from,long long to,string txt){
        vlogf("func_login_back" v(from) v(to) v(txt));
        emit fa_swap_txt(from,QString::fromStdString(txt));
    };
    web_c.func_swap_file = [=](long long from,long long to,string filename){
        vlogf("func_login_back" v(from) v(to) v(filename));
        emit fa_swap_files(from,QString::fromStdString(filename));
    };

    return web_c.open(ip,port);
}

void net_connect::send_txt(long long account_to, std::string txt)
{
    web_c.ask_swap_txt(v_account,account_to,txt);
}

void net_connect::send_file(long long account_to, std::string filename)
{
    web_c.ask_swap_file(v_account,account_to,filename);
}


