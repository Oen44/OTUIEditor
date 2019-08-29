#ifndef CLABEL_H
#define CLABEL_H

#include "otuiwidget.h"

namespace OTUI {
    class CLabel : public CWidget
    {
    public:
        CLabel();
        CLabel(QString widgetId, QString imagePath);
        ~CLabel();

    public:
        void draw(QPainter* painter);
    };
}

#endif // CLABEL_H
