#include "ux_web_server.h"

int main()
{
    //启动服务器
    vlogd("open server");
    ux_web_server k;
    int ret = k.open_server();

    vloge("close server: " vv(ret));
    return ret;
}
