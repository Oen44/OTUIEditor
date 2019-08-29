#include "cmainwindow.h"

OTUI::CMainWindow::CMainWindow() : CWidget()
{

}

OTUI::CMainWindow::CMainWindow(QString widgetId, QString imagePath) : CWidget(widgetId, imagePath)
{

}

OTUI::CMainWindow::~CMainWindow()
{

}

void OTUI::CMainWindow::draw(QPainter *painter)
{
    painter->save();
    painter->setPen(QColor("#dfdfdf"));
    painter->setFont(QFont("Verdana", 11));
    painter->drawText(x(), y(), width(), 25, Qt::AlignCenter, QString("Title"));
    painter->restore();
}
