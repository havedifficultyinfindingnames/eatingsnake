#include "eatingsnake.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eatingsnake w;
    w.show();
    return a.exec();
}
