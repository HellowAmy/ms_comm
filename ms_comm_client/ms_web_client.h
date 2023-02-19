#ifndef MS_WEB_CLIENT_H
#define MS_WEB_CLIENT_H

#include "lib/web_sock/inter_sock.hpp"
#include "ux_manage.h"
#include "../web_protocol.h"

#include <string>
#include <fstream>
#include <thread>
#include <queue>
#include <map>
#include <memory>
#include <functional>
#include <iostream>
#include <condition_variable>

using std::shared_ptr;
using std::string;
using std::thread;
using std::map;
using std::mutex;
using std::queue;
using std::map;
using std::function;
using std::pair;
using std::condition_variable;
using std::placeholders::_1;
using std::placeholders::_2;
using std::bind;


//===== 日志宏 =====
#define vv(value) "["#value": "<<value<<"] "
#define vloge(...) std::cout<<"\033[31m[Err] ["<<__FILE__<<":<"<<__LINE__   \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<std::endl
#define vlogw(...) std::cout<<"\033[33m[War] ["<<__FILE__<<":<"<<__LINE__   \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<std::endl
#define vlogd(...) std::cout<<"\033[32m[Deb] ["<<__FILE__<<":<"<<__LINE__   \
    <<">] <<<< "<<__VA_ARGS__<<"\033[0m"<<std::endl
#define vlogf(...) std::cout<<"[Inf] ["<<__FILE__<<":<"<<__LINE__           \
    <<">] <<<< "<<__VA_ARGS__<<std::endl
//===== 日志宏 =====




using func_task = function<void(const string&)>;

#define WRITE_BUFSIZE_HIGH_WATER (1U << 23)  // 8M

class ms_web_client : public inter_client
{
public:
    //文件状态--发送
    struct io_send
    {
        bool is_send;           //正在发送
        int prog_now;           //进度条
        en_build_file type;     //传输类型
        long long size_block;   //块大小
        long long size_file;    //文件总长度
        long long account_to;   //账号--目标-接收者
        long long account_from; //账号--来自-发送者
        string insert_name;     //插入的文件索引
        string save_path;       //保存路径
        fstream ofs;            //打开的文件对象
    };

    //文件状态--接收
    struct io_recv
    {
        bool is_send;           //正在接收
        int prog_now;           //进度条
        en_build_file type;     //传输类型
        long long size_block;   //块大小
        long long size_file;    //文件总长度
        long long account_to;   //账号--目标-接收者
        long long account_from; //账号--来自-发送者
        string insert_name;     //插入的文件索引
        string filename;        //插入的文件索引法
        string save_path;       //保存路径
        string send_path;       //发送方的路径
        fstream ofs;            //打开的文件对象
    };

public:
    explicit ms_web_client();
    void set_file_path(string path);
    string get_file_path();

    void ask_register(string passwd);                   //注册请求
    void ask_login(long long account,string passwd);    //登录请求
    void ask_logout(long long account);                 //登出请求
    void ask_recover_passwd(long long account);         //找回密码
    void ask_friends_list(long long account);           //好友列表请求
    void ask_add_ret(long long account_from,            //好友申请结果
            long long account_to,bool is_agree);

    //好友添加
    void ask_swap_add_friend(long long account_from,
                         long long account_to);

    //发送文字
    void ask_swap_txt(long long account_from,
                      long long account_to,string txt);

    //发送文件(参数1:来自账号,参数2:目标账号,参数3:标记文件名,参数4:发送的文件路径,参数5:发送类型)
    void ask_swap_file(long long account_from,
                       long long account_to,
                       string filename,
                       string file_path,
                       en_build_file type);

    //===== 回调函数 =====
    function<void()> func_open = nullptr;
    function<void()> func_close = nullptr;

    //请求服务器反馈
    function<void(long long account,string passwd,bool ok)>
        func_register_back = nullptr;
    function<void(long long account,bool ok)>
        func_login_back = nullptr;
    function<void(long long account,bool ok)>
        func_logout_back = nullptr;
    function<void(long long account,string passwd,bool ok)>
        func_recover_passwd_back = nullptr;
    function<void(long long account,string name,bool end)>
        func_friends_list_back = nullptr;
    function<void(long long account,bool ok,bool self)>
        func_add_ret_back = nullptr;
    //请求服务器反馈

    //交换反馈
    function<void(long long account_from,string txt)>
        func_swap_txt = nullptr;
    function<void(long long account_from)>
        func_swap_add_friend = nullptr;
    function<void(long long account_from,string filename,en_build_file type)>
        func_swap_build = nullptr;
    function<void(long long account_from,string filename,en_build_file type,bool is_ok)>
        func_swap_file_finish = nullptr;
    function<void(long long account_from,string filename,en_build_file type,bool is_ok)>
        func_swap_file_ret = nullptr;
    function<void(long long account_from,en_swap_error err)>
        func_swap_error = nullptr;
    //交换反馈

    //进度条
    function<void(string filename,long long account_from,int now)> func_prog_send = nullptr;
    function<void(string filename,long long account_from,int now)> func_prog_recv = nullptr;
    //进度条
    //===== 回调函数 =====


protected:
    string v_path_files = "/tmp/qt_comm_cache/recv_file/";

    map<string,io_send> map_send;//文件名和文件状态--发送
    map<string,io_recv> map_recv;//文件名和文件状态--接收
    map<en_mode_index,func_task> map_task_func;//任务函数

    //===== 任务函数 =====
    //== 请求服务器反馈 ==
    void register_back(const string& meg);          //注册任务反馈-c
    void login_back(const string& meg);             //登录请求反馈-c
    void logout_back(const string& meg);            //登出请求反馈-c
    void recover_passwd_back(const string& meg);    //找回密码反馈-c
    void friends_list_back(const string& meg);      //好友列表反馈-c
    void add_ret_back(const string& meg);           //申请结果反馈-c
    //== 请求服务器反馈 ==

    //== 文件转发处理 ==
    void swap_file_build(const string& meg);      //建立文件:c1->c2
    void swap_file_request(const string& meg);    //发送文件段请求:c2->c1
    void swap_file_send(const string& meg);       //发送文件段:c1->c2
    void swap_file_finish(const string& meg);     //发送完成:c1->c2
    void swap_file_ret(const string& meg);        //发送结果反馈:c2->c1
    //== 文件转发处理 ==

    void swap_txt(const string& meg);           //交换文字
    void swap_add_friend(const string& meg);    //好友添加
    void swap_error(const string& meg);         //错误反馈
    //===== 任务函数 =====

    void on_open() override;
    void on_message(const string& meg) override;
    void on_close() override;

    long long write_buf(fstream *ofs,const char *buf,int size);
    long long read_buf(fstream *ofs,char *buf,int size);

    //发送到服务端
    std::mutex lock_send;
    bool send_str(const string &str);
};

#endif // MS_WEB_CLIENT_H
