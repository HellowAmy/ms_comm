#include "ux_web_server.h"

ux_web_server::ux_web_server()
{
    sock.func_bind(bind(&ux_web_server::on_open,this,_1,_2),
                   bind(&ux_web_server::on_message,this,_1,_2),
                   bind(&ux_web_server::on_close,this,_1));

//    void on_open(const web_sock& sock, const web_http& http) override;
//    void on_message(const web_sock& sock, const string& meg) override;
//    void on_close(const web_sock& sock) override;

//    vinit_c(vlog::e_info);//日志初始化

    //加载登录账号文件
    file_account = "../data/account.txt";
    if(ux_manage::load_account(file_account,map_account) == false)
        { vloge("load_account error: " vv(file_account)); };

//    map_add();

    //===== 添加任务函数到容器 =====
    map_task_add(e_register,task_register);//账号注册
    map_task_add(e_login,task_login);//登录请求
    map_task_add(e_logout,task_logout);//登出请求
    map_task_add(e_recover_passwd,task_recover_passwd);//忘记密码
    //===== 添加任务函数到容器 =====


//    e_register,             //注册请求-s
//          e_register_back,        //注册任务反馈-c
//    e_login,                //登录请求-s
//          e_login_back,           //登录请求反馈-c
//    e_logout,               //登出请求-s
//          e_logout_back,          //登出请求反馈-c
//    e_recover_passwd,       //找回密码-s
//          e_recover_passwd_back,  //找回密码反馈-c
//    e_disconnect_txt,       //目标账号未连接--发送文字-s
//    e_disconnect_file,      //目标账号未连接--发送文件-s
//          e_swap_txt,             //交换文字-c2
//          e_swap_file_build,      //建立文件-c2
//          e_swap_file_build_err,  //建立文件错误-c1
//          e_swap_file_send,       //发送文件段-c2
//          e_swap_file_send_err,   //发送文件错误-c1
//          e_swap_file_ret_err,    //接收文件完整性错误-c1
//          e_swap_file_request,    //发送文件段请求-c2








//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//             (e_register,bind(&ux_web_server::task_register,this,_1,_2)));
//    //登录请求
//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//             (e_login,bind(&ux_web_server::task_login,this,_1,_2)));

//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//                    (e_error,bind(&ux_web_server::task_error,this,_1,_2)));

//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//             (e_swap_txt,bind(&ux_web_server::task_swap_txt,this,_1,_2)));

//    map_func.insert(pair<en_mode_index,function<void(const web_sock&, const string&)>>
//                    (e_swap_file,bind(&ux_web_server::task_swap_file,this,_1,_2)));


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








//    enum_transmit cmd = *(enum_transmit*)
//            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
//    auto it_find = map_func.find(cmd);
//    if(it_find != map_func.end())
//    {
//        (std::bind(it_find->second,sock,meg))();//执行任务
//    }
//    else vlogw("map_func not find" v(cmd));
}

void ux_web_server::on_close(const web_sock &sock)
{
    move_connect();
}

void ux_web_server::move_connect()
{
//    vlevel(vlog::e_info,vlog::e_info);
    unique_lock<mutex> lock(lock_connect);
    for_it(it,map_connect)
    {
        if(it->second != nullptr)
        {
            if(it->second->isConnected() == false)
                { map_connect.erase(it); break; }
        }
        else map_connect.erase(it);
    }
    //    vlogd("map_connect size: " v(map_connect.size()));
}

bool ux_web_server::move_map_connect(const long long &account)
{
    auto it = map_connect.find(account);
    if(it != map_connect.end()) { map_connect.erase(it); return true; }
    return false;
}



void ux_web_server::task_register(const web_sock &sock, const std::string &meg)
{
    vlogf("task_register");
    ct_register ct;
//    ct_register_back ct_back;
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





//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_register>(meg,cmd,ct);

//    //add_account: 账号插入到容器，并返回账号
//    ct_register_back ct_back;
//    unique_lock<mutex> lock(lock_account);
//    if(ux_manage::add_account(map_account,ct.passwd,ct_back.account))
//    {
//        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
//        ct_back.flg = ux_manage::save_account(file_account,map_account);
//    }
//    else ct_back.flg = 0;
//    vlogf("ct_back: " v(ct_back.passwd) v(ct_back.account));

//    //反馈到客户端
//    string str = ux_manage::to_str<enum_transmit,ct_register_back>
//            (enum_transmit::e_register_back,ct_back);
//    if(send_back(sock,str) == -1) vlogw("send to client error");
}

