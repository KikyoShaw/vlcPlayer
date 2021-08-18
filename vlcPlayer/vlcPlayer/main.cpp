#include "vlcPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vlcPlayer w;
    w.show();
    return a.exec();
}
