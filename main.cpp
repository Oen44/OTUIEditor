#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QSurfaceFormat>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile File(":/stylesheet.css");
    File.open(QFile::ReadOnly);
    a.setStyleSheet(File.readAll());

    MainWindow w;
    w.show();

    return a.exec();
}
