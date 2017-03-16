#include <QApplication>

#include "startwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow sw;
#ifdef __ANDROID__
    sw.showFullScreen();
#else
    sw.showMaximized();
#endif
    return a.exec();
}
