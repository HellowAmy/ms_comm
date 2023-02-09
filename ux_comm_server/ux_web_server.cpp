#include "ux_web_server.h"

ux_web_server::ux_web_server()
{
    sock.func_bind(bind(&ux_web_server::on_open,this,_1,_2),
                   bind(&ux_web_server::on_message,this,_1,_2),
                   bind(&ux_web_server::on_close,this,_1));

    //加载登录账号文件
    file_account = "../data/account.txt";
    if(ux_manage::load_account(file_account,map_account) == false)
        { vloge("load_account error: " vv(file_account)); };

    //===== 添加任务函数到容器 =====
    map_task_add(e_register,task_register);//账号注册
    map_task_add(e_login,task_login);//登录请求
    map_task_add(e_logout,task_logout);//登出请求
    map_task_add(e_recover_passwd,task_recover_passwd);//忘记密码
    //===== 添加任务函数到容器 =====
}

int ux_web_server::open_server(int port, int th_count)
{
    return sock.open(port,th_count);
}

void ux_web_server::on_open(const web_sock &sock, const web_http &http)
{
    vlogd("on_open" vv(sock->id()) vv(http->Url()));
}

void ux_web_server::on_message(const web_sock &sock, const string &meg)
{
    //执行匹配的任务函数
    ct_head_mode ct;
    to_ct(meg,ct);
    if(ct.type == en_mode::e_request) //进入应答函数
    {
        auto it_func = map_task_func.find(ct.func);
        if(it_func != map_task_func.end())
        { (std::bind(it_func->second,sock,meg))(); }
        else { vloge("it_func not find: e_request"); }
    }
    else if(ct.type == en_mode::e_swap) //进入交换函数
    { task_swap(sock,meg); }
    else //未发现协议
    { vloge("on_message not find: " vv(ct.type) vv(ct.func)); }
}

void ux_web_server::on_close(const web_sock &sock)
{
    vlogd("on_close");
    if(move_connect(sock) == false)
    { vloge("move failed: " vv((map_connect_rm.find(sock))->second)); };
}

bool ux_web_server::move_connect(const web_sock &sock)
{
    unique_lock<mutex> lock(lock_connect);
    bool is_ok = false;
    auto it = map_connect_rm.find(sock);
    if(it != map_connect_rm.end())
    {
        auto it_ac = map_connect.find(it->second);
        if(it_ac != map_connect.end())
        {
            vlogd("move account: " vv(it_ac->first));
            map_connect.erase(it_ac);
            map_connect_rm.erase(it);
            is_ok = true;
            vlogd(vv(map_connect.size()) vv(map_connect_rm.size()));
        }
    }
    return is_ok;
}

bool ux_web_server::move_connect(const long long &account)
{
    std::unique_lock<mutex> lock(lock_connect);
    bool is_ok = false;
    auto it = map_connect.find(account);
    if(it != map_connect.end()) { map_connect.erase(it); is_ok = true; }
    return is_ok;
}

void ux_web_server::task_register(const web_sock &sock, const std::string &meg)
{
    vlogf("task_register");
    ct_register ct;
    MAKE_CT_RES(ct_back,register_back);

    //账号插入到存在容器，并返回账号
    to_ct(meg,ct);
    if(add_to_account(ct.passwd,ct_back.account))
    {
        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
        ct_back.is_success = ux_manage::save_account(file_account,map_account);
    }
    else
    { ct_back.is_success = false; vloge("add_to_account err"); }

    //反馈到客户端
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }
}

void ux_web_server::task_login(const web_sock &sock, const std::string &meg)
{
    vlogf("task_login");
    ct_login ct;
    MAKE_CT_RES(ct_back,login_back);

    to_ct(meg,ct);
    ct_back.account = ct.account;
    ct_back.is_success = false;

    //添加到连接队列
    auto it = map_account.find(ct.account);
    if(it != map_account.end())
    {
        if(s_equals(it->second,ct.passwd) && add_to_connect(ct.account,sock))
        { ct_back.is_success = true; }
    }
    else vlogw("not find: " vv(ct.account));

    //反馈到客户端
    vlogf("back: " vv(ct_back.account) vv(ct_back.is_success) vv(map_connect.size()));
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }
    vlogf("task_login:out");
}

void ux_web_server::task_logout(const web_sock &sock, const std::string &meg)
{
    vlogf("task_logout");
    ct_logout ct;
    MAKE_CT_RES(ct_back,logout_back);

    //账号移除连接容器
    to_ct(meg,ct);
    if(move_connect(ct.account))
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
    }
    else { ct_back.is_success = false; }

    //反馈到客户端
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }
}

void ux_web_server::task_recover_passwd(const web_sock &sock, const std::string &meg)
{
    vlogf("task_recover_passwd");
    ct_recover_passwd ct;
    MAKE_CT_RES(ct_back,recover_passwd_back);

    to_ct(meg,ct);
    auto it = map_account.find(ct.account);
    if(it != map_account.end())
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
        strncpy(ct_back.passwd,it->second.c_str(),sizeof(it->second.size()));
    }

    //反馈到客户端
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }
}

void ux_web_server::task_swap(const web_sock &sock, const std::string &meg)
{
    //转发数据，如果转发失败则判断失败转发对应函数
    ct_head_swap ct;
    MAKE_CT_RES(ct_back,disconnect);
    to_ct(meg,ct);

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end()) //连接查找
    {
        //转发数据,如果转发失败则反馈到数据发送者
        if(send_str(it->second,meg) == false)
        {
            ct_back.head_info.type = ct.type;
            ct_back.head_info.func = ct.func;
            ct_back.account_to = ct.account_to;
            if(send_str(sock,to_str(ct_back)))
            { vloge("send_str to back: err" vv(ct.func)); };

            vlogw("send_str: err" vv(ct.func));
        }
    }
    else vlogw("task_swap not find" vv(meg));
}

bool ux_web_server::add_to_connect(const long long &account, const web_sock &sock)
{
    std::unique_lock<mutex> lock(lock_connect);
    bool ok1 = ux_manage::add_connect(map_connect,account,sock);
    bool ok2 = ux_manage::add_connect(map_connect_rm,sock,account);
    return (ok1 && ok2);
}

//void ux_web_server::remove_connect(map<long long,web_sock>::iterator it)
//{  std::unique_lock<mutex> lock(lock_connect); map_connect.erase(it); }

bool ux_web_server::add_to_account(const std::string &passwd, long long &ret_account)
{
    std::unique_lock<mutex> lock(lock_account);
    return ux_manage::add_account(map_account,passwd,ret_account);
}

bool ux_web_server::send_str(const web_sock &sock, const std::string &str)
{
    std::unique_lock<mutex> lock(lock_send);
    sock->send(str.c_str(),str.size());
    return sock->isConnected();
}
