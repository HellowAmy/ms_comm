#include "net_connect.h"
#include "lib/vlog.hpp"

net_connect::net_connect(QObject *parent)
{
    vflog::instance()->init(vflog::e_info);
    vflog::instance()->close_log(false,false);
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

void net_connect::send_file(long long account_to, std::string filename)
{
    web_c.ask_swap_file(v_account,account_to,filename);
}


