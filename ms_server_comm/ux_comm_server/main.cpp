#include "ux_web_server.h"

#include <iostream>



//#include "ux_web_server.h"
#include "log_show.h"
#include <string>
#include <vector>
using namespace std;



#include <string.h>
enum en_tn
{
    e_t1,
    e_t2,
    e_t3
};

struct head
{
    int ty;
};

struct head2
{
    char ty[4];
};

struct t1
{
    long long ac;
};

struct t2
{
    char ps[128];
};

struct tt2 : t2
{
    char ps[128];
};

struct t3
{
    long long ac;
    char ps[128];
};

//template<class T>
//string to_str(head &h1,T &type)
//{
////    string ((char*)&h1);
////    string ((char*)&type);
//    return string ((char*)&h1) + string ((char*)&type);
//}




template <class T_head,class T_ct>
string to_str(T_head h,T_ct ct)
{
    return string ((char*)&h,sizeof(T_head)) + string ((char*)&ct,sizeof(T_ct));
}

template <class T_head,class T_ct>
void from_str(const string &str,T_head &h,T_ct &ct)
{
    h = *(T_head*)string(str.begin(),str.begin()+sizeof(T_head)).c_str();
    ct = *(T_ct*)string(str.begin()+sizeof(T_head),str.end()).c_str();
}


int main()
{

//    clog<<"asdasdp"<<endl;
//    cout<<k2.ps<<endl;
//    cout<<"org:  "<<h1.ty<<"  |  "<<k2.ps<<endl;
//    string s1((char*)&h1,sizeof(head));// = (char*)&h1;
//    string s2((char*)&k2,sizeof(t2));// = (char*)&k2;
//    cout<<"ch1:  "<< ((head*)s1.c_str())->ty <<"  |  "<< ((t2*)s2.c_str())->ps <<endl;
//    cout<<"size: "<<s1.size()<<" | "<<s2.size()<<endl;
//    string s3 = s1 + s2;
//    cout<<"s3:"<<s3.size()<<endl;
//    cout<<"s3r:  "<<hg.ty <<"  |  "<<kg.ps<<endl;
//    head g1 = *(head*)string(s3.begin(),s3.begin()+sizeof(head)).c_str();
//    t2 g2 = *(t2*)string(s3.begin()+sizeof(head),s3.end()).c_str();




//    t2 k2;
//    strncpy(k2.ps,"111111111asdhauhgwhdahsdgashdawd9999999999",sizeof(k2.ps));
//    string s3 = to_str(en_tn::e_t3,k2);



//    head hg;
//    t2 kg;
//    from_str(s3,hg,kg);
//    cout<<"s3r:  "<<hg.ty <<"  |  "<<kg.ps<<endl;



//    t3 k3;
//    k3.ac = 1241241;
//    strncpy(k3.ps,"2225asfkaklf6666",sizeof(k3.ps));
//    string s4 = to_str(en_tn::e_t2,k3);



//    head hg1;
//    t3 kg1;
//    from_str(s4,hg1,kg1);
//    cout<<"s3r:  "<<hg1.ty <<"  |  "<<kg1.ps<<"  |  "<<kg1.ac<<endl;


    ux_web_server k;
    k.open_server();


#if 0
    cout<<"服务器退出: "<<endl;
    ux_web_server server;
    int ret = server.open();
    cout<<"服务器退出: "<<ret<<endl;
#endif
    return 0;
}
