#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(855,630);
    //w.showMinimized();
    w.show();

    return a.exec();
}
