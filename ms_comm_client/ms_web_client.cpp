#include "ms_web_client.h"

ms_web_client::ms_web_client()
{
    set_file_path();

    //===== 添加任务函数到容器 =====
    map_task_add(register_back);            //注册任务反馈-c
    map_task_add(login_back);               //登录请求反馈-c
    map_task_add(logout_back);              //登出请求反馈-c
    map_task_add(recover_passwd_back);      //找回密码反馈-c
    map_task_add(swap_txt);                 //交换文字:c1->c2
    map_task_add(swap_file_build);          //建立文件:c1->c2
    map_task_add(swap_file_request);        //发送文件段请求:c2->c1
    map_task_add(swap_file_send);           //发送文件段:c1->c2
    map_task_add(swap_file_finish);         //发送完成:c1->c2
    map_task_add(swap_file_ret);            //发送完成:c2->c1
    map_task_add(swap_error);               //错误返回:all
    //===== 添加任务函数到容器 =====

}

void ms_web_client::set_file_path(std::string path)
{ v_path_files = path; }



//===== 快速建立请求函数 =====
//功能： 解析接收内容并触发func回调函数
//== 宏参考模板 ==
//void ms_web_client::ask_register(std::string passwd）//参数替换IN_ARGS
//{
//    vlogf("ask_register");
//    MAKE_CT_REQ(ct,register);
//    //== 函数体 ==
//    strncpy(ct.passwd,passwd.c_str(),sizeof(ct.passwd));
//    //== 函数体 ==
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
    send_str(to_str(ct));                           \
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
    vlogd("send:ask_swap_txt");
    MAKE_CT_SWAP(ct,swap_txt,account_to);
    ct.account_from = account_from;
    strncpy(ct.buf_txt,txt.c_str(),sizeof(ct.buf_txt));

    if(send_str(to_str(ct)))
    { vlogw("send err: ask_swap_txt"); }
}

void ms_web_client::ask_swap_file
    (long long account_from, long long account_to,
     std::string filename,std::string file_path,en_build_file type)
{
    vlogd("send:ask_swap_file");
    MAKE_CT_SWAP(ct,swap_file_build,account_to);

    fstream ofs(file_path,ios::in|ios::binary);
    if(ofs.is_open())
    {
        //记录发送信息
        ofs.seekg(0,ios::end);
        ct.size_file = ofs.tellg();
        ct.size_block = WRITE_BUFSIZE_HIGH_WATER;//发送量达到高负载时停止
        ct.account_from = account_from;
        ct.type = type;
        strncpy(ct.filename,filename.c_str(),sizeof(ct.filename));
        ofs.seekg(0,ios::beg);

        //记录文件流状态
        io_send ct_send;
        ct_send.is_send = true;
        ct_send.type = type;
        ct_send.size_block = ct.size_block;
        ct_send.size_file = ct.size_file;
        ct_send.account_from = account_from;
        ct_send.account_to = account_to;
        ct_send.ofs = move(ofs);
        vlogf("is_open and ask_swap_file send");

        send_str(to_str(ct));
        map_send.insert(pair<string,io_send>(to_str<long long>(account_to)+filename,std::move(ct_send)));
        vlogf("insert: " vv(filename) vv(map_send.size()));
    }
    else
    {
        vlogw("is_open err");
    }
}



//===== 快速定义反馈函数 =====
//== 原型 ==
//void ms_web_client::register_back(const std::string &meg)
//{
//    vlogd("register_back");
//    ct_register_back ct;
//    to_ct(meg,ct);
//    if(func_register_back) func_register_back
//            (ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);
//}
//== 原型 ==

