#include "net_connect.h"

net_connect::net_connect(QObject *parent)
{
//    std::thread (&net_connect::work_thread,this).detach();

//    connect(this,&net_connect::fa_login_status,this,[](bool ok){
//        vlog<<"po="<<ok<<endl;
//    },Qt::DirectConnection);
//    connect(this,&net_connect::fa_swap_txt,this,[](long long from,string txt){
//        vlog<<"fa_swap_txt="<<from<<"|"<<txt<<endl;
//    },Qt::DirectConnection);

//    connect(web_c,&ms_web_client::fa_register_back,this,[](bool ok){
//        vlog<<"po="<<ok<<endl;
//    });




}

void net_connect::open(long long account,string passwd)
{



    web_c.func_open = [=](){
        web_c.ask_login(account,passwd);
    };
    web_c.func_login_back = [=](bool ok){
        is_connect = ok;
//        cao_log(ok);
//        emit fa_login_status(ok);
//        function<void()> fun = bind(&net_connect::fa_login_status,this,ok);
////        queue_task(fun);
/// e
///
     emit fa_login_status(ok);
//        queue_task.push(bind(&net_connect::fa_login_status,this,ok));
//        cv_var.notify_one();
//        vlog<<"fa_login_status:"<<is_connect<<endl;
    };
    web_c.func_swap_txt = [=](long long from,long long to,string txt){
        emit fa_swap_txt(from,txt);
        vlog<<"emit: "<<from<<""<<txt<<endl;

    };
    web_c.func_swap_file = [=](long long from,long long to,string filename){
        emit fa_swap_files(from,filename);
    };
}

void net_connect::init(std::string ip, int port, int thread)
{
    web_c.open_client(ip,port,thread);
}

bool net_connect::send_txt(long long account_to, std::string txt)
{
    if(is_connect) web_c.ask_swap_txt(v_account,account_to,txt);
    else return false;
    return true;
}

bool net_connect::send_file(long long account_to, std::string filename)
{
    if(is_connect) web_c.ask_swap_file(v_account,account_to,filename);
    else return false;
    return true;
}

bool net_connect::isf_connect()
{
    return is_connect;
}

//void net_connect::work_thread()
//{
//    vlog<<"work_thread - thread id: "<<std::this_thread::get_id()<<endl;

//    while(is_working)
//    {
//        function<void()> func;
//        {
//            //上锁--避免多线程竞争,函数返回真则启动唤醒
//            std::unique_lock<std::mutex> lock (cv_lock);
//            while(queue_task.empty()) cv_var.wait(lock);

//            func = queue_task.front();
//            queue_task.pop();
//        }
//        func();
//        vlog<<"func"<<endl;
//    }
//}

