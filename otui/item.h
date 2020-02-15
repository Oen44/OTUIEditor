#ifndef ITEM_H
#define ITEM_H

#include "widget.h"

namespace OTUI {
    class Item : public Widget
    {
    public:
        Item();
        Item(QString widgetId);
        ~Item();
    };
}

#endif // ITEM_H
