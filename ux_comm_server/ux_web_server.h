#ifndef UX_WEB_SERVER_H
#define UX_WEB_SERVER_H

#include "lib/web_sock/inter_sock.hpp"
#include "../web_protocol.h"
#include "lib/vts/vts.h"
#include "ux_manage.h"

#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <queue>
#include <iostream>

using std::function;
using std::unique_lock;
using std::string;
using std::map;
using std::pair;
using std::queue;
using std::mutex;
using std::condition_variable;
using std::placeholders::_1;
using std::placeholders::_2;
using std::bind;
using std::ref;


//===== 日志宏 =====
#define vv(value) "["#value": "<<value<<"] "
#define vloge(...) std::cout<<"\033[31m[Err] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogw(...) std::cout<<"\033[33m[War] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogd(...) std::cout<<"\033[32m[Deb] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<endl
#define vlogf(...) std::cout<<"[Inf] ["<<__FILE__<<":<"<<__LINE__ \
    <<">] <<<< "<<__VA_ARGS__<<endl
//===== 日志宏 =====

using func_task = function<void(const web_sock&, const string&)>;

class ux_web_server
{
public:
    ux_web_server();
    int open_server(int port = 5005,int th_count = 8);

protected:
    inter_server sock;

    string file_account;//账号数据
    map<long long,string> map_account;//已存在的账号密码

    //== 加入连接队列：登陆时记录登陆账号 ==
    std::mutex lock_connect;
    map<long long,web_sock> map_connect;//用账号索引sock
    map<web_sock,long long> map_connect_rm;//双向索引：加速移除sock
    bool add_to_connect(const long long &account,const web_sock& sock);
    bool move_connect(const web_sock &sock);//关闭时的对象
    bool move_connect(const long long &account);//指定账号
    //== 加入连接队列：登陆时记录登陆账号 ==


    //===== 任务函数 =====
    map<en_mode_index,func_task> map_task_func;//任务函数
    void task_register(const web_sock& sock, const string& meg);//账号注册
    void task_login(const web_sock& sock, const string& meg);//登录请求
    void task_logout(const web_sock& sock, const string& meg);//登出请求
    void task_recover_passwd(const web_sock& sock, const string& meg);//忘记密码
    void task_swap(const web_sock& sock, const string& meg);//数据交换
    //===== 任务函数 =====


    //== 网络函数回调 ==
    void on_open(const web_sock& sock, const web_http& http);
    void on_message(const web_sock& sock, const string& meg);
    void on_close(const web_sock& sock);
    //== 网络函数回调 ==


    //加入账号数据库
    std::mutex lock_account;
    bool add_to_account(const std::string &passwd, long long &ret_account);

    //发送到客户端
    std::mutex lock_send;
    bool send_str(const web_sock &sock,const string &str);

    //字符串相等
    template<class T1,class T2>
    bool s_equals(T1 s1,T2 s2)
    { return std::string(s1) == std::string(s2); }
};

#endif // UX_WEB_SERVER_H
