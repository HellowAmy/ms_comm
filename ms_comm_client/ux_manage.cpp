#include "ux_manage.h"

ux_manage::ux_manage()
{

}

long long ux_manage::rand_account(int count)
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

bool ux_manage::save_account
    (const std::string &filename,
     const map<long long, std::string> &map_account)
{
    bool is_ok = false;

    fstream ofs(filename,ios::out);
    if(ofs.is_open())
    {
        for(const auto &a:map_account)
        {
            ofs<<a.first<<a.second<<std::endl;
        }
        is_ok = true;
        ofs.close();
    }
    return is_ok;
}

bool ux_manage::load_account
    (const string &filename,
     map<long long, string> &map_account, int line_end)
{
    bool is_ok = false;

    fstream ofs(filename,ios::in);
    if(ofs.is_open())
    {
        string buf;
        while(getline(ofs,buf))
        {
            string buf_account(buf.begin(),buf.begin()+line_end);
            string buf_passwd(buf.begin()+line_end,buf.end());

            long long account = atoll(buf_account.c_str());
            map_account.insert(pair<long long ,string>(account,buf_passwd));
        }

        is_ok = true;
        ofs.close();
    }
    return is_ok;
}

long long ux_manage::add_account
    (map<long long, std::string> &map_account,string passwd)
{
    long long account = 0;
    for(int i=0;i<100;i++)
    {
        account = ux_manage::rand_account();
        auto it = map_account.insert(pair<long long ,string>(account,passwd));
        if(it.second == true) break;
    }
    return account;
}

bool ux_manage::add_account
    (map<long long, std::string> &map_account,
     const std::string &passwd, long long &ret_account)
{
    for(int i=0;i<100;i++)
    {
        ret_account = ux_manage::rand_account();
        auto it = map_account.insert(pair<long long ,string>(ret_account,passwd));
        if(it.second == true) return true;
    }
    return false;
}



