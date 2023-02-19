#include <iostream>
#include <vector>

#include "sqlite3.h"
#include <string.h>

#include "inter_sqlist.h"
//#include <sqlite3ext.h>

//struct in
//{
//    long long ac;
//    char ps[64];
//    char ne[64];
//};

//std::vector<in> vec;

using namespace std;



//static int callback(void *NotUsed, int argc, char **argv, char **azColName){
//    cout<<argc<<endl;
//    if(argc < 3) { cout<<"po 2"<<endl; return 0; }
//    in w;
//    w.ac = stoll(argv[0]);
//    strncpy(w.ps,argv[1],sizeof (w.ps));
//    strncpy(w.ne,argv[2],sizeof (w.ne));
//    vec.push_back(std::move(w));

//    return 0;
//}

int main()
{


    inter_sqlist in;
    if(in.open_db("../account_info.db"))
    {

//        cout<<in.close_db()<<endl;
        cout<<in.create_table(in.create_ac_info)<<endl;
        cout<<in.create_table(in.create_ac_friends)<<endl;


//        vector<long long> vec = in.select_account();
//        for(auto a:vec)
//        {
//            cout<<a<<endl;
//        }
//        cout<<in.insert_ac_info(1234567890,"123qwe","nihao")<<endl;
//        cout<<in.insert_ac_info(1234567891,"123qwe","nihao")<<endl;
//        cout<<in.insert_ac_info(1234567892,"123qwe","nihao")<<endl;
//        cout<<in.insert_ac_info(1234567893,"123qwe","nihao")<<endl;

//        cout<<in.insert_ac_friends(1234567890,1234567891)<<endl;
//        cout<<in.insert_ac_friends(1234567890,1234567892)<<endl;
//        cout<<in.insert_ac_friends(1234567891,1234567892)<<endl;
//        cout<<in.insert_ac_friends(1234567891,1234567893)<<endl;


        in.build_account(10,"123456");

        in.build_friends();

//        cout<<"========="<<endl;
        in.close_db();

    }


//        cout<<in.find_account(1234567890)<<endl;
//        cout<<in.find_account(1234567891)<<endl;
//        cout<<in.find_account(1234567895)<<endl;
//        cout<<in.find_account(1234567894)<<endl;


//        cout<<"============="<<endl;
//        vector<long long> vec = in.select_friends(1234567892);
//        for(auto a:vec)
//        {

//            inter_sqlist::ct_ac_info ct = in.select_info(a);
//            if(ct.is_null == false)
//            {
//                cout<<ct.account<<"|"<<ct.passwd<<"|"<<ct.name<<endl;
//            }
//            else cout<<"is_null"<<endl;



//    sqlite3 *db;
//    char *errmsg;
//    int cmd;

//    （8位数字账号【主建】【非空】，64位字符密码【非空】，32位字符昵称）
//    create table ac_info (
//        ac_id   integer         primary key     not null,
//        pw      varchar(64)     not null,
//        name    varchar(32)
//    );

//    （8位数字账号【外键】【非空】，不定长字符好友列表【外键】【不等于索引】）
//    create table ac_friends (
//        ac_id       integer not null,
//        friends     integer ,
//        primary key (ac_id,friends),
//        foreign key(ac_id)      references ac_info(ac_id),
//        foreign key(friends)    references ac_info(ac_id),
//        check (ac_id != friends)
//    );





//    if (sqlite3_open("test.db", &db) == SQLITE_OK)
//    {

//        string cmd1 =
//                R"(
//                    create table ac_info (
//                        ac_id   integer         primary key     not null,
//                        pw      varchar(64)     not null,
//                        name    varchar(32)
//                    );
//                )";

//        string cmd2 =
//                R"(
//                    create table ac_friends (
//                        ac_id       integer not null,
//                        friends     integer ,
//                        primary key (ac_id,friends),
//                        foreign key(ac_id)      references ac_info(ac_id),
//                        foreign key(friends)    references ac_info(ac_id),
//                        check (ac_id != friends)
//                    );
//                )";

//        int res1 = sqlite3_exec(db,cmd1.c_str(),&callback,0,NULL);
//        int res2 = sqlite3_exec(db,cmd2.c_str(),&callback,0,NULL);
//        cout<<res1<<"|"<<res2<<endl;

//        string in1 =
//                R"(
//                    insert into ac_info values(1234567890,'123qwe','nihao0');
//                    insert into ac_info values(1234567891,'123qwe','nihao1');
//                    insert into ac_info values(1234567892,'123qwe','nihao2');
//                    insert into ac_info values(1234567893,'123qwe','nihao3');
//                    insert into ac_info values(1234567894,'123qwe','nihao4');
//                )";

//        string in2 =
//                R"(
//                    insert into ac_friends values(1234567890,1234567893);
//                    insert into ac_friends values(1234567890,1234567891);
//                    insert into ac_friends values(1234567890,1234567892);
//                    insert into ac_friends values(1234567891,1234567890);
//                    insert into ac_friends values(1234567891,1234567892);
//                    insert into ac_friends values(1234567891,1234567893);
//                    insert into ac_friends values(1234567892,1234567890);
//                    insert into ac_friends values(1234567892,1234567891);
//                    insert into ac_friends values(1234567892,1234567893);
//                )";


//        long long account = 1234567894;
//        std::string passwd = "123qwe";
//        std::string name = "nihao4";
//        std::string table_ac_info = "ac_info";
//        char buf[1024];
//        snprintf(buf,sizeof(buf),"insert into %s values(%lld,%s,%s);",
//                 table_ac_info.c_str(),account,passwd.c_str(),name.c_str());



//        cout<<buf<<endl;


//        int r1 = sqlite3_exec(db,in1.c_str(),&callback,0,NULL);
//        int r2 = sqlite3_exec(db,in2.c_str(),&callback,0,NULL);
//        cout<<r1<<"|"<<r2<<endl;



//        string ine1 =
//                R"(
//                    select * from ac_info;
//                )";

//        string ine2 =
//                R"(
//                    select * from ac_friends;
//                )";

//        int re1 = sqlite3_exec(db,ine1.c_str(),&callback,0,NULL);

//        for(auto a :vec)
//        {
//            cout<<a.ac<<"|"<<a.ps<<"|"<<a.ne<<endl;
//        }
//        int re2 = sqlite3_exec(db,ine2.c_str(),&callback,0,NULL);


//        cout<<re1<<"|"<<re2<<endl;



//        insert into ac_info values(1234567890,'123qwe','nihao0');
//        insert into ac_info values(1234567891,'123qwe','nihao1');
//        insert into ac_info values(1234567892,'123qwe','nihao2');
//        insert into ac_info values(1234567893,'123qwe','nihao3');
//        insert into ac_info values(1234567894,'123qwe','nihao4');


//        printf("open database sucess.\n");
//    printf(	"%s\n",sqlite3_errmsg(db));
//    return -1;

//        sqlite3_close(db);
//    }
//    else
//    {
//        printf("else === open database sucess.\n");
//    }





    cout << "Hello World!" << endl;
    return 0;
}
