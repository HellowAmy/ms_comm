#include "ms_web_client.h"

ms_web_client::ms_web_client()
{
//    vinit_c(vlog::e_info);

    set_file_path();

    //===== 添加任务函数到容器 =====
//    map_task_add(e_register_back,back_register);

//e_register_back

    //===== 添加任务函数到容器 =====
//    map_task_add(e_register_back,back_register);//账号注册
//    map_task_add(e_login_back,back_login);//登录请求
//    map_task_add(e_logout_back,back_logout);//登出请求
//    map_task_add(e_recover_passwd_back,back_recover_passwd);//忘记密码 e_disconnect
//    map_task_add(e_disconnect,back_recover_passwd);//忘记密码


//    map_task_add(register_back);



    //===== 添加任务函数到容器 =====
    map_task_add(register_back);        //注册任务反馈-c
    map_task_add(login_back);           //登录请求反馈-c
    map_task_add(logout_back);          //登出请求反馈-c
    map_task_add(recover_passwd_back);  //找回密码反馈-c
    map_task_add(disconnect_txt);       //目标账号未连接--发送文字-c
    map_task_add(disconnect_file);      //目标账号未连接--发送文件-c
    map_task_add(disconnect);           //转发未连接
    map_task_add(swap_txt);             //交换文字-c2
    map_task_add(swap_file_build);      //建立文件-c2
    map_task_add(swap_file_build_err);  //建立文件错误-c1
    map_task_add(swap_file_send);       //发送文件段-c2
    map_task_add(swap_file_send_err);   //发送文件错误-c1
    map_task_add(swap_file_ret_err);    //接收文件完整性错误-c1
    //===== 添加任务函数到容器 =====



    //    e_swap_txt,             //交换文字-c2
    //    e_swap_file_build,      //建立文件-c2
    //    e_swap_file_build_err,  //建立文件错误-c1
    //    e_swap_file_send,       //发送文件段-c2
    //    e_swap_file_send_err,   //发送文件错误-c1
    //    e_swap_file_ret_err,    //接收文件完整性错误-c1
    //    e_swap_file_request,    //发送文件段请求-c2

//    void swap_txt(const string& meg);               //交换文字-c2
//    void swap_file_build(const string& meg);        //建立文件-c2
//    void swap_file_build_err(const string& meg);    //建立文件错误-c1
//    void swap_file_send(const string& meg);         //发送文件段-c2
//    void swap_file_send_err(const string& meg);     //发送文件错误-c1
//    void swap_file_ret_err(const string& meg);      //接收文件完整性错误-c1


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




//    //账号注册
//    map_task_func.insert(pair<en_mode_index,function<void(const string&)>>
//             (e_register_back,bind(&ms_web_client::back_register,this,_1)));
//    //账号注册
//    map_func.insert(pair<enum_transmit,function<void(const string&)>>
//             (e_register_back,bind(&ms_web_client::back_register,this,_1)));
//    //登录请求
//    map_func.insert(pair<enum_transmit,function<void(const string&)>>
//             (e_login_back,bind(&ms_web_client::back_login,this,_1)));
//    //错误反馈
//    map_func.insert(pair<enum_transmit,function<void(const string&)>>
//             (e_error,bind(&ms_web_client::back_error,this,_1)));
//    //数据交换
//    map_func.insert(pair<enum_transmit,function<void(const string&)>>
//             (e_swap_txt,bind(&ms_web_client::back_swap_txt,this,_1)));
//    //文件交换
//    map_func.insert(pair<enum_transmit,function<void(const string&)>>
//             (e_swap_file,bind(&ms_web_client::back_swap_file,this,_1)));
    //===== 添加任务函数到容器 =====






}

void ms_web_client::set_file_path(std::string path)
{
    v_path_files = path;
}


//===== 快速建立请求函数 =====
//功能： 解析接收内容并触发func回调函数
//== 宏参考模板 ==
//void ms_web_client::ask_register(std::string passwd）//参数替换IN_ARGS
//{
//    vlogf("ask_register");
//    MAKE_CT_REQ(ct,register);

        //== 函数体 ==
//    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
        //== 函数体 ==

//    if(send_str(this->sock(),to_str(ct)))
//    { vlogf("send err: ask_register"); }
//}

