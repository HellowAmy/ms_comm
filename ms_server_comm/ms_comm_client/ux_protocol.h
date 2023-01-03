#ifndef UX_PROTOCOL_H
#define UX_PROTOCOL_H

//#include "ux_protocol.h"

enum enum_transmit
{
    e_register,
    e_register_back,

    e_login,
    e_login_back,

    e_error,

    e_swap_txt,
    e_swap_file
};

//struct ct_cmd
//{
//    int type;//存储enum_transmit枚举值
//};

struct ct_register
{
    char passwd[64];
};

struct ct_register_back
{
    long long account;
    char passwd[64];
    int flg;//注册成功返回1，失败返回0
};

struct ct_login
{
    long long account;
    char passwd[64];
};

struct ct_login_back
{
    int flg;//登录成功返回1，失败返回0
};

struct ct_error
{
    long long account;
    char error[64];
};

struct ct_swap_txt
{
    long long account_from;
    long long account_to;
    char data[1024];
};

struct ct_swap_file
{
    long long account_from;
    long long account_to;
    long long lenght;//文件长度
    int status;//传输状态 -- 0:首次参数, 1:后续内容传输, 2:最后一次传输
    int buf_size;//本次内容的真实长度
    char filename[255];//文件名,只存在于首次传输(0)
    char buf[4096];//文件内容
};


#endif // UX_PROTOCOL_H
