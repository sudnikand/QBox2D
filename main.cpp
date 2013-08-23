#include <QtGui/QApplication>
#include "mainwindow.h"
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("QBox2D");
    qsrand(time(0));
    MainWindow w;
    w.show();

    return a.exec();
}
