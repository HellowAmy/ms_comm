#ifndef UX_MANAGE_H
#define UX_MANAGE_H

#include <time.h>
#include <string.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>

#include <fstream>
#include <map>

using std::getline;
using std::fstream;
using std::string;
using std::map;
using std::pair;
using std::ios;

class ux_manage
{
public:
    ux_manage();

    //生产随机账号
    static long long rand_account(int count = 10);

    //保存账号
    static bool save_account(const string &filename,
                             const map<long long,string> &map_account);
    //加载账号
    static bool load_account(const string &filename,
                             map<long long,string> &map_account,
                             int size_buf = 10);
    //账号插入到容器，并返回账号
    static long long add_account(map<long long,string> &map_account,
                            string passwd);

    //账号插入到容器，并返回账号
    static bool add_account(map<long long,string> &map_account,
                            const string &passwd,long long &ret_account);

    //账号和fd插入到容器连接队列
    template<class T>
    static bool add_connect(map<long long,T> &map_connect,long long account,T sock)
    {
        if(map_connect.find(account) == map_connect.end())
        {
            map_connect.insert(pair<long long,T>(account,sock));
            return true;
        }
        else return false;
    }

    //结构体转string
    template <class T_head,class T_ct>
    static string to_str(T_head h,T_ct ct)
    {
        return string((char*)&h,sizeof(T_head)) +
                string((char*)&ct,sizeof(T_ct));
    }

    //string转结构体
    template <class T_head,class T_ct>
    static void from_str(const string &str,T_head &h,T_ct &ct)
    {
        h = *(T_head*)string(str.begin(),str.begin()+sizeof(T_head)).c_str();
        ct = *(T_ct*)string(str.begin()+sizeof(T_head),str.end()).c_str();
    }
};

#endif // UX_MANAGE_H