void ux_web_server::task_login(const web_sock &sock, const std::string &meg)
{
    vlogf("task_login");
    ct_login ct;
//    ct_login_back ct_back;
    MAKE_CT_RES(ct_back,login_back);


    //添加到连接队列
    to_ct(meg,ct);
    auto it = map_account.find(ct.account);
    if(it != map_account.end())
    {
        if(it->second == string(ct.passwd,sizeof(ct.passwd))
                && add_to_connect(ct.account,sock))
        {
            ct_back.account = ct.account;
            ct_back.is_success = true;
        }
        else { ct_back.is_success = false; };
    }

    //反馈到客户端
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }








//    ct_login ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_login>(meg,cmd,ct);
//    vlogf("task_login: " v(ct.account) v(ct.passwd));

//    //添加到连接队列
//    ct_login_back ct_back;
//    unique_lock<mutex> lock(lock_connect);

//    if(map_account.find(ct.account) != map_account.end()
//            && ux_manage::add_connect<web_sock>(map_connect,ct.account,sock))
//    { ct_back.flg = 1; }
//    else ct_back.flg = 0;

//    //反馈到客户端
//    string str = ux_manage::to_str<enum_transmit,ct_login_back>
//            (enum_transmit::e_login_back,ct_back);
//    if(send_back(sock,str) == -1) vlogw("send to client error");
    //    vlogd("map_connect size: " v(map_connect.size()));
}

void ux_web_server::task_logout(const web_sock &sock, const std::string &meg)
{
    vlogf("task_logout");
    ct_logout ct;
//    ct_logout_back ct_back;
    MAKE_CT_RES(ct_back,logout_back);

    //账号移除连接容器
    to_ct(meg,ct);
    if(move_map_connect(ct.account))
    {
        ct_back.account = ct.account;
        ct_back.is_success = true;
    }
    else { ct_back.is_success = false; }

    //反馈到客户端
    if(send_str(sock,to_str(ct_back)) == false)
    { vloge("send_str err"); }









//    //添加到连接队列
//    to_ct(meg,ct);
//    auto it = map_account.find(ct.account);
//    if(it != map_account.end())
//    {
//        if(it->second == string(ct.passwd) && add_to_connect(ct.account,sock))
//        {
//            ct_back.account = ct.account;
//            ct_back.is_success = true;
//        }
//        else { ct_back.is_success = false; };
//    }

//    //反馈到客户端
//    if(send_str(sock,to_str(ct_back)) == false)
//    { vloge("send_str err"); }


}

void ux_web_server::task_recover_passwd(const web_sock &sock, const std::string &meg)
{
    vlogf("task_recover_passwd");
    ct_recover_passwd ct;
//    ct_recover_passwd_back ct_back;
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







//    vlogf("task_logout");
//    ct_login ct;
//    ct_login_back ct_back;

//    //账号移除连接容器
//    to_ct(meg,ct);
//    if(move_map_connect(ct.account))
//    {
//        ct_back.account = ct.account;
//        ct_back.is_success = true;
//    }
//    else { ct_back.is_success = false; }

//    //反馈到客户端
//    if(send_str(sock,to_str(ct_back)) == false)
//    { vloge("send_str err"); }


}

void ux_web_server::task_swap(const web_sock &sock, const std::string &meg)
{
    //转发数据，如果转发失败则判断失败转发对应函数
    ct_head_swap ct;
//    ct_disconnect ct_back;
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
    return ux_manage::add_connect<web_sock>(map_connect,account,sock);
}

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

//void ux_web_server::task_error(const web_sock &sock, const std::string &meg)
//{

//}

//void ux_web_server::task_swap_txt(const web_sock &sock, const std::string &meg)
//{
//    ct_swap_txt ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);
//    vlogf("task_swap_txt: " v(ct.account_to) v(ct.account_from) v(ct.data));

//    auto it = map_connect.find(ct.account_to);
//    if(it != map_connect.end())
//    {
//        if(send_back(it->second,meg) == -1)
//        {
//            move_connect();
//            vlogw("send to client error");
//        }
//    }
//    else vlogw("task_swap_txt: map_connect not find");
//}

//void ux_web_server::task_swap_file(const web_sock &sock, const std::string &meg)
//{
//    vlevel(vlog::e_warning,vlog::e_warning);
//    ct_swap_file ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);
//    vlogf("task_swap_file: " v(ct.account_to) v(ct.account_from));

//    auto it = map_connect.find(ct.account_to);
//    if(it != map_connect.end())
//    {
//        if(send_back(it->second,meg) == -1)
//        {
//            move_connect();
//            vloge("send to client error: " v(ct.account_to) v(ct.account_from));
//        }
//    }
//    vlevel(vlog::e_info,vlog::e_info);
//}

//int ux_web_server::send_back(const web_sock &sock,const std::string &str)
//{
//    std::unique_lock<mutex> lock(lock_send);
//    int ret = -1;
//    if(sock->isConnected()) ret = sock->send(str.c_str(),str.size());
//    else vlogw("isConnected false");
//    return ret;
//}
