#include "inter_sqlist.h"

template<typename T>
std::string tos(const T& t)
{
    std::ostringstream oss; oss << t;
    return oss.str();
}

template<typename T>
T frs(const std::string& str)
{
    T t; std::istringstream iss(str); iss >> t;
    return t;
}

inter_sqlist::inter_sqlist()
{

}

bool inter_sqlist::open_db(std::string path)
{ return (sqlite3_open(path.c_str(), &v_db) == SQLITE_OK); }

bool inter_sqlist::close_db()
{ return (sqlite3_close(v_db) == SQLITE_OK); }

bool inter_sqlist::create_table(std::string table)
{ return (sqlite3_exec(v_db,table.c_str(),nullptr,0,NULL) == 0); }

bool inter_sqlist::insert_ac_info(long long account, std::string passwd, std::string name)
{
    char buf[1024];
    snprintf(buf,sizeof(buf),"insert into %s values(%lld,'%s','%s');",
             table_ac_info.c_str(),account,passwd.c_str(),name.c_str());
    return (sqlite3_exec(v_db,buf,nullptr,0,NULL) == 0);
}

bool inter_sqlist::insert_ac_friends(long long account, long long friends)
{
    char buf[1024];
    snprintf(buf,sizeof(buf),"insert into %s values(%lld,%lld);",
             table_ac_friends.c_str(),account,friends);
    return (sqlite3_exec(v_db,buf,nullptr,0,NULL) == 0);
}

bool inter_sqlist::find_account(long long account)
{
    auto callback = [](void *NotUsed, int argc, char **argv, char **azColName){
        if(argc > 0) { *(bool*)NotUsed = true; return 0; } //取出参数指针并赋值
        else return -1;
    };

    bool is_find = false;
    char buf[1024];
    snprintf(buf,sizeof(buf),"select * from %s where ac_id==%lld;",
             table_ac_info.c_str(),account);
    sqlite3_exec(v_db,buf,callback,&is_find,NULL);
    return is_find;
}

vector<long long> inter_sqlist::select_friends(long long account)
{
    auto callback = [](void *NotUsed, int argc, char **argv, char **azColName){
        vector<long long> *pvec = (vector<long long>*)NotUsed;
        for(int i=0;i<argc;i++)
        { (*pvec).push_back(frs<long long>(argv[i])); }
        return 0;
    };

    vector<long long> vec;
    char buf[1024];
    snprintf(buf,sizeof(buf),"select friends from %s where ac_id==%lld;",
             table_ac_friends.c_str(),account);
    sqlite3_exec(v_db,buf,callback,&vec,NULL);
    return vec;
}

vector<long long> inter_sqlist::select_account()
{
    vector<long long> vec;

    auto callback = [](void *NotUsed, int argc, char **argv, char **azColName){
        vector<long long> *pvec = (vector<long long>*)NotUsed;
        for(int i=0;i<argc;i++)
        { (*pvec).push_back(frs<long long>(argv[i])); }
        return 0;
    };

    char buf[1024];
    snprintf(buf,sizeof(buf),"select ac_id from %s;",
             table_ac_info.c_str());
    sqlite3_exec(v_db,buf,callback,&vec,NULL);
    return vec;
}

void inter_sqlist::build_account(int size,string passwd)
{
    int i=0;
    while(true)
    {
        long long ac_id = rand_account(10);
        if(insert_ac_info(ac_id,passwd,"name:"+tos(ac_id)) == 1) i++;
        if(i >= size) break;
        usleep(1000);
    }
}

void inter_sqlist::build_friends()
{
    vector<long long> vec = select_account();

    for(auto a:vec)
    {
        for(auto b:vec)
        {
            insert_ac_friends(a,b);
        }
    }
}

long long inter_sqlist::rand_account(int count)
{
    long long account = 0;
    if(count < 1) return account;

    srand(time(NULL));
    while(true)
    {
        char buf[count + 1];
        memset(buf,0,sizeof (buf));

        for(int i=0;i<count;i++) buf[i] = '0' + rand() % 10;

        if(buf[0] != '0')
        {
            account = atoll(buf);
            break;
        }
    }
    return account;
}

inter_sqlist::ct_ac_info inter_sqlist::select_info(long long account)
{
    ct_ac_info ct;
    ct.is_null = true;

    auto callback = [](void *NotUsed, int argc, char **argv, char **azColName){
        if(argc != 3) return -1;
        ct_ac_info *pct = (ct_ac_info*)NotUsed;
        (*pct).is_null = false;
        (*pct).account = frs<long long>(argv[0]);
        (*pct).passwd = tos(argv[1]);
        (*pct).name = tos(argv[2]);
        return 0;
    };

    char buf[1024];
    snprintf(buf,sizeof(buf),"select * from %s where ac_id==%lld;",
             table_ac_info.c_str(),account);
    sqlite3_exec(v_db,buf,callback,&ct,NULL);
    return ct;
}


