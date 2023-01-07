#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include <QObject>

//#include <iostream>
//using namespace std;

#include "ms_web_client.h"

class net_connect : public QObject
{
    Q_OBJECT
public:
    explicit net_connect(QObject *parent = nullptr);
//    void init(string ip = "127.0.0.1",int port = 5005,int thread = 1);
    int open_client(long long account,string passwd,
                     string ip = "127.0.0.1",int port = 5005);
    bool send_txt(long long account_to,string txt);
    bool send_file(long long account_to,string filename);
//    bool isf_connect();

signals:
    emit void fa_login_status(bool ok);
    emit void fa_swap_txt(long long from,QString txt);
    emit void fa_swap_files(long long from,QString filename);

protected:
//    bool is_connect = false;
    long long v_account = 0;
    string v_passwd;

    ms_web_client web_c;


//    bool is_working = true;//启动标记
//    queue<function<void()>> queue_task;//任务队列
//    std::mutex cv_lock;//操作变量前准备锁
//    std::condition_variable cv_var;//条件变量--等待唤醒
//    void work_thread();//任务线程
};

#endif // NET_CONNECT_H
