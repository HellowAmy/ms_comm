#include "widget.h"
//#include "ms_web_client.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    //    3359116136123qwe
    //    4093996320123qwe
    //    5511780295123qwe
    QApplication a(argc, argv);
    Widget w;
    w.open_c(3359116136,"123qwe");
    w.show();

    Widget w1;
    w1.open_c(4093996320,"123qwe");
    w1.show();

    Widget w2;
    w2.open_c(5511780295,"123qwe");
    w2.show();

    return a.exec();
}
