#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include <QObject>

#include "ms_web_client.h"

class net_connect : public QObject
{
    Q_OBJECT
public:
    explicit net_connect(QObject *parent = nullptr);
    int open_connect(string ip = "127.0.0.1",int port = 5005,string txt = "");
    long long get_account();
    string get_passwd();
    bool get_status();
    void close_connect();
    void set_file_path(string path);
    void ask_register(string passwd);//注册请求
    void ask_login(long long account,string passwd);//登录请求
    void ask_logout();//登出请求
    void ask_recover_passwd(long long account);//找回密码

    //发送文字
    void ask_swap_txt(long long account_to,string txt);

    //发送文件(参数1:来自账号,参数2:目标账号,参数3:对方标记文件名,参数4:发送的文件路径,参数5:发送类型)
    void ask_swap_file(long long account_to,string filename,string file_path, en_build_file type);

signals:
    //请求服务器反馈
    emit void fa_open();
    emit void fa_close();
    emit void fa_register_back(long long account,string passwd,bool ok);
    emit void fa_login_back(long long account,bool ok);
    emit void fa_logout_back(long long account,bool ok);
    emit void fa_recover_passwd_back(long long account,string passwd,bool ok);

    //交换反馈
    emit void fa_swap_txt(long long account_from,string txt);
    emit void fa_swap_file_finish(long long account_from,string filename,en_build_file type,bool is_ok);
    emit void fa_swap_file_ret(long long account_from,string filename,en_build_file type,bool is_ok);
    emit void fa_swap_error(long long account_from,en_swap_error err);

    //进度条
    emit void fa_prog_send(string filename,long long account_from,int now);
    emit void fa_prog_recv(string filename,long long account_from,int now);

protected:
    ms_web_client ms;
    string v_passwd;
    long long v_account;
    bool is_login = false;
};

#endif // NET_CONNECT_H
