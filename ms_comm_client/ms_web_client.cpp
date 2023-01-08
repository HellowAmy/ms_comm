#include "ms_web_client.h"

ms_web_client::ms_web_client()
{
    vflog::instance()->init(vflog::e_info);
    vflog::instance()->close_log(false,false);
    set_file_path();

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

void ms_web_client::set_file_path(std::string path)
{
    v_path_files = path;
}

int ms_web_client::ask_register(std::string passwd)
{
    vlogf<<"ask_register"<<endl;
    ct_register ct;
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
    string str = ux_manage::to_str<enum_transmit,ct_register>
            (enum_transmit::e_register,ct);
    return send_meg(str);
}

int ms_web_client::ask_login(long long account, std::string passwd)
{
    vlogf<<"ask_login"<<endl;
    ct_login ct;
    ct.account = account;
    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
    string str = ux_manage::to_str<enum_transmit,ct_login>
            (enum_transmit::e_login,ct);
    return send_meg(str);
}

int ms_web_client::ask_swap_txt
    (long long account_from, long long account_to, std::string txt)
{
    vlogf<<"ask_swap_txt"<<endl;
    ct_swap_txt ct;
    ct.account_from = account_from;
    ct.account_to = account_to;
    strncpy(ct.data,txt.c_str(),sizeof(ct.data));
    string str = ux_manage::to_str<enum_transmit,ct_swap_txt>
            (enum_transmit::e_swap_txt,ct);
    return send_meg(str);
}

void ms_web_client::ask_swap_file
    (long long account_from,long long account_to,std::string filename)
{

    vflog::instance()->set_level(vflog::e_warning,vflog::e_warning);//vflog::e_warning
    vflog::instance()->close_log(false,true);
    vlogf<<"ask_swap_file:in"<<endl;
    vlogf<<"|"<<account_from<<"|"<<account_to<<"|"<<filename<<endl;
    auto func = [=](long long account_from,long long account_to,std::string filename)
    {
        vlogf<<"func:in"<<endl;
        bool is_err = false;
        ct_swap_file ct;
        ct.account_from = account_from;
        ct.account_to = account_to;

        fstream ofs(filename,ios::in | ios::binary);
        if(ofs.is_open())
        {
            vlogf<<"ofs.is_open(): "<<filename<<endl;
            ofs.seekg(0,ios::end);
            ct.lenght = ofs.tellg();
            ofs.seekg(0,ios::beg);

            string temp = tools::stm(filename)("\\",-1,-1);
            strncpy(ct.filename,temp.c_str(),sizeof(ct.filename));



            long long size_sum = 0;
            while(ofs.eof() == false)
            {
                //发送文件时状态
                if(ofs.tellg() == 0) ct.status = 0;
                else ct.status = 1;

                ofs.read(ct.buf,sizeof(ct.buf));
                ct.buf_size = ofs.gcount();//记录发送真实字节
                size_sum += ofs.gcount();//累计发送字节
                if(size_sum == ct.lenght) ct.status = 2;//最后一次发送
                vlogf<<"size_sum:"<<size_sum<<"|"<<ct.lenght<<endl;

                string str = ux_manage::to_str<enum_transmit,ct_swap_file>
                        (enum_transmit::e_swap_file,ct);
                int send_size = send_meg(str);
                if(send_size <= 0)
                {
                    vlogw<<"ask_swap_file :send close"<<endl;
                    is_err = true; break;
                }
                vlogf<<"send_size:"<<send_size<<endl;
            }
            ofs.close();
            if(is_err) vlogw<<"ask_swap_file is error: "<<is_err<<endl;
            else vlogf<<"send finish for file"<<endl;
        }
        else vlogw<<"ask_swap_file open false"<<filename<<endl;
        vlogf<<"func:out|err: "<<is_err<<endl;


    };

    func(account_from,account_to,filename);
    vflog::instance()->close_log(false,false);
    vflog::instance()->init(vflog::e_info);
//    std::thread th(func,account_from,account_to,filename);
//    th.join();
    vlogf<<"ask_swap_file:out"<<endl;
}

void ms_web_client::on_open()
{
    vlogf<<"on_open"<<endl;
    if(func_open) func_open();
}

void ms_web_client::on_message(const std::string &meg)
{
//    vflog::instance()->close_log(false,true);
    vlogf<<"get_id:"<<std::this_thread::get_id()<<endl;

    //执行匹配的任务函数
    enum_transmit cmd = *(enum_transmit*)
            string(meg.begin(),meg.begin() + sizeof(enum_transmit)).c_str();
    auto it_find = map_func.find(cmd);
    if(it_find != map_func.end())
    {
        vlogf<<"on_message:type: "<<cmd<<endl;
        (std::bind(it_find->second,meg))();
    }
    else vlogw<<"on_message:not find"<<endl;
//    vflog::instance()->close_log(false,false);
}

void ms_web_client::on_close()
{
    vlogf<<"on_close"<<endl;
}

int ms_web_client::send_meg(const std::string &meg)
{
    vflog::instance()->close_log(false,true);
    int ret = -1;
    if(this->sock().isConnected()) ret = this->sock().send(meg.c_str(),meg.size());
    else vlogw<<"isConnected false"<<endl;
    return ret;
}

bool ms_web_client::write_file(std::fstream *ofs, const char *buf, int size)
{
    ofs->write(buf,size);
    if(ofs->fail()) { vlogw<<"write_file false"<<endl; return false; }
    else return true;
}

void ms_web_client::back_register(const std::string &meg)
{
    vlogf<<"back_register"<<endl;
    ct_register_back ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_register_back>(meg,cmd,ct);
    if(func_register_back) func_register_back(ct.account,ct.passwd,ct.flg);
}

void ms_web_client::back_login(const std::string &meg)
{
    vlogf<<"back_login"<<endl;
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
    vlogf<<"back_swap_txt"<<endl;
    ct_swap_txt ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_txt>(meg,cmd,ct);
    if(func_swap_txt) func_swap_txt(ct.account_from,ct.account_to,ct.data);
}

void ms_web_client::back_swap_file(const std::string &meg)
{
    vlogf<<"back_swap_file"<<endl;
    ct_swap_file ct;
    enum_transmit cmd;
    ux_manage::from_str<enum_transmit,ct_swap_file>(meg,cmd,ct);

    //同名文件同时传输导致内容混乱并发生错误
    auto it = map_ofs.find(ct.filename);
    if(it == map_ofs.end())
    {
        vlogf<<"creator fstream"<<endl;
        map_ofs.insert(pair<string,fstream*>(ct.filename,new fstream));
        it = map_ofs.find(ct.filename);
        if(it == map_ofs.end()) { vlogf<<"insert false and return"<<endl; return; }
    }

    vflog::instance()->close_log(false,true);

    //接受状态
    if(ct.status == 1 && it->second->is_open())
    {
        if(write_file(it->second,ct.buf,ct.buf_size) == false)
            { vlogw<<"write_file false and return"<<endl; return; };
    }
    else if(ct.status == 2 && it->second->is_open())
    {
        if(write_file(it->second,ct.buf,ct.buf_size) == false)
            { vlogw<<"write_file false and return"<<endl; };
        it->second->close();

        vlogf<<"swap finish"<<endl;
        if(func_swap_file) func_swap_file(ct.account_from,ct.account_to,ct.filename);
    }
    else if(ct.status == 0)
    {
        //首次进入清空文件
        it->second->open(v_path_files + it->first,ios::out);
        if(it->second->is_open()) it->second->close();

        it->second->open(v_path_files + it->first,ios::out|ios::binary|ios::app);
        if(it->second->is_open())
        {
            vlogf<<"open: "<<v_path_files + it->first<<endl;
            if(write_file(it->second,ct.buf,ct.buf_size) == false)
                { vlogw<<"write_file false and return"<<endl; };
        }
        else vlogw<<"file not open"<<endl;
        vlogf<<"it->second->open: "<<v_path_files + it->first<<endl;
    }
    else vlogw<<"swap_file error|open: "<<it->second->is_open()<<"|status: "<<ct.status<<endl;

    vflog::instance()->close_log(false,false);
//    if(it->second->is_open() == false) map_ofs.erase(it);
}
