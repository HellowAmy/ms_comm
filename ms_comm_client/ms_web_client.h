#ifndef MS_WEB_CLIENT_H
#define MS_WEB_CLIENT_H

#include "lib/web_sock/inter_sock.hpp"
//#include "ux_protocol.h"
#include "ux_manage.h"
#include "../web_protocol.h"

//#include "lib/vts/vts.h"

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


//register_back

//===== 快速建立 =====
#define map_task_add(func) \
    map_task_func.insert(pair<en_mode_index,func_task> \
        (en_mode_index::e_##func,bind(&ms_web_client::func,this,_1)))
//===== 快速建立 =====

using func_task = function<void(const string&)>;

//#define WRITE_BUFSIZE_HIGH_WATER (1U << 23)  // 8M

class ms_web_client : public inter_client
{
public:
    explicit ms_web_client();
    void set_file_path(string path = "D/tmp/");

    void ask_register(string passwd);//注册请求
    void ask_login(long long account,string passwd);//登录请求
    void ask_logout(long long account);//登出请求
    void ask_recover_passwd(long long account);//找回密码
    void ask_swap_txt(long long account_from,long long account_to,string txt);//发送文字
    void ask_swap_file(long long account_from,long long account_to,string filename);//发送文件

//    bool is_connect();







//    void register_back(const string& meg);          //注册任务反馈-c
//    void login_back(const string& meg);             //登录请求反馈-c
//    void logout_back(const string& meg);            //登出请求反馈-c
//    void recover_passwd_back(const string& meg);    //找回密码反馈-c
//    void disconnect_txt(const string& meg);         //目标账号未连接--发送文字-c
//    void disconnect_file(const string& meg);        //目标账号未连接--发送文件-c
//    void disconnect(const string& meg);             //转发未连接
//    void swap_txt(const string& meg);               //交换文字-c2
//    void swap_file_build(const string& meg);        //建立文件-c2
//    void swap_file_build_err(const string& meg);    //建立文件错误-c1
//    void swap_file_send(const string& meg);         //发送文件段-c2
//    void swap_file_send_err(const string& meg);     //发送文件错误-c1
//    void swap_file_ret_err(const string& meg);      //接收文件完整性错误-c1



    function<void()> func_open = nullptr;
    function<void()> func_close = nullptr;
//    function<void(const string& meg)> func_swap = nullptr;




    function<void(long long account,string passwd,bool ok)> func_register_back = nullptr;
    function<void(long long account,bool ok)> func_login_back = nullptr;
    function<void(long long account,bool ok)> func_logout_back = nullptr;
    function<void(long long account,string passwd,bool ok)> func_recover_passwd_back = nullptr;
    function<void(long long account_to,ct_head_mode head_info)> func_disconnect = nullptr;
    function<void(long long account_from,string txt)> func_swap_txt = nullptr;


//    //交换文字
//    CT_BUILD_SWAP(ct_swap_txt,
//        long long account_from;
//        char buf_txt[1024];
//    );

//    //转发未连接
//    CT_BUILD_MODE(ct_disconnect,
//        long long account_to;
//        ct_head_mode head_info;
//    );

//    function<void(long long from,long long to,string txt)> func_swap_txt = nullptr;
//    function<void(long long from,long long to,string filename)> func_swap_file = nullptr;

protected:
    string v_path_files;
    map<string,shared_ptr<fstream>> map_ofs;//文件名和读取流
//    map<enum_transmit,function<void(const string&)>> map_task_func;//任务函数





    map<en_mode_index,func_task> map_task_func;//任务函数
//    map<en_mode_index,func_task> map_task_func;//任务函数

    //===== 任务函数 =====
    void register_back(const string& meg);          //注册任务反馈-c
    void login_back(const string& meg);             //登录请求反馈-c
    void logout_back(const string& meg);            //登出请求反馈-c
    void recover_passwd_back(const string& meg);    //找回密码反馈-c
    void disconnect_txt(const string& meg);         //目标账号未连接--发送文字-c
    void disconnect_file(const string& meg);        //目标账号未连接--发送文件-c
    void disconnect(const string& meg);             //转发未连接
    void swap_txt(const string& meg);               //交换文字-c2
    void swap_file_build(const string& meg);        //建立文件-c2
    void swap_file_build_err(const string& meg);    //建立文件错误-c1
    void swap_file_send(const string& meg);         //发送文件段-c2
    void swap_file_send_err(const string& meg);     //发送文件错误-c1
    void swap_file_ret_err(const string& meg);      //接收文件完整性错误-c1
    //===== 任务函数 =====

//ct_head_mode cd;
//ct_swap_txt l1;
//    e_register,             //注册请求-s
//    e_register_back,        //注册任务反馈-c
//    e_login,                //登录请求-s
//    e_login_back,           //登录请求反馈-c
//    e_logout,               //登出请求-s
//    e_logout_back,          //登出请求反馈-c
//    e_recover_passwd,       //找回密码-s
//    e_recover_passwd_back,  //找回密码反馈-c
//    e_disconnect_txt,       //目标账号未连接--发送文字-c
//    e_disconnect_file,      //目标账号未连接--发送文件-c
//    e_disconnect,           //转发未连接
//    e_swap_txt,             //交换文字-c2
//    e_swap_file_build,      //建立文件-c2
//    e_swap_file_build_err,  //建立文件错误-c1
//    e_swap_file_send,       //发送文件段-c2
//    e_swap_file_send_err,   //发送文件错误-c1
//    e_swap_file_ret_err,    //接收文件完整性错误-c1
//    e_swap_file_request,    //发送文件段请求-c2




//    void back_error(const string& meg);//错误反馈
//    void back_swap_txt(const string& meg);//数据交换
//    void back_swap_file(const string& meg);//文件交换
    //===== 任务函数 =====

    void on_open() override;
    void on_message(const string& meg) override;
    void on_close() override;

//    int send_meg(const string& meg);
    bool write_file(shared_ptr<fstream> sp_ofs,const char *buf,int size);


    //发送到服务端
    std::mutex lock_send;
    template<class Tsock>
    bool send_str(const Tsock &sock,const string &str)
    {
        std::unique_lock<mutex> lock(lock_send);
        sock->send(str.c_str(),str.size());
        return sock->isConnected();
    }
};

#endif // MS_WEB_CLIENT_H