//== 请求函数宏模板声明 ==
#define IN_ARGS(...) __VA_ARGS__
#define BUILD_CT_ASK_REQ(func_name,IN_ARGS,...)     \
void ms_web_client::ask_##func_name(IN_ARGS)        \
{                                                   \
    vlogd("ask_"#func_name);                        \
    MAKE_CT_REQ(ct,func_name);                      \
    __VA_ARGS__                                     \
    if(send_str(this->sock(),to_str(ct)))           \
    { vlogw("send err: ask_"#func_name); }          \
}

//== 模板使用 ==
BUILD_CT_ASK_REQ(register,IN_ARGS(std::string passwd),
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
);

BUILD_CT_ASK_REQ(login,IN_ARGS(long long account,std::string passwd),
    ct.account = account;
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
);

BUILD_CT_ASK_REQ(logout,IN_ARGS(long long account),
    ct.account = account;
);

BUILD_CT_ASK_REQ(recover_passwd,IN_ARGS(long long account),
                 ct.account = account;
);
//===== 快速建立请求函数 =====


void ms_web_client::ask_swap_txt(long long account_from, long long account_to, std::string txt)
{
    vlogd("ask_swap_txt");
    MAKE_CT_SWAP(ct,swap_txt,account_to);
    ct.account_from = account_from;
    strncpy(ct.buf_txt,txt.c_str(),sizeof(ct.buf_txt));

    if(send_str(this->sock(),to_str(ct)))
    { vlogw("send err: ask_swap_txt"); }
}

void ms_web_client::ask_swap_file(long long account_from, long long account_to, std::string filename)
{

}


////交换文字
//CT_BUILD_SWAP(ct_swap_txt,
//    long long account_from;
//    char buf_txt[1024];
//);

////快速声明结构体--交换
//#define MAKE_CT_SWAP(in_name,in_type,in_account)    \
//    ct_##in_type in_name;                           \
//    in_name.head.type = en_mode::e_swap;            \
//    in_name.head.func = en_mode_index::e_##in_type; \
//    in_name.head.account_to = in_account            \


//CT_BUILD_MODE(ct_register,
//    char passwd[64];
//);
//CT_BUILD_MODE(ct_login,
//    long long account;
//    char passwd[64];
//);
//CT_BUILD_MODE(ct_logout,
//    long long account;
//);
//CT_BUILD_MODE(ct_recover_passwd,
//    long long account;
//);



//int ms_web_client::ask_register(std::string passwd)
//{
//    vlogf("ask_register");
//    ct_register ct;
//    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
//    string str = ux_manage::to_str<enum_transmit,ct_register>
//            (enum_transmit::e_register,ct);
//    return send_meg(str);
//}

//int ms_web_client::ask_login(long long account, std::string passwd)
//{
//    vlogf("ask_login");
//    ct_login ct;
//    ct.account = account;
//    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
//    string str = ux_manage::to_str<enum_transmit,ct_login>
//            (enum_transmit::e_login,ct);
//    return send_meg(str);
//}

//int ms_web_client::ask_swap_txt
//    (long long account_from, long long account_to, std::string txt)
//{
//    vlogf("ask_swap_txt");
//    ct_swap_txt ct;
//    ct.account_from = account_from;
//    ct.account_to = account_to;
//    strncpy(ct.data,txt.c_str(),sizeof(ct.data));
//    string str = ux_manage::to_str<enum_transmit,ct_swap_txt>
//            (enum_transmit::e_swap_txt,ct);
//    return send_meg(str);
//}

//void ms_web_client::ask_swap_file
//    (long long account_from,long long account_to,std::string filename)
//{
////    vlogf("ask_swap_file");
//    vlogf(v(account_from) v(account_to) v(filename));
//    auto func = [=](long long account_from,long long account_to,std::string filename)
//    {
//        vlogf("func:in");
//        bool is_err = false;
//        ct_swap_file ct;
//        ct.account_from = account_from;
//        ct.account_to = account_to;

//        fstream ofs(filename,ios::in | ios::binary);
//        if(ofs.is_open())
//        {
//            vlogd("is_open");
//            ofs.seekg(0,ios::end);
//            ct.lenght = ofs.tellg();
//            ofs.seekg(0,ios::beg);

//            string temp = vts::stm(filename)("\\",-1,-1);
//            strncpy(ct.filename,temp.c_str(),sizeof(ct.filename));

//            long long size_sum = 0;
//            while(ofs.eof() == false && this->sock().isConnected())
//            {
//                //发送文件时状态
//                if(ofs.tellg() == 0) ct.status = 0;
//                else ct.status = 1;

//                ofs.read(ct.buf,sizeof(ct.buf));
//                ct.buf_size = ofs.gcount();//记录发送真实字节
//                size_sum += ofs.gcount();//累计发送字节
//                if(size_sum == ct.lenght) ct.status = 2;//最后一次发送

//                string str = ux_manage::to_str<enum_transmit,ct_swap_file>
//                        (enum_transmit::e_swap_file,ct);
//                int send_size = send_meg(str);
//                if(send_size == 0)
//                {
//                    size_t size_now = this->sock().channel->writeBufsize();
//                    if(size_now > WRITE_BUFSIZE_HIGH_WATER)
//                    {
//                        for(int i=0;i<1000000;i++)
//                        {
//                            if(this->sock().isConnected() == false)
//                                { is_err = true; break; }

//                            size_t temp = this->sock().channel->writeBufsize();
//                            if(temp < WRITE_BUFSIZE_HIGH_WATER/8)
//                                { vlogw("break high line"); break; }
//                            else if(temp > WRITE_BUFSIZE_HIGH_WATER*2)
//                                { vlogw("out high line"); }
//                        }
//                    }
//                }
//                else if(send_size == -1) { vlogw("send flase"); is_err = true; return; }
//            }
//            ofs.close();
//            vlogw("close: " v(is_err));
//        }
//        else vlogw("open false: " v(filename));
//    };

//    std::thread th(func,account_from,account_to,filename);
//    th.detach();
//    vlogd("thread detach: out");
//}

//bool ms_web_client::is_connect()
//{
//    return this->sock().isConnected();
//}

//===== 快速定义反馈函数 =====
//功能： 解析接收内容并触发func回调函数
#define BUILD_CT_BACK_FUNC_RES(func_name,...)               \
void ms_web_client::func_name(const std::string &meg)       \
{                                                           \
    vlogd(#func_name);                                      \
    ct_##func_name ct;                                      \
    to_ct(meg,ct);                                          \
    if(func_##func_name) func_##func_name(__VA_ARGS__);     \
}                                                           \

BUILD_CT_BACK_FUNC_RES(register_back,
    ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);

BUILD_CT_BACK_FUNC_RES(login_back,
    ct.account,ct.is_success);

BUILD_CT_BACK_FUNC_RES(logout_back,
    ct.account,ct.is_success);

BUILD_CT_BACK_FUNC_RES(recover_passwd_back,
    ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);

BUILD_CT_BACK_FUNC_RES(disconnect,
    ct.account_to,ct.head_info);

BUILD_CT_BACK_FUNC_RES(swap_txt,
    ct.account_from,string(ct.buf_txt,sizeof(ct.buf_txt)));
//===== 快速定义反馈函数 =====

//void ms_web_client::register_back(const std::string &meg)
//{
//    vlogf("register_back");
//    ct_register_back ct;
//    to_ct(meg,ct);

//    if(func_register_back) func_register_back
//            (ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);
//}

//void ms_web_client::login_back(const std::string &meg)
//{
//    vlogf("login_back");
//    ct_login_back ct;
//    to_ct(meg,ct);

//    if(func_login_back) func_login_back(ct.account,ct.is_success);
//}

//void ms_web_client::logout_back(const std::string &meg)
//{
//    vlogf("login_back");
//    if(func_swap) func_swap(meg);
//}

void ms_web_client::on_open()
{
    vlogf("on_open");
    if(func_open) func_open();
}

void ms_web_client::on_message(const std::string &meg)
{
    //执行匹配的任务函数
    ct_head_mode ct;
    to_ct(meg,ct);
    auto it_func = map_task_func.find(ct.func);
    if(it_func != map_task_func.end())
    { (std::bind(it_func->second,meg))(); }


//    else { vloge("it_func not find: e_response"); }
//    if(ct.type == en_mode::e_response) //进入应答函数
//    {

//    }


//    else if(ct.type == en_mode::e_swap) //进入交换函数
//    { task_swap(sock,meg); }
//    else //未发现协议
//    { vloge("on_message not find: " vv(meg)); }


//    vlevel(vlog::e_warning,vlog::e_warning);
//    vlogf("on_message: " v(std::this_thread::get_id()));
//    vlogw(v(std::this_thread::get_id()));

    //执行匹配的任务函数
//    enum_transmit cmd = *(enum_transmit*)
//            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
//    auto it_find = map_func.find(cmd);
//    if(it_find != map_func.end())
//    {
//        vlogf("on_message type: " v(cmd));
//        (std::bind(it_find->second,meg))();
//    }
//    else vlogw("on_message:not find");
}

void ms_web_client::on_close()
{
//    for_it(it,map_ofs) { it->second->close(); }
    vlogw("on_close");
    if(func_close) func_close();
}

//int ms_web_client::send_meg(const std::string &meg)
//{
//    int ret = 0;
//    if(this->sock().isConnected()) ret = this->sock().send(meg.c_str(),meg.size());
//    else { vlogw("isConnected false"); ret = -1; }
//    return ret;
//}

bool ms_web_client::write_file(shared_ptr<std::fstream> sp_ofs, const char *buf, int size)
{
    sp_ofs->write(buf,size);
    if(sp_ofs->bad()) { vloge("write_err");  return false; }//sp_ofs->close();
    else return true;
}

//bool ms_web_client::send_str(const web_sock &sock, const std::string &str)
//{
//    std::unique_lock<mutex> lock(lock_send);
//    sock->send(str.c_str(),str.size());
//    return sock->isConnected();
//}

//void ms_web_client::back_register(const std::string &meg)
//{
//    vlogf("back_register");
//    ct_register_back ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_register_back>(meg,cmd,ct);
//    if(func_register_back) func_register_back(ct.account,ct.passwd,ct.flg);
//}

//void ms_web_client::back_login(const std::string &meg)
//{
//    vlogf("back_login");
//    ct_login_back ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_login_back>(meg,cmd,ct);
//    if(func_login_back) func_login_back(ct.flg);
//}

//void ms_web_client::back_error(const std::string &meg)
//{

//}

//void ms_web_client::back_swap_txt(const std::string &meg)
//{
//    vlogf("back_swap_txt");
//    ct_swap_txt ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);
//    if(func_swap_txt) func_swap_txt(ct.account_from,ct.account_to,ct.data);
//}

//void ms_web_client::back_swap_file(const std::string &meg)
//{
//    ct_swap_file ct;
//    enum_transmit cmd;
//    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);

////    static long long sian = 0;
////    sian += ct.buf_size;
////    vlogw("long long sian: " v(sian));

//    //同名文件同时传输导致内容混乱并发生错误
//    auto it = map_ofs.find(ct.filename);
//    if(it == map_ofs.end())
//    {
//        if(ct.status == 0)
//        {
//            //首次进入时先插入文件对象，再将对象重新定位到操作符
//            vlogw("creator fstream: " v(ct.filename));
//            map_ofs.insert(pair<string,shared_ptr<fstream>>
//                           (ct.filename,std::make_shared<fstream>()));
//            it = map_ofs.find(ct.filename);//重新定位到操作符
//            if(it == map_ofs.end()) { vloge("creator fstream false"); return; }
//        }
//        else { vloge("swap error: " v(ct.filename)); return; }
//    }

//    //接受状态
//    if(ct.status == 1 && it->second->is_open())
//    {
//        if(write_file(it->second,ct.buf,ct.buf_size) == false)
//            { map_ofs.erase(it); return; }
////        vlogw("long long sian: " v(sian));
//    }
//    else if(ct.status == 2 && it->second->is_open())
//    {
//        if(write_file(it->second,ct.buf,ct.buf_size) == false)
//            { map_ofs.erase(it); return; }
//        it->second->close();
//        map_ofs.erase(it);

//        vlogw("func_swap_file" v(ct.status));
//        if(func_swap_file) func_swap_file(ct.account_from,ct.account_to,ct.filename);
//    }
//    else if(ct.status == 0)
//    {
//        //首次进入清空文件
//        vlogw("first open: " v(v_path_files + it->first) v(it->second));
//        it->second->open(v_path_files + it->first,ios::out|ios::binary);
//        if(it->second->is_open())
//        {
//            if(write_file(it->second,ct.buf,ct.buf_size) == false)
//                { map_ofs.erase(it); return; }
//            vlogw("is_open and write: " v(it->second));
//        }
//        else vlogw("file not open");
//    }
//    else vlogw("swap_file error" v(it->second->is_open()) v(ct.status));
//}

//template<class Tsock>
//bool ms_web_client::send_str(const Tsock &sock, const std::string &str)
//{

//}
