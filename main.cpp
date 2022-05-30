#include "mygraphicsmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGraphicsMainWindow w;
    w.show();
    return a.exec();
}
