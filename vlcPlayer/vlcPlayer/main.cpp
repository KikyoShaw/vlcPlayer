#include "vlcPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//���ó���ico
	QApplication::setWindowIcon(QIcon(":/qrc/qrc/MediaPlayer.ico"));
    vlcPlayer w;
    w.show();
    return a.exec();
}
