#include "standardplayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StandardPlayer w;
    w.show();

    return a.exec();
}
