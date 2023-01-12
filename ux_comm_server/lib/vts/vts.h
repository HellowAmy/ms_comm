#ifndef VTS_H
#define VTS_H


//=====
#define VOPEN_VLOG
#define VOPEN_STM
#define VOPEN_FOR
#define VOPEN_PUSH
//#define VLOG_CLOSE //开启此宏将取消所有注释--头文件之前定义
//=====

//===== vflog =====
#ifdef VOPEN_VLOG

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
using std::endl;

namespace vts
{
//===== vts =====

#define cl "\033[0m"<<end
#define okc ok_c && op_c && oc_c
#define okf ok_f && op_f && oc_f

//说明:简单打印日志系统,输出到文件中
//init()参数说明:
//      [string filename:输出到的文件名路径]
//      [level el:输出等级,等级不足将无法输出]
//      [bool is_new:追加或者从零开始写入到文件]
//
class vlog
{
public:
    enum level{ e_error,e_warning,e_debug,e_info };
    static vlog* instance() { if(obj==nullptr){obj=new vlog;} return obj; };
    void set_level(level elf,level elc) { el_f=elf; el_c=elc; }
    void set_status(bool of,bool oc) { oc_f=of; oc_c=oc; }

    bool init(const std::string &filename,bool is_app = true,level el = e_info)
    {
        if(v_filename!=filename){ if(op_f){ op_f=false; ofs.close(); } }
        else { if(op_f) return op_f; } el_f = el;
        if(is_app) ofs.open(filename,std::ios::out | std::ios::app);
        else ofs.open(filename,std::ios::out );
        op_f = ofs.is_open(); return op_f;
    }
    bool init(level el = e_info)
    {
        if(op_c) { return op_c; }
        el_c = el; op_c = true; return op_c;
    }

    template<class T>
    vlog& operator<<(const T &txt)
    { if(okf) ofs<<txt; if(okc) std::cout<<txt; return *this; };
    vlog& operator<<(std::ostream& (*end)(std::ostream&))
    { if(okf) ofs<<cl; if(okc) std::cout<<cl; return *this; };
    vlog& operator<<(level el)
    {
        if(el <= el_f) ok_f = true; else ok_f = false;
        if(el <= el_c) ok_c = true; else ok_c = false;
        if(okf){ ofs<<vec[el]<<"] ["<<get_time()<<"] "; }
        if(okc){ std::cout<<vec[el]<<"] "; }
        return *this;
    };

private:
    explicit vlog();
    ~vlog(){ if(ofs.is_open()){ofs.close();} };

    bool oc_f = true;
    bool oc_c = true;
    bool op_f = false;
    bool op_c = false;
    bool ok_f = false;
    bool ok_c = false;
    level el_f;
    level el_c;

    static vlog* obj;
    std::fstream ofs;
    std::vector<std::string> vec;
    std::string v_filename;

    std::string get_time()
    {
        time_t t; time(&t); char tmp[32];
        strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&t));
        return tmp;
    }
    class mcv { ~mcv(){delete obj; obj = nullptr;} }; static mcv t;
};

//打印到文件

#ifdef VLOG_CLOSE

#define vloge(...)
#define vlogw(...)
#define vlogd(...)
#define vlogf(...)

#else

#define vloge(...) \
    *vlog::instance()<<vlog::e_error\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogw(...) \
    *vlog::instance()<<vlog::e_warning\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogd(...) \
    *vlog::instance()<<vlog::e_debug\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#define vlogf(...) \
    *vlog::instance()<<vlog::e_info\
    <<"["<<__FILE__<<":<"<<__LINE__<<">] <<<< "<<__VA_ARGS__<<endl \

#endif

//==快速宏
#define v(value) "["#value": "<<value<<"] " \

#define l <<"|"<< \

#define vinit_f(filename,status,level) \
    vlog::instance()->init(filename,status,level) \

#define vinit_c(level) \
    vlog::instance()->init(level) \

#define vstatus(of,oc) \
    vlog::instance()->set_status(of,oc) \

#define vlevel(of,oc) \
    vlog::instance()->set_level(of,oc) \
//==快速宏

//===== vts =====
}
#endif //VOPEN_VLOG
//===== vflog =====


//===== stm =====
#ifdef VOPEN_STM

#include <string>
using std::string;

namespace vts
{
//===== vts =====

//功能:字符串切割,主要针对单字符串的多重切割,支持不同切割符号,支持正反双向切割
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
        if((begin < 0) && (end < 0)) v_str = split_back(v_str,flg,begin,end);
        else v_str = split_go(v_str,flg,begin,end);
        return split_t(arg...);
    }

    template<class ...Tarr>
    string operator()(Tarr ...arg) { return split_t(arg...); }

    //反向切割:反向参数传递
    string split_back(const string &str,const string &flg, int end,int begin)
    {
        begin *= -1; begin -= 1; end *= -1;
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
    string split_go(const string &str,const string &flg, int begin,int end)
    {
        int len = (end - begin) + 1;
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


//===== vts =====
}
#endif //VOPEN_STM
//===== stm =====


//===== for =====
#ifdef VOPEN_FOR

namespace vts
{
//===== vts =====


//功能:直接打印(包括换行)或者提供快捷的for版本
//      提供对象版本和指针版本,后缀_p为指针版本
//for_show : 根据迭代器打印
//for_it : 返回迭代器自行操作
//for_i : 返回类型size_t的变量自行操作
//for_n : 制定执行循环次数,不希望返回迭代器
#define for_show(arr) \
    for(auto it=arr.begin();it!=arr.end();it++) \
    { std::cout<<*it<<std::endl; }
#define for_show_p(arr) \
    for(auto it=arr->begin();it!=arr->end();it++) \
    { std::cout<<*it<<std::endl; }

#define for_it(it,arr) \
    for(auto it=arr.begin();it!=arr.end();it++)
#define for_it_p(it,arr) \
    for(auto it=arr->begin();it!=arr->end();it++)

#define for_i(i,arr) \
    for(size_t i=0;i<arr.size();i++)

#define for_n(n) \
    for(size_t index_it_for=0;index_it_for<n;index_it_for++)


//===== vts =====
}
#endif //VOPEN_FOR
//===== for =====


//===== push =====
#ifdef VOPEN_PUSH

#include <memory>

namespace vts
{
//===== vts =====

//===== define: push arr =====
//功能:op_dot类对","操作运算符进行重载，使得op_dot可以向标准库容器push内容
//      提供两个宏对容器进行操作:
//      new_arr:[容器对象][容器类型][数据类型][push的内容]
//      push_arr:[容器对象][push的内容]
template <template<class,class> class Tcont,class Ttype,
          class Talloc = std::allocator<Ttype>>
class op_dot
{
public:
    op_dot(){ sp_arr = std::make_shared<Tcont<Ttype,Talloc>>();}
    op_dot& operator,(const Ttype &type)
    { sp_arr->push_back(type); return *this; }
    std::shared_ptr<Tcont<Ttype,Talloc>> get_arr(){ return sp_arr;}
private:
    std::shared_ptr<Tcont<Ttype,Talloc>> sp_arr;
};

#define new_arr(arr,arr_type,type,...) \
    op_dot<arr_type,type> obj##arr; auto arr = obj##arr.get_arr(); obj##arr,__VA_ARGS__

#define push_arr(arr,...) \
    obj##arr,__VA_ARGS__


//===== vts =====
}
#endif //VOPEN_FOR
//===== push =====


#endif // VTS_H
