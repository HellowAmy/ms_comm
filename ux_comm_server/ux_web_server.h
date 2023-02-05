#ifndef UX_WEB_SERVER_H
#define UX_WEB_SERVER_H

#include "lib/web_sock/inter_sock.hpp"
#include "../web_protocol.h"
#include "lib/vts/vts.h"
//#include "ux_protocol.h"
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

//using vts::vlog;
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


//===== 快速建立 =====
#define map_task_add(en,func) \
    map_task_func.insert(pair<en_mode_index,func_task> \
        (en,bind(&ux_web_server::func,this,_1,_2)))
//===== 快速建立 =====


using func_task = function<void(const web_sock&, const string&)>;

class ux_web_server //: public inter_server
{
public:
    ux_web_server();
    int open_server(int port = 5005,int th_count = 8);

protected:
    inter_server sock;

    void on_open(const web_sock& sock, const web_http& http);
    void on_message(const web_sock& sock, const string& meg);
    void on_close(const web_sock& sock);

    string file_account;
    map<long long,string> map_account;//已存在的账号密码
    map<long long,web_sock> map_connect;//已连接账号

    void move_connect();
    bool move_map_connect(const long long &account);



    map<en_mode_index,func_task> map_task_func;//任务函数

    //===== 任务函数 =====
    void task_register(const web_sock& sock, const string& meg);//账号注册
    void task_login(const web_sock& sock, const string& meg);//登录请求
    void task_logout(const web_sock& sock, const string& meg);//登出请求
    void task_recover_passwd(const web_sock& sock, const string& meg);//忘记密码

//    void task_error(const web_sock& sock, const string& meg);//错误反馈
    void task_swap(const web_sock& sock, const string& meg);//数据交换
//    void task_swap_file(const web_sock& sock, const string& meg);//文件交换
    //===== 任务函数 =====



    //加入连接队列
    std::mutex lock_connect;
    bool add_to_connect(const long long &account,const web_sock& sock);

    //加入账号数据库
    std::mutex lock_account;
    bool add_to_account(const std::string &passwd, long long &ret_account);

    //发送到客户端
    std::mutex lock_send;
    bool send_str(const web_sock &sock,const string &str);





//    //登录请求
//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//             (e_login,bind(&ux_web_server::task_login,this,_1,_2)));



};

#endif // UX_WEB_SERVER_H
