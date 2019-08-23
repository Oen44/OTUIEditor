#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);

    //w.showMaximized();
    w.show();

    return a.exec();
}
