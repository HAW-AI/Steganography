#include <QtGui/QApplication>
#include "difftests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiffTests w;
    w.show();
    
    return a.exec();
}
