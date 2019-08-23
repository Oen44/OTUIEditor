#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QMessageBox::StandardButton reply;

    reply = QMessageBox::question(0, "Save", "Do you want to save your changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);*/

    MainWindow w;

    //QSurfaceFormat format;
    //format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    //format.setSwapInterval(0);
    //QSurfaceFormat::setDefaultFormat(format);

    //w.showMaximized();
    w.show();

    return a.exec();
}
