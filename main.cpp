#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //创建主窗口
    MainWindow widget;
    widget.show();
    return a.exec();
}
