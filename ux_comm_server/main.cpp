#include "ux_web_server.h"

//template<class T1,class T2>
//bool s_equals(T1 s1,T2 s2)
//{ return std::string(s1) == std::string(s2); }

int main()
{
    cout<<"open server"<<endl;
    ux_web_server k;
    k.open_server();
    cout<<"close server"<<endl;

#if 0
    cout<<"服务器退出: "<<endl;
    ux_web_server server;
    int ret = server.open();
    cout<<"服务器退出: "<<ret<<endl;
#endif
    return 0;
}
