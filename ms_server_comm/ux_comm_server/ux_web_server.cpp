#include "ux_web_server.h"

ux_web_server::ux_web_server()
{
    file_account = "../data/account.txt";
    ux_manage::load_account(file_account,map_account);

    //===== 添加任务函数到容器 =====
    //账号注册
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_register,bind(&ux_web_server::task_register,this,_1,_2)));
    //登录请求
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_login,bind(&ux_web_server::task_login,this,_1,_2)));
    //错误反馈
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
                    (e_error,bind(&ux_web_server::task_error,this,_1,_2)));
    //数据交换
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
             (e_swap_txt,bind(&ux_web_server::task_swap_txt,this,_1,_2)));
    //文件交换
    map_func.insert(pair<enum_transmit,function<void(const web_sock&, const string&)>>
                    (e_swap_file,bind(&ux_web_server::task_swap_file,this,_1,_2)));
    //===== 添加任务函数到容器 =====

}

int ux_web_server::open_server(int port,int thread)
{
    vlog<<"port: "<<port<<" | thread: "<<thread<<endl;
    for(int i=0;i<thread;i++)
    {
        std::thread th_temp(&ux_web_server::work_thread,this);
        th_temp.detach();
    }
    return this->open(port);
}

void ux_web_server::on_open(const web_sock &sock, const web_http &http)
{
    vlog<<"on_open"<<endl;
}

void ux_web_server::on_message(const web_sock &sock, const string &meg)
{
    //执行匹配的任务函数
    enum_transmit cmd = *(enum_transmit*)
            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
    auto it_find = map_func.find(cmd);
    if(it_find != map_func.end())
    {
        vlog<<"on_message:type:"<<cmd<<endl;
        function<void()> func = std::bind(it_find->second,sock,meg);
        queue_task.push(func);
        cv_var.notify_one();
    }
    else vlog<<"on_message: not find"<<endl;
}

void ux_web_server::on_close(const web_sock &sock)
{
    for(auto a:map_connect)
    {
        if(a.second == sock)
        {
            map_connect.erase(a.first);
            break;
        }
    }
    vlog<<"on_close:map_connect size: "<<map_connect.size()<<endl;
}

void ux_web_server::task_register(const web_sock &sock, const std::string &meg)
{
    vlog<<"task_register:in"<<endl;

    ct_register ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_register>(meg,cmd,ct);

    //add_account: 账号插入到容器，并返回账号
    ct_register_back ct_back;
    if(ux_manage::add_account(map_account,ct.passwd,ct_back.account))
    {
        strncpy(ct_back.passwd,ct.passwd,sizeof(ct.passwd));
        ct_back.flg = ux_manage::save_account(file_account,map_account);
    }
    else ct_back.flg = 0;

    //反馈到客户端
    string str = ux_manage::to_str<enum_transmit,ct_register_back>
            (enum_transmit::e_register_back,ct_back);
    sock->send(str.c_str(),str.size());
}

void ux_web_server::task_login(const web_sock &sock, const std::string &meg)
{
    vlog<<"task_login:in"<<endl;

    ct_login ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_login>(meg,cmd,ct);

    //添加到连接队列
    ct_login_back ct_back;
    if(ux_manage::add_connect<const web_sock &>
            (map_connect,ct.account,sock)) { ct_back.flg = 1; }
    else ct_back.flg = 0;

    //反馈到客户端
    string str = ux_manage::to_str<enum_transmit,ct_login_back>
            (enum_transmit::e_login_back,ct_back);
    sock->send(str.c_str(),str.size());

    vlog<<"task_login:map_connect size: "<<map_connect.size()<<endl;
    for(auto a:map_connect)
    {
        vlog<<a.first<<endl;
    }
}

void ux_web_server::task_error(const web_sock &sock, const std::string &meg)
{

}

void ux_web_server::task_swap_txt(const web_sock &sock, const std::string &meg)
{
    vlog<<"task_swap_txt"<<endl;

    ct_swap_txt ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end()) { it->second->send(meg.c_str(),meg.size()); }
    else
    {
        vlog<<"task_swap_txt:not find account"<<endl;
        ct_error err;
        err.account = ct.account_to;
        strncpy(err.error,"error: task_swap_txt",sizeof(err.error));

        //错误反馈
        string str = ux_manage::to_str<enum_transmit,ct_error>
                (enum_transmit::e_error,err);
        sock->send(str.c_str(),str.size());
    }
}

void ux_web_server::task_swap_file(const web_sock &sock, const std::string &meg)
{
    vlog<<"task_swap_file"<<endl;

    ct_swap_file ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);

    auto it = map_connect.find(ct.account_to);
    if(it != map_connect.end()) { it->second->send(meg.c_str(),meg.size()); }
    else
    {
        vlog<<"task_swap_file:not find account"<<endl;
        ct_error err;
        err.account = ct.account_to;
        strncpy(err.error,"error: task_swap_file",sizeof(err.error));

        //错误反馈
        string str = ux_manage::to_str<enum_transmit,ct_error>
                (enum_transmit::e_error,err);
        sock->send(str.c_str(),str.size());
    }
}

void ux_web_server::work_thread()
{
    vlog<<"work_thread - thread id: "<<std::this_thread::get_id()<<endl;

    while(is_working)
    {
        function<void()> func;
        {
            //上锁--避免多线程竞争,函数返回真则启动唤醒
            std::unique_lock<std::mutex> lock (cv_lock);
            while(queue_task.empty()) cv_var.wait(lock);

            func = queue_task.front();
            queue_task.pop();
        }
        func();
    }
}
