#ifndef CBUTTON_H
#define CBUTTON_H

#include "otuiwidget.h"

namespace OTUI {
    class CButton : public CWidget
    {
    public:
        CButton();
        CButton(QString widgetId, QString imagePath);
        ~CButton();

    public:
        void draw(QPainter* painter);
    };
}

#endif // CBUTTON_H
