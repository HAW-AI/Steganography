#include <QtGui/QApplication>
#include "mainwindow.h"
#include "qstring.h"
#include "bitChanger.h"
#include "steganography.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
