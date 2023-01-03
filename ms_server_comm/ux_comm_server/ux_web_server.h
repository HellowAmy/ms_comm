#ifndef UX_WEB_SERVER_H
#define UX_WEB_SERVER_H

#include "lib/web_sock/inter_sock.hpp"
#include "ux_protocol.h"
#include "log_show.h"
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

class ux_web_server : protected inter_server
{
public:
    ux_web_server();
    int open_server(int port = 5005,int thread = 1);

protected:
    void on_open(const web_sock& sock, const web_http& http) override;
    void on_message(const web_sock& sock, const string& meg) override;
    void on_close(const web_sock& sock) override;

    string file_account;
    map<long long,string> map_account;//已存在的账号密码
    map<long long,const web_sock&> map_connect;//已连接账号

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


    bool is_working = true;//启动标记
    queue<function<void()>> queue_task;//任务队列
    std::mutex cv_lock;//操作变量前准备锁
    std::condition_variable cv_var;//条件变量--等待唤醒
    void work_thread();//任务线程
};

#endif // UX_WEB_SERVER_H
