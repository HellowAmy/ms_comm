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
    //加载账号数据
    file_account = "../data/account_info.db";
    if(account_db.open_db(file_account) == false)
    { vloge("load_account error: " vv(file_account)); };

    //===== 添加任务函数到容器 =====
    MAP_TASK_ADD(register);//账号注册
    MAP_TASK_ADD(login);//登录请求名
    MAP_TASK_ADD(logout);//登出请求
    MAP_TASK_ADD(recover_passwd);//忘记密码
    MAP_TASK_ADD(friends_list);//好友列表
    MAP_TASK_ADD(add_ret);//好友申请
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

    if(ct.type == en_mode::e_swap) //进入交换函数
    { task_swap(sock,meg); }
    else if(ct.type == en_mode::e_request) //进入应答函数
    {
        auto it_func = map_task_func.find(ct.func);
        if(it_func != map_task_func.end())
        { (std::bind(it_func->second,sock,meg))(); }
        else { vloge("not find func: e_request func"); }
    }
    else //未发现协议
    { vloge("on_message not find: " vv(ct.type) vv(ct.func)); }
}

void ux_web_server::on_close(const web_sock &sock)
{
    vlogd("on_close");
    if(move_connect(sock) == false)
    { vlogw("move failed: " vv((map_connect_rm.find(sock))->second)); };
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
        else vloge("find err: connect map number not equals");
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
    vlogd(vv(ct.passwd) vv(ct.name));
    if(add_to_account(to_string(ct.passwd),to_string(ct.name),ct_back.account))
    {
        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
        ct_back.is_success = true;              
    }
    else
    { ct_back.is_success = false; vlogw("add_to_account err"); }
    vlogd("task_register: " vv(ct_back.account) vv(ct.name) vv(ct_back.is_success));
);

//登入
BUILD_TASK_BACK_PO(login,
    ct_back.account = ct.account;
    ct_back.is_success = false;

    inter_sqlist::ct_ac_info temp = account_db.select_info(ct.account);
    if(temp.is_null == false)
    {
        //判断密码相等并成功加入连接队列则返回成功
        if(s_equals(temp.passwd,ct.passwd)
                && add_to_connect(ct.account,sock))
        { ct_back.is_success = true; }
    }
    else vlogw("not find: " vv(ct.account));
    vlogd("login: " vv(ct.account) vv(ct_back.is_success));
);

//登出
BUILD_TASK_BACK_PO(logout,
    if(move_connect(ct.account))
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
    }
    else { ct_back.is_success = false; }
    vlogd("logout: " vv(ct_back.is_success));
);

//忘记密码
BUILD_TASK_BACK_PO(recover_passwd,
    inter_sqlist::ct_ac_info temp = account_db.select_info(ct.account);
    if(temp.is_null == false)
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
        strncpy(ct_back.passwd,temp.passwd.c_str(),sizeof(ct_back.passwd));
    }
    vlogd("recover_passwd: " vv(ct_back.account) vv(ct_back.is_success));
);

//好友列表
BUILD_TASK_BACK_PO(friends_list,

    //连续发送: n-1 (总数减1)
    vector<long long> vec = account_db.select_friends(ct.account);
    ct_back.is_end = false;
    for(size_t i=0;i<vec.size();i++)
    {
        inter_sqlist::ct_ac_info info = account_db.select_info(vec[i]);
        if(info.is_null == false)
        {
            strncpy(ct_back.name,info.name.c_str(),sizeof(ct_back.name));
            ct_back.account = vec[i];
            if(send_str(sock,to_str(ct_back)) == false)
            { vloge("send_str err"); }
        }
    }
    ct_back.is_end = true;//发送结束标记
);

//好友申请
BUILD_TASK_BACK_PO(add_ret,
    ct_back.account_from = ct.account_from;
    ct_back.is_agree = ct.is_agree;
    ct_back.is_self = false;

    //插入数据库--这里应该启用数据库的事务以防止插入失败时回滚
    auto it = map_connect.find(ct.account_to);
    if(ct.is_agree && (it != map_connect.end()))
    {
        if(account_db.insert_ac_friends(ct.account_from,ct.account_to) == false)
        { vloge("insert_ac_friends err"); }

        if(account_db.insert_ac_friends(ct.account_to,ct.account_from) == false)
        { vloge("insert_ac_friends err"); }
    }

    //同时反馈到申请方（双方都会接收到消息并更新好友）
    if(send_str(it->second,to_str(ct_back)) == false)
    { vlogw("send_str err"); }
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
            ct_back.err = en_swap_error::e_error_swap;
            ct_back.swap_func = ct.func;
            ct_back.account_from = ct.account_to;
            if(send_str(sock,to_str(ct_back)) == false)
            { vloge("send_str err: en_swap_error"); }
        }
    }
    else
    {
        MAKE_CT_SWAP(ct_back,swap_error,ct.account_to);
        ct_back.err = en_swap_error::e_error_disconnect;
        ct_back.swap_func = ct.func;
        ct_back.account_from = ct.account_to;
        if(send_str(sock,to_str(ct_back)) == false)
        { vloge("send_str err: en_swap_error"); }
        vlogw("task_swap not find" vv(ct.account_to) vv(ct.func));
    }
}

//== 加入连接队列 ==
bool ux_web_server::add_to_connect(const long long &account, const web_sock &sock)
{
    std::unique_lock<mutex> lock(lock_connect);
    bool ok1 = ux_manage::add_connect(map_connect,account,sock);
    bool ok2 = ux_manage::add_connect(map_connect_rm,sock,account);
    return (ok1 && ok2);
}

//注册账号加入数据库
bool ux_web_server::add_to_account
    (const std::string &passwd,const std::string &name,long long &ret_account)
{
    std::unique_lock<mutex> lock(lock_account);
    for(int i=0;i<1000;i++)
    {
        ret_account = ux_manage::rand_account();
        if(account_db.insert_ac_info(ret_account,passwd,name))
        { return true; }
    }
    return false;
}
//== 加入连接队列 ==

//发送数据
bool ux_web_server::send_str(const web_sock &sock, const std::string &str)
{
    std::unique_lock<mutex> lock(lock_send);
    sock->send(str.c_str(),str.size());
    return sock->isConnected();
}
