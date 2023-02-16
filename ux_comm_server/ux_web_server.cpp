#include "ux_web_server.h"

//===== 添加任务函数到容器 =====
//==原型:
//map_task_func.insert(pair<en_mode_index,func_task>
//    (en_mode_index::e_login,bind(&ux_web_server::task_login,this,_1,_2)));
//===== 添加任务函数到容器 =====

#define MAP_TASK_ADD(func) \
    map_task_func.insert(pair<en_mode_index,func_task> \
        (e_##func,bind(&ux_web_server::task_##func,this,_1,_2)))
//===== 快速建立 =====

ux_web_server::ux_web_server()
{
    //绑定网络回调函数
    sock.func_bind(bind(&ux_web_server::on_open,this,_1,_2),
                   bind(&ux_web_server::on_message,this,_1,_2),
                   bind(&ux_web_server::on_close,this,_1));

    //加载登录账号文件
    file_account = "../data/account.txt";
    if(ux_manage::load_account(file_account,map_account) == false)
        { vloge("load_account error: " vv(file_account)); };

    //===== 添加任务函数到容器 =====
    MAP_TASK_ADD(register);//账号注册
    MAP_TASK_ADD(login);//登录请求
    MAP_TASK_ADD(logout);//登出请求
    MAP_TASK_ADD(recover_passwd);//忘记密码
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
    vlogd("on_message: " vv(ct.func));

    if(ct.type == en_mode::e_swap) //进入交换函数
    { task_swap(sock,meg); }
    else if(ct.type == en_mode::e_request) //进入应答函数
    {
        auto it_func = map_task_func.find(ct.func);
        if(it_func != map_task_func.end())
        { (std::bind(it_func->second,sock,meg))(); }
        else { vloge("it_func not find: e_request"); }
    }
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



//===== 反馈函数快速宏 =====
//== 原型 ==
//void ux_web_server::task_register(const web_sock &sock, const std::string &meg)
//{
//    vlogf("task_register");
//    ct_register ct;
//    MAKE_CT_RES(ct_back,register_back);//生成反馈函数
//
//    //== 函数体 ==
//    //账号插入到存在容器，并返回账号
//    to_ct(meg,ct);
//    if(add_to_account(ct.passwd,ct_back.account))
//    {
//        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
//        ct_back.is_success = ux_manage::save_account(file_account,map_account);
//    }
//    else
//    { ct_back.is_success = false; vloge("add_to_account err"); }
//    //== 函数体 ==
//
//    //反馈到客户端
//    if(send_str(sock,to_str(ct_back)) == false)
//    { vloge("send_str err"); }
//}
//== 原型 ==

#define BUILD_TASK_BACK_PO(func,...)                    \
void ux_web_server::task_##func                         \
        (const web_sock &sock, const std::string &meg)  \
{                                                       \
    vlogf("task_"#func);                                \
    ct_##func ct;                                       \
    MAKE_CT_RES(ct_back,func##_back);                   \
    to_ct(meg,ct);                                      \
    __VA_ARGS__                                         \
    if(send_str(sock,to_str(ct_back)) == false)         \
    { vloge("send_str err"); }                          \
}
//== 快速宏 ==

//注册
BUILD_TASK_BACK_PO(register,
    if(add_to_account(ct.passwd,ct_back.account))
    {
        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
        ct_back.is_success = ux_manage::save_account(file_account,map_account);
    }
    else
    { ct_back.is_success = false; vloge("add_to_account err"); }
);

//注册
BUILD_TASK_BACK_PO(login,
    ct_back.account = ct.account;
    ct_back.is_success = false;
    auto it = map_account.find(ct.account);//添加到连接队列
    if(it != map_account.end())
    {
        if(s_equals(it->second,ct.passwd) && add_to_connect(ct.account,sock))
        { ct_back.is_success = true; }
    }
    else vlogw("not find: " vv(ct.account));
);

//登出
BUILD_TASK_BACK_PO(logout,
    if(move_connect(ct.account))
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
    }
    else { ct_back.is_success = false; }
);

//忘记密码
BUILD_TASK_BACK_PO(recover_passwd,
    auto it = map_account.find(ct.account);
    if(it != map_account.end())
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
        strncpy(ct_back.passwd,it->second.c_str(),sizeof(it->second.size()));
    }
);
//===== 反馈函数快速宏 =====


//交换函数
void ux_web_server::task_swap(const web_sock &sock, const std::string &meg)
{
    //转发数据，如果转发失败则判断失败转发对应函数
    ct_head_swap ct;
    to_ct(meg,ct);

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end()) //连接查找
    {
        //转发数据,如果转发失败则反馈到数据发送者
        if(send_str(it->second,meg) == false)
        {
            vloge("send_str err: " vv(ct.func));
            MAKE_CT_SWAP(ct_back,swap_error,ct.account_to);
            ct_back.err = en_swap_error::e_error_disconnect;
            ct_back.account_from = ct.account_to;
            if(send_str(sock,meg) == false) { vloge("send_str err: en_swap_error"); }
        }
    }
    else vlogw("task_swap not find" vv(meg));
}

//== 加入连接队列 ==
bool ux_web_server::add_to_connect(const long long &account, const web_sock &sock)
{
    std::unique_lock<mutex> lock(lock_connect);
    bool ok1 = ux_manage::add_connect(map_connect,account,sock);
    bool ok2 = ux_manage::add_connect(map_connect_rm,sock,account);
    return (ok1 && ok2);
}

bool ux_web_server::add_to_account(const std::string &passwd, long long &ret_account)
{
    std::unique_lock<mutex> lock(lock_account);
    return ux_manage::add_account(map_account,passwd,ret_account);
}
//== 加入连接队列 ==

//发送数据
bool ux_web_server::send_str(const web_sock &sock, const std::string &str)
{
    std::unique_lock<mutex> lock(lock_send);
    sock->send(str.c_str(),str.size());
    return sock->isConnected();
}
