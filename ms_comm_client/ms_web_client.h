#ifndef MS_WEB_CLIENT_H
#define MS_WEB_CLIENT_H

#include "lib/web_sock/inter_sock.hpp"
#include "ux_protocol.h"
#include "ux_manage.h"
#include "lib/vts/vts.h"

#include <string>
#include <fstream>
#include <thread>
#include <queue>
#include <map>
#include <memory>
#include <condition_variable>

using vts::vlog;
using std::shared_ptr;
using std::string;
using std::thread;
using std::map;
using std::queue;
using std::condition_variable;
using std::placeholders::_1;
using std::placeholders::_2;
using std::bind;

#define WRITE_BUFSIZE_HIGH_WATER (1U << 23)  // 8M

class ms_web_client : public inter_client
{
public:
    explicit ms_web_client();
    void set_file_path(string path = "D/tmp/");

    int ask_register(string passwd);
    int ask_login(long long account,string passwd);
    int ask_swap_txt(long long account_from,long long account_to,string txt);
    void ask_swap_file(long long account_from,long long account_to,string filename);
    bool is_connect();

    function<void()> func_open = nullptr;
    function<void()> func_close = nullptr;
    function<void(long long account,string passwd,bool ok)> func_register_back = nullptr;
    function<void(bool ok)> func_login_back = nullptr;
    function<void(long long from,long long to,string txt)> func_swap_txt = nullptr;
    function<void(long long from,long long to,string filename)> func_swap_file = nullptr;

protected:
    string v_path_files;
    map<string,shared_ptr<fstream>> map_ofs;//文件名和读取流
    map<enum_transmit,function<void(const string&)>> map_func;//任务函数

    //===== 任务函数 =====
    void back_register(const string& meg);//账号注册
    void back_login(const string& meg);//登录请求
    void back_error(const string& meg);//错误反馈
    void back_swap_txt(const string& meg);//数据交换
    void back_swap_file(const string& meg);//文件交换
    //===== 任务函数 =====

    void on_open() override;
    void on_message(const string& meg) override;
    void on_close() override;

    int send_meg(const string& meg);
    bool write_file(shared_ptr<fstream> sp_ofs,const char *buf,int size);

};

#endif // MS_WEB_CLIENT_H
