#include "mainwindow.h"

OTUI::MainWindow::MainWindow() : Widget()
{
}

OTUI::MainWindow::MainWindow(QString widgetId, QString dataPath, QString imagePath) : Widget(widgetId, dataPath, imagePath)
{
}

OTUI::MainWindow::~MainWindow()
{
}

void OTUI::MainWindow::draw(QPainter &painter)
{
    painter.save();
    painter.setPen(getColor());
    painter.setFont(QFont("Verdana", 11));
    painter.drawText(x(), y(), width(), 25, Qt::AlignCenter, getText());
    painter.restore();
}
