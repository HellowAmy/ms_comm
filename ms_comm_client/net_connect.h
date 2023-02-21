#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QDir>

#include "ms_web_client.h"

class net_connect : public QObject
{
    Q_OBJECT
public:
    explicit net_connect(QObject *parent = nullptr);
    int open_connect(QString ip = "127.0.0.1",int port = 5005,QString txt = "");
    long long get_account();
    QString get_passwd();
    bool get_status();
    void close_connect();
    void set_file_path(QString path);
    void ask_register(QString passwd,QString name);         //注册请求
    void ask_login(long long account,QString passwd);       //登录请求
    void ask_logout();                                      //登出请求
    void ask_recover_passwd(long long account);             //找回密码
    void ask_friends_list(long long account);               //好友列表请求
    void ask_add_ret(long long account_to,bool is_agree);   //好友申请结果
    void ask_swap_add_friend(long long account_to);         //好友添加
    void ask_swap_txt(long long account_to,QString txt);    //发送文字

    //发送文件(参数1:来自账号,参数2:目标账号,参数3:对方标记文件名,参数4:发送的文件路径,参数5:发送类型)
    void ask_swap_file(long long account_to,QString filename,QString file_path, en_build_file type);

signals:
    //请求服务器反馈
    emit void fa_open();
    emit void fa_close();
    emit void fa_register_back(long long account,QString passwd,bool ok);
    emit void fa_login_back(long long account,bool ok);
    emit void fa_logout_back(long long account,bool ok);
    emit void fa_recover_passwd_back(long long account,QString passwd,bool ok);
    emit void fa_friends_list_back(QMap<long long,string> map);
    emit void fa_add_ret_back(long long account,bool ok,bool self);

    //交换反馈
    emit void fa_swap_txt(long long account_from,QString txt);
    emit void fa_swap_add_friend(long long account_from);
    emit void fa_swap_build(long long account_from,QString filename,en_build_file type);
    emit void fa_swap_file_finish(long long account_from,QString filename,en_build_file type,bool is_ok);
    emit void fa_swap_file_ret(long long account_from,QString filename,en_build_file type,bool is_ok);
    emit void fa_swap_error(long long account_from,en_swap_error err);

    //进度条
    emit void fa_prog_send(QString filename,long long account_from,int now);
    emit void fa_prog_recv(QString filename,long long account_from,int now);

protected:
    ms_web_client ms;
    QMap<long long,string> map;

    QString v_passwd;
    long long v_account;
    bool is_login = false;
};

#endif // NET_CONNECT_H
