#include <QApplication>

#include "fft.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv); 

    MainWindow w;
    w.show();

    return application.exec();
}
