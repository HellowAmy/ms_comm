#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{

//    tools::stm("\\")()
//    string filename = "D:\\test\\11\\11.PNG";
//    string temp = tools::stm(filename)("\\",-1,-1);
//    std::cout<<temp<<endl;
//    return 0;

    //    3359116136123qwe
    //    4093996320123qwe
    //    5511780295123qwe
    QApplication a(argc, argv);

#if 0
    Widget w;
    w.open_c(3359116136,"123qwe");
    w.show();
#else
    Widget w1;
//    w1.open_c(4093996320,"123qwe");
    w1.show();
#endif
//    Widget w2;
//    w2.open_c(5511780295,"123qwe");
//    w2.show();

    return a.exec();
}
