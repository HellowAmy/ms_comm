#ifndef NET_CONNECT_H
#define NET_CONNECT_H

#include <QObject>

#include "ms_web_client.h"
#include "lib/vts/vts.h"

class net_connect : public QObject
{
    Q_OBJECT
public:
    explicit net_connect(QObject *parent = nullptr);
    int open_client(long long account,string passwd,
                     string ip = "127.0.0.1",int port = 5005);
    void send_txt(long long account_to,string txt);
    void send_file(long long account_to,string filename);

signals:
    emit void fa_connect_close();
    emit void fa_login_status(bool ok);
    emit void fa_swap_txt(long long from,QString txt);
    emit void fa_swap_files(long long from,QString filename);

protected:
    long long v_account = 0;
    string v_passwd;
    ms_web_client web_c;
};

#endif // NET_CONNECT_H
