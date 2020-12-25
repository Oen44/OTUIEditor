#ifndef ITEM_H
#define ITEM_H

#include "widget.h"

namespace OTUI {
    class Item : public Widget
    {
    public:
        Item();
        Item(QString widgetId);
        Item(QString widgetId, QString dataPath, QString imagePath);
        ~Item();
    };
}

#endif // ITEM_H
