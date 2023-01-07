#include "ux_web_server.h"

int main()
{
    cout<<"open server"<<endl;
    ux_web_server k;
    k.open();
    cout<<"close server"<<endl;

#if 0
    cout<<"服务器退出: "<<endl;
    ux_web_server server;
    int ret = server.open();
    cout<<"服务器退出: "<<ret<<endl;
#endif
    return 0;
}
