#ifndef WEB_PROTOCOL_H
#define WEB_PROTOCOL_H

//#include "web_protocol.h"


//===== 解析结构体 =====
//解体结构体信息
#include <string>

//string转索引结构体
template <class T_head>
static void to_head(const std::string &str,T_head &h)
{ h = *(T_head*)str.c_str(); }

//结构体转string
template <class T_ct>
static std::string to_str(T_ct ct)
{ return std::string((char*)&ct,sizeof(T_ct)); }

//string转结构体
template <class T_ct>
static void to_ct(const std::string &str,T_ct &ct)
{ ct = *(T_ct*)str.c_str(); }
//===== 解析结构体 =====



//===== 网络传输数据结构 =====
//连接传输模式
enum en_mode
{
    e_request,      //任务请求
    e_response,     //任务响应
    e_swap,         //交换数据
};

//连接传输模式--索引对应函数
enum en_mode_index
{
    e_register,             //注册请求-s
    e_register_back,        //注册任务反馈-c
    e_login,                //登录请求-s
    e_login_back,           //登录请求反馈-c
    e_logout,               //登出请求-s
    e_logout_back,          //登出请求反馈-c
    e_recover_passwd,       //找回密码-s
    e_recover_passwd_back,  //找回密码反馈-c
    e_disconnect_txt,       //目标账号未连接--发送文字-c
    e_disconnect_file,      //目标账号未连接--发送文件-c
    e_disconnect,           //转发未连接
    e_swap_txt,             //交换文字-c2
    e_swap_file_build,      //建立文件-c2
    e_swap_file_build_err,  //建立文件错误-c1
    e_swap_file_send,       //发送文件段-c2
    e_swap_file_send_err,   //发送文件错误-c1
    e_swap_file_ret_err,    //接收文件完整性错误-c1
    e_swap_file_request,    //发送文件段请求-c2
};


//CT_BUILD_MODE模板长度
struct ct_head_mode
{
    en_mode type;                   
    en_mode_index func;
};

//CT_BUILD_SWAP模板长度
struct ct_head_swap
{
    en_mode type;                   
    en_mode_index func;
    long long account_to;
};


//任务结构体生成模板--请求答复模式
#define CT_BUILD_MODE(ct_name,...)  \
struct ct_name                      \
{                                   \
    ct_head_mode head;              \
    __VA_ARGS__;                    \
}                                   \

//任务结构体生成模板--交换数据
#define CT_BUILD_SWAP(ct_name,...)  \
struct ct_name                      \
{                                   \
    ct_head_swap head;              \
    __VA_ARGS__;                    \
}                                   \



//快速声明结构体--请求
#define MAKE_CT_REQ(in_name,in_type)                \
    ct_##in_type in_name;                           \
    in_name.head.type = en_mode::e_request;         \
    in_name.head.func = en_mode_index::e_##in_type  \

//快速声明结构体--应答
#define MAKE_CT_RES(in_name,in_type)                \
    ct_##in_type in_name;                           \
    in_name.head.type = en_mode::e_response;        \
    in_name.head.func = en_mode_index::e_##in_type  \

//快速声明结构体--交换
#define MAKE_CT_SWAP(in_name,in_type,in_account)    \
    ct_##in_type in_name;                           \
    in_name.head.type = en_mode::e_swap;            \
    in_name.head.func = en_mode_index::e_##in_type; \
    in_name.head.account_to = in_account            \



//注册请求
CT_BUILD_MODE(ct_register,
    char passwd[64];
);

//注册任务反馈  
CT_BUILD_MODE(ct_register_back,
    bool is_success;
    long long account;
    char passwd[64];
);

//登录请求   
CT_BUILD_MODE(ct_login,
    long long account;
    char passwd[64];
);    

//登录请求反馈
CT_BUILD_MODE(ct_login_back,
    bool is_success;
    long long account;
);

//登出请求
CT_BUILD_MODE(ct_logout,
    long long account;
);

//登出请求反馈
CT_BUILD_MODE(ct_logout_back,
    bool is_success;
    long long account;
);

//找回密码
CT_BUILD_MODE(ct_recover_passwd,
    long long account;
);

//找回密码反馈
CT_BUILD_MODE(ct_recover_passwd_back,
    bool is_success;
    long long account;
    char passwd[64];
);

//目标账号未连接--发送文字
CT_BUILD_MODE(ct_disconnect_txt,
    long long account_to;
    char buf_txt[1024];
);

//目标账号未连接--发送文件
CT_BUILD_MODE(ct_disconnect_file,
    long long account_to;
    char filename[255];
);

//转发未连接
CT_BUILD_MODE(ct_disconnect,
    long long account_to;
    ct_head_mode head_info;
);

//交换文字
CT_BUILD_SWAP(ct_swap_txt,
    long long account_from;
    char buf_txt[1024];
);

//建立文件
CT_BUILD_SWAP(ct_swap_file_build,
    long long account_from;
    long long block_sum;
    long long size_sum;
    char filename[255];
);

//建立文件错误
CT_BUILD_SWAP(ct_swap_file_build_err,
    long long account_from;
    char filename[255];
);

//发送文件段
CT_BUILD_SWAP(ct_swap_file_send,
    long long account_from;
    long long block_count;
    long long size_buf;
    char filename[255];
    char buf[4096];
);

//发送文件错误
CT_BUILD_SWAP(ct_swap_file_send_err,
    long long account_from;
    char filename[255];
);

//接收文件完整性错误
CT_BUILD_SWAP(ct_swap_file_ret_err,
    long long account_from;
    char filename[255];
);

//发送文件段请求
CT_BUILD_SWAP(ct_swap_file_request,
    long long account_from;
    long long block_count;
    char filename[255];
);


//===== 网络传输数据结构 =====


#endif // WEB_PROTOCOL_H


