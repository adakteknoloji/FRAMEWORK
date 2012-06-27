#include <QtGui/QApplication>
#include "printer_test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PRINTER_TEST w;
    w.show();

    return a.exec();
}
