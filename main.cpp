#include "mainwindow.h"
#include <QApplication>

#include "socket.hpp"
#include "servidor.hpp"
#include "util.hpp"

#define PORTA 8228


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.Init();
    w.InitHold();
    return a.exec();
}
