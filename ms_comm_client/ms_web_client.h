#ifndef MS_WEB_CLIENT_H
#define MS_WEB_CLIENT_H

#include "lib/web_sock/inter_sock.hpp"
#include "ux_protocol.h"
#include "ux_manage.h"
#include "lib/vlog.hpp"

#include <string>
#include <fstream>
#include <thread>
#include <queue>
#include <map>
#include <condition_variable>

using std::string;
using std::thread;
using std::map;
using std::queue;
using std::condition_variable;
using std::placeholders::_1;
using std::placeholders::_2;
using std::bind;

class ms_web_client : public inter_client
{
public:
    explicit ms_web_client();
//    int open_client(string ip = "127.0.0.1",int port = 5005,int thread = 1);

    int ask_register(string passwd);
    int ask_login(long long account,string passwd);
    int ask_swap_txt(long long account_from,long long account_to,string txt);
    void ask_swap_file(long long account_from,long long account_to,string filename);

    function<void()> func_open = nullptr;
    function<void(long long account,string passwd,bool ok)> func_register_back = nullptr;
    function<void(bool ok)> func_login_back = nullptr;
    function<void(long long from,long long to,string txt)> func_swap_txt = nullptr;
    function<void(long long from,long long to,string filename)> func_swap_file = nullptr;

protected:
    map<string,fstream> map_ofs;//文件名和读取流
    map<enum_transmit,function<void(const string&)>> map_func;//任务函数

    //===== 任务函数 =====
    void back_register(const string& meg);//账号注册
    void back_login(const string& meg);//登录请求
    void back_error(const string& meg);//错误反馈
    void back_swap_txt(const string& meg);//数据交换
    void back_swap_file(const string& meg);//文件交换
    //===== 任务函数 =====

//    bool is_working = true;//启动标记
//    queue<function<void()>> queue_task;//任务队列
//    std::mutex cv_lock;//操作变量前准备锁
//    std::condition_variable cv_var;//条件变量--等待唤醒
//    void work_thread();//任务线程

    void on_open() override;
    void on_message(const string& meg) override;
    void on_close() override;

    int send_meg(const string& meg);

};

#endif // MS_WEB_CLIENT_H
