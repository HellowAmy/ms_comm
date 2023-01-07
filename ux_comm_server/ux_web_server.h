#ifndef UX_WEB_SERVER_H
#define UX_WEB_SERVER_H

#include "lib/web_sock/inter_sock.hpp"
#include "ux_protocol.h"
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

class ux_web_server : public inter_server
{
public:
    ux_web_server();
//    int open_server(int port = 5005,int thread = 8);

protected:
    void on_open(const web_sock& sock, const web_http& http) override;
    void on_message(const web_sock& sock, const string& meg) override;
    void on_close(const web_sock& sock) override;

    string file_account;
    map<long long,string> map_account;//已存在的账号密码
    map<long long,web_sock> map_connect;//已连接账号

    //任务函数
    map<enum_transmit,
        function<void(const web_sock&, const string&)>> map_func;

    //===== 任务函数 =====
    void task_register(const web_sock& sock, const string& meg);//账号注册
    void task_login(const web_sock& sock, const string& meg);//登录请求
    void task_error(const web_sock& sock, const string& meg);//错误反馈
    void task_swap_txt(const web_sock& sock, const string& meg);//数据交换
    void task_swap_file(const web_sock& sock, const string& meg);//文件交换
    //===== 任务函数 =====

    std::mutex lock_account;
    std::mutex lock_connect;
    std::mutex lock_send;
    int send_back(const web_sock &sock,const string &str);
};

#endif // UX_WEB_SERVER_H
