#include "ms_web_client.h"

ms_web_client::ms_web_client()
{
    //===== 添加任务函数到容器 =====
    //账号注册
    map_func.insert(pair<enum_transmit,function<void(const string&)>>
             (e_register_back,bind(&ms_web_client::back_register,this,_1)));
    //登录请求
    map_func.insert(pair<enum_transmit,function<void(const string&)>>
             (e_login_back,bind(&ms_web_client::back_login,this,_1)));
    //错误反馈
    map_func.insert(pair<enum_transmit,function<void(const string&)>>
             (e_error,bind(&ms_web_client::back_error,this,_1)));
    //数据交换
    map_func.insert(pair<enum_transmit,function<void(const string&)>>
             (e_swap_txt,bind(&ms_web_client::back_swap_txt,this,_1)));
    //文件交换
    map_func.insert(pair<enum_transmit,function<void(const string&)>>
             (e_swap_file,bind(&ms_web_client::back_swap_file,this,_1)));
    //===== 添加任务函数到容器 =====
}

int ms_web_client::open_client(string ip,int port, int thread)
{
    vlog<<"port: "<<port<<" | thread: "<<thread<<endl;
    for(int i=0;i<thread;i++)
    {
        std::thread th_temp(&ms_web_client::work_thread,this);
        th_temp.detach();
    }
    return this->open(ip,port);
}

int ms_web_client::ask_register(std::string passwd)
{
    vlog<<"ask_register"<<endl;
    ct_register ct;
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
    string str = ux_manage::to_str<enum_transmit,ct_register>
            (enum_transmit::e_register,ct);
    return this->sock().send(str.c_str(),str.size());
}

int ms_web_client::ask_login(long long account, std::string passwd)
{
    vlog<<"ask_login"<<endl;
    ct_login ct;
    ct.account = account;
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
    string str = ux_manage::to_str<enum_transmit,ct_login>
            (enum_transmit::e_login,ct);
    return this->sock().send(str.c_str(),str.size());
}

int ms_web_client::ask_swap_txt
    (long long account_from, long long account_to, std::string txt)
{
    vlog<<"ask_swap_txt"<<endl;
    ct_swap_txt ct;
    ct.account_from = account_from;
    ct.account_to = account_to;
    strncpy(ct.data,txt.c_str(),sizeof(ct.data));
    string str = ux_manage::to_str<enum_transmit,ct_swap_txt>
            (enum_transmit::e_swap_txt,ct);
    return this->sock().send(str.c_str(),str.size());
}

void ms_web_client::ask_swap_file(long long account_from,long long account_to,std::string filename)
{
    vlog<<"ask_swap_file"<<endl;
    auto func = [=](long long account_from,long long account_to,std::string filename)
    {
        ct_swap_file ct;
        ct.account_from = account_from;
        ct.account_to = account_to;

        fstream ofs(filename,ios::in | ios::binary);
        if(ofs.is_open())
        {
            ofs.seekg(0,ios::end);
            ct.lenght = ofs.tellg();
            ofs.seekg(0,ios::beg);
            while(ofs.eof() == false)
            {
                strncpy(ct.filename,filename.c_str(),sizeof(ct.filename));

                //发送文件时状态
                if(ofs.tellg() == 0) ct.status = 0;
                else ct.status = 1;

                ofs.read(ct.buf,sizeof(ct.buf));
                ct.buf_size = ofs.gcount();//记录发送真实字节
                if(ofs.tellg() == ct.lenght) ct.status = 2;//最后一次发送

                string str = ux_manage::to_str<enum_transmit,ct_swap_file>
                        (enum_transmit::e_swap_file,ct);
                this->sock().send(str.c_str(),str.size());
            }
            ofs.close();
        }
    };

    std::thread th(func,account_from,account_to,filename);
    th.detach();
}

void ms_web_client::on_open()
{
    vlog<<"on_open"<<endl;
    if(func_open) func_open();
}

void ms_web_client::on_message(const std::string &meg)
{
    vlog<<"on_message:in"<<endl;

    //执行匹配的任务函数
    enum_transmit cmd = *(enum_transmit*)
            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
    auto it_find = map_func.find(cmd);
    if(it_find != map_func.end())
    {
        vlog<<"on_message:type: "<<cmd<<endl;
        function<void()> func = std::bind(it_find->second,meg);
        queue_task.push(func);
        cv_var.notify_one();
    }
    else vlog<<"on_message:not find"<<endl;
}

void ms_web_client::on_close()
{
    vlog<<"on_close"<<endl;
}

void ms_web_client::back_register(const std::string &meg)
{
    vlog<<"back_register"<<endl;
    ct_register_back ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_register_back>(meg,cmd,ct);
    if(func_register_back) func_register_back(ct.account,ct.passwd,ct.flg);
}

void ms_web_client::back_login(const std::string &meg)
{
    vlog<<"back_login"<<endl;
    ct_login_back ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_login_back>(meg,cmd,ct);
    if(func_login_back) func_login_back(ct.flg);
}

void ms_web_client::back_error(const std::string &meg)
{

}

void ms_web_client::back_swap_txt(const std::string &meg)
{
    vlog<<"back_swap_txt"<<endl;
    ct_swap_txt ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);
    if(func_swap_txt) func_swap_txt(ct.account_from,ct.account_to,ct.data);
}

void ms_web_client::back_swap_file(const std::string &meg)
{
    vlog<<"back_swap_file"<<endl;
    ct_swap_file ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);

    //同名文件同时传输导致内容混乱并发生错误
    auto it = map_ofs.find(ct.filename);
    if(it == map_ofs.end())
        map_ofs.insert(pair<string,fstream>(ct.filename,fstream()));

    //接受状态
    if(ct.status == 1 && it->second.is_open()) it->second.write(ct.buf,ct.buf_size);
    else if(ct.status == 2 && it->second.is_open())
    {
        it->second.write(ct.buf,ct.buf_size);
        it->second.close();
        if(func_swap_file) func_swap_file(ct.account_from,ct.account_to,it->first);
    }
    else if(ct.status == 0)
    {
        it->second.open(it->first,ios::out | ios::binary | ios::app);
        if(it->second.is_open()) it->second.write(ct.buf,ct.buf_size);
    }
    else vlog<<"back_swap_file:recv file buf error"<<endl;
}

void ms_web_client::work_thread()
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
