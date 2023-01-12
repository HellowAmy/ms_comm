#ifndef INTER_SOCK_H
#define INTER_SOCK_H

#include "include/WebSocketServer.h"
#include "include/WebSocketClient.h"

//using namespace std;
using namespace hv;
using std::function;
using std::string;
using namespace std::placeholders;
using std::bind;

typedef WebSocketChannelPtr web_sock;
typedef HttpRequestPtr web_http;

//void server_open(const web_sock& sock, const web_http& http){}
//void server_message(const web_sock& sock, const string& msg){}
//void server_close(const web_sock& sock){}

//void client_open(){}
//void client_message(const string& msg){}
//void client_close(){}

class inter_server
{
public:
    explicit inter_server()
    {
        func_bind(bind(&inter_server::on_open,this,_1,_2),
                  bind(&inter_server::on_message,this,_1,_2),
                  bind(&inter_server::on_close,this,_1));
    }
    virtual ~inter_server(){}

    void func_bind(function<void(const web_sock&, const web_http&)> open,
                   function<void(const web_sock&, const string&)> meg,
                   function<void(const web_sock&)> close){
        ws.onopen = open;
        ws.onmessage = meg;
        ws.onclose = close;
    }

    virtual void on_open(const web_sock& sock, const web_http& http){}
    virtual void on_message(const web_sock& sock, const string& meg){}
    virtual void on_close(const web_sock& sock){}

    WebSocketService& sock() { return ws; }
    WebSocketServer& get_server() { return server; }

    int open(int port = 5005,int th_count = 8)
    {
        server.registerWebSocketService(&ws);
        server.setPort(port);
        server.setThreadNum(th_count);
        return server.run();
    }

    void stop_server(){ server.stop(); };
    void start_server(){ server.start(); };

protected:
    WebSocketServer server;
    WebSocketService ws;
};

class inter_client
{
public:
    explicit inter_client()
    {
        func_bind(bind(&inter_client::on_open,this),
                  bind(&inter_client::on_message,this,_1),
                  bind(&inter_client::on_close,this));
    }
    virtual ~inter_client(){}

    WebSocketClient& sock() { return sock_client; }

    void func_bind(function<void()> open,
                   function<void(const string& msg)> meg,
                   function<void()> close){
        sock_client.onopen = open;
        sock_client.onmessage = meg;
        sock_client.onclose = close;
    }

    virtual void on_open(){}
    virtual void on_message(const string& meg){}
    virtual void on_close(){}

    int open(string ip = "127.0.0.1",int port = 5005,string txt = "")
    {
        reconn_setting_init(&reconn);
        reconn.min_delay = 1000;
        reconn.max_delay = 2000;
        reconn.delay_policy = 1;
//        sock_client.setReconnect(&reconn);
        sock_client.setReconnect(nullptr);


        string temp = "ws://"+ip+":"+to_string(port)+"/"+txt;
        return sock_client.open(temp.c_str());
    }
    void close_client(){ sock_client.closesocket(); sock_client.close(); }

protected:
    WebSocketClient sock_client;
    reconn_setting_t reconn;
};

#endif // INTER_SOCK_H
