#include "item.h"

OTUI::Item::Item() : Widget()
{
    m_parent = nullptr;
}

OTUI::Item::Item(QString widgetId) : Widget(widgetId)
{
    m_parent = nullptr;
}

OTUI::Item::Item(QString widgetId, QString dataPath, QString imagePath) : Widget(widgetId, dataPath, imagePath)
{
    m_parent = nullptr;
}

OTUI::Item::~Item()
{

}