//功能： 解析接收内容并触发func回调函数
#define BUILD_CT_BACK_FUNC(func,...)                   \
void ms_web_client::func(const std::string &meg)       \
{                                                      \
    vlogd(#func);                                      \
    ct_##func ct;                                      \
    to_ct(meg,ct);                                     \
    if(func_##func) func_##func(__VA_ARGS__);          \
}

BUILD_CT_BACK_FUNC(register_back,
    ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);

BUILD_CT_BACK_FUNC(login_back,
    ct.account,ct.is_success);

BUILD_CT_BACK_FUNC(logout_back,
    ct.account,ct.is_success);

BUILD_CT_BACK_FUNC(recover_passwd_back,
    ct.account,string(ct.passwd,sizeof(ct.passwd)),ct.is_success);

BUILD_CT_BACK_FUNC(swap_txt,
    ct.account_from,string(ct.buf_txt,sizeof(ct.buf_txt)));

BUILD_CT_BACK_FUNC(swap_file_ret,
    ct.account_from,string(ct.filename,sizeof(ct.filename)),ct.is_success);

BUILD_CT_BACK_FUNC(swap_error,
    ct.account_from,ct.err);
//===== 快速定义反馈函数 =====



void ms_web_client::swap_file_build(const std::string &meg)
{
    vlogf("revc:swap_file_build");
    ct_swap_file_build ct;
    io_recv ct_flg;
    to_ct(meg,ct);

    string filename(ct.filename);
    fstream ofs(v_path_files+filename,ios::out|ios::binary);
    if(ofs.is_open())
    {
        vlogd("is open: " vv(v_path_files+filename));
        ct_flg.is_send = true;
        ct_flg.type = ct.type;
        ct_flg.save_path = v_path_files + filename;
        ct_flg.size_block = ct.size_block;
        ct_flg.size_file = ct.size_file;
        ct_flg.account_to = ct.account_from;
        ct_flg.account_from = ct.head.account_to;
        ct_flg.ofs = std::move(ofs);

        MAKE_CT_SWAP(ct_back,swap_file_request,ct_flg.account_to);
        ct_back.account_from = ct_flg.account_from;
        strncpy(ct_back.filename,ct.filename,sizeof(ct_back.filename));

        vlogd("insert: " vv(filename) vv(ct_flg.save_path));
        map_recv.insert(pair<string,io_recv>(filename,std::move(ct_flg)));

        send_str(to_str(ct_back));
    }
    else { vlogw("swap_file_build open err"); }
}

void ms_web_client::swap_file_send(const std::string &meg)
{
    ct_swap_file_send ct;
    to_ct(meg,ct);

    //接收文件块
    auto it = map_recv.find(string(ct.filename));
    if(it != map_recv.end())
    {
        io_recv *precv = &it->second;
        write_buf(&precv->ofs,ct.buf,ct.size_buf);

        //接收块完成，请求下一块
        if(ct.is_next == true)
        {
            vlogf("revc: next request: " vv(precv->ofs.tellg()));
            MAKE_CT_SWAP(ct_back,swap_file_request,precv->account_to);
            ct_back.account_from = precv->account_from;
            strncpy(ct_back.filename,ct.filename,sizeof(ct_back.filename));
            send_str(to_str(ct_back));
        }
    }
    else { vlogw("not find: swap_file_send"); }
}


void ms_web_client::swap_file_finish(const std::string &meg)
{
    vlogf("recv:swap_file_finish");
    ct_swap_file_finish ct;
    to_ct(meg,ct);

    auto it = map_recv.find(string(ct.filename));
    if(it != map_recv.end())
    {
        io_recv *precv = &it->second;
        MAKE_CT_SWAP(ct_back,swap_file_ret,precv->account_to);

        if(precv->size_file == precv->ofs.tellg())
        { ct_back.is_success = true;  }
        else { ct_back.is_success = false; }

        ct_back.account_from = precv->account_from;
        strncpy(ct_back.filename,ct.filename,sizeof(ct_back.filename));
        send_str(to_str(ct_back));//反馈到发送方

        //反馈到接收方
        if(func_swap_file_finish)
        {
            func_swap_file_finish
                    (precv->account_to,precv->save_path,
                     precv->type,ct_back.is_success);
        }
        precv->ofs.close();
        map_recv.erase(it);
        vlogd("io_recv close: " vv(ct_back.is_success) vv(precv->size_file) vv(precv->ofs.tellg()));
    }
    else { vlogw("not find: swap_file_finish"); }
}

void ms_web_client::swap_file_request(const std::string &meg)
{
    vlogf("send:ct_swap_file_request");
    ct_swap_file_request ct;
    to_ct(meg,ct);

    //查询已经打开的文件
    auto it = map_send.find(to_str<long long>(ct.account_from)+string(ct.filename));
    if(it != map_send.end())
    {
        io_send *psend = &it->second;
        if(psend->is_send)
        {
            //发送一块数据
            MAKE_CT_SWAP(ct_back,swap_file_send,psend->account_to);
            size_t count = (psend->size_block/sizeof(ct_back.buf));
            for(size_t i=0;i<count;i++)
            {
                ct_back.size_buf = read_buf
                        (&psend->ofs,ct_back.buf,sizeof(ct_back.buf));
                if(ct_back.size_buf < 0) vloge("write_buf err");

                strncpy(ct_back.filename,ct.filename,sizeof(ct_back.filename));
                ct_back.account_from = psend->account_from;
                ct_back.off_file = psend->ofs.tellg();

                //发送块完成标记
                if(i >= (count -1) && (psend->ofs.eof() == false))
                    ct_back.is_next = true;
                else ct_back.is_next = false;

                if(ct_back.is_next) vlogf("off:" vv(psend->ofs.tellg()) vv(ct_back.size_buf));

                send_str(to_str(ct_back));
            }

            //文件触底，发送完成标记
            if(psend->ofs.eof())
            {
                vlogd("send file finish: in eof:" vv(ct_back.size_buf));
                MAKE_CT_SWAP(ct_end,swap_file_finish,psend->account_to);
                ct_end.account_from = psend->account_from;
                strncpy(ct_end.filename,ct.filename,sizeof(ct_end.filename));
                send_str(to_str(ct_end));

                //结束文件发送
                psend->ofs.close();
                map_send.erase(it);
            }
        }
        else
        {
            vloge("swap_file_request err");
        }
    }
    else vlogw("not find: swap_file_request");
}

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
    else { vloge("not find fun: " vv(meg)); }

}

void ms_web_client::on_close()
{
    vlogw("on_close");
    if(func_close) func_close();
}

long long ms_web_client::write_buf(std::fstream *ofs, const char *buf, int size)
{
    ofs->write(buf,size);
    return ofs->gcount();
}

long long ms_web_client::read_buf(std::fstream *ofs,char *buf, int size)
{
    memset(buf,0,size);
    ofs->read(buf,size);
    return ofs->gcount();
}

bool ms_web_client::send_str(const std::string &str)
{
    std::unique_lock<mutex> lock(lock_send);
    this->sock().send(str.c_str(),str.size());
    return this->sock().isConnected();
}
