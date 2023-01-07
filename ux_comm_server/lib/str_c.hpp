#ifndef STR_C_HPP
#define STR_C_HPP

#include <string>
using std::string;

namespace tools
{
//===== namespace tools =====

//功能:字符串切割,主要针对单字符串的多重切割,支持不同切割符号,支持正反向切割
//算法:查找标记位置,向前取值,舍弃标记末尾数据
//用法示例:
//      [1]
//      string a = "1-22-333-4444-55555";
//      string b = "-";
//      cout<<"ret = "<<tools::stm(a)("-",0,2)<<endl;
//      [ret = 1-22-333]
//      cout<<"ret = "<<tools::stm(a)("-",-1,-1)<<endl;
//      [ret = 55555]
//      cout<<"ret = "<<tools::stm(a)("-",-3,-1)<<endl;
//      [ret = 333-4444-55555]
//
//      [2]
//      string a = "11--22--33ashgdas pas<<19911-12-3>>da asdg ausd";
//      string b = "--";
//      cout<<"ret = "<<tools::stm(a)(" ",1,1,"<<",1,-1,">>",0,0)<<endl;
//      [ret = 19911-12-3]
//
struct stm
{
    string v_str;
    stm(const string &str) : v_str(str){}

    template<class ...Tarr> string split_t(){ return v_str; }
    template<class ...Tarr>
    string split_t(const string &flg,int begin,int end,Tarr ...arg)
    {
        if((begin < 0) && (end < 0)) v_str = split_d(v_str,flg,begin,end);
        else v_str = split_s(v_str,flg,begin,end);
        return split_t(arg...);
    }

    template<class ...Tarr>
    string operator()(Tarr ...arg) { return split_t(arg...); }

    //反向切割:反向参数传递
    string split_d(const string &str,const string &flg, int end,int begin)
    {
        begin *= -1;
        begin -= 1;
        end *= -1;
        int len = end - begin;
        if((begin < 0) || (len <= 0)) return "";

        size_t pos_begin = str.size();
        for(int i=0;i<begin;i++)
        {
            if((pos_begin = str.rfind(flg,pos_begin)) == str.npos) return "";
            pos_begin -= 1;
        }

        size_t pos_end = pos_begin;
        for(int i=0;i<len;i++)
        {
            if((pos_end = str.rfind(flg,pos_end)) == str.npos) return "";
            pos_end -= 1;
        }

        if(pos_begin != str.size()) pos_begin += 1;
        pos_end += flg.size() +1;

        if(pos_begin <= pos_end) return "";
        return string (str.begin()+pos_end,str.begin()+pos_begin);
    }

    //正向切割:正向参数传递
    string split_s(const string &str,const string &flg, int begin,int end)
    {
        int len = (end - begin) +1;
        if((begin < 0) || (len <= 0)) return "";

        size_t pos_begin = 0;
        for(int i=0;i<begin;i++)
        {
            if((pos_begin = str.find(flg,pos_begin)) == str.npos) return "";
            pos_begin += flg.size();
        }

        size_t pos_end = pos_begin;
        for(int i=0;i<len;i++)
        {
            if((pos_end = str.find(flg,pos_end)) == str.npos) return "";
            pos_end += flg.size();
        }

        pos_end -= flg.size();
        if(pos_begin >= pos_end) return "";
        return string(str.begin()+pos_begin,str.begin()+pos_end);
    }
};


//===== namespace tools =====
}

#endif // STR_C_HPP
