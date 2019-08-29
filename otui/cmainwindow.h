#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include "otuiwidget.h"

namespace OTUI {
    class CMainWindow : public CWidget
    {
    public:
        CMainWindow();
        CMainWindow(QString widgetId, QString imagePath);
        ~CMainWindow();

    public:
        void draw(QPainter* painter);
    };

}

#endif // CMAINWINDOW_H
