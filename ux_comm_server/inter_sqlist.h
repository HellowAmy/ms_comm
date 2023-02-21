#ifndef INTER_SQLIST_H
#define INTER_SQLIST_H

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <sstream>

using std::string;
using std::vector;


class inter_sqlist
{
public:
    struct ct_ac_info
    {
        bool is_null;
        long long account;
        string passwd;
        string name;
    };

    string create_ac_info =
            R"(
                create table ac_info (
                    ac_id   integer         primary key     not null,
                    passwd  varchar(64)     not null,
                    name    varchar(32)
                );
            )";

    string create_ac_friends =
            R"(
                create table ac_friends (
                    ac_id       integer not null,
                    friends     integer ,
                    primary key (ac_id,friends),
                    foreign key(ac_id)      references ac_info(ac_id),
                    foreign key(friends)    references ac_info(ac_id),
                    check (ac_id != friends)
                );
            )";

    string table_ac_info = "ac_info";
    string table_ac_friends = "ac_friends";

public:
    inter_sqlist();
    bool open_db(string path);
    bool close_db();
    bool create_table(string table);
    bool insert_ac_info(long long account,string passwd,string name);
    bool insert_ac_friends(long long account,long long friends);
    bool find_account(long long account);
    ct_ac_info select_info(long long account);
    vector<long long> select_friends(long long account);
    vector<long long> select_account();

    //参见随机账号加入到表中(参数1:账号数量,参数2:统一密码,参数3:统一昵称)
    void build_account(int size,string passwd);
    void build_friends();

protected:
    sqlite3 *v_db;

    long long rand_account(int count);
};

#endif // INTER_SQLIST_H
