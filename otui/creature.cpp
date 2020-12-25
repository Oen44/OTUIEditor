#include "creature.h"

OTUI::Creature::Creature() : Widget()
{
    m_parent = nullptr;
}

OTUI::Creature::Creature(QString widgetId) : Widget(widgetId)
{
    m_parent = nullptr;
}

OTUI::Creature::Creature(QString widgetId, QString dataPath, QString imagePath) : Widget(widgetId, dataPath, imagePath)
{
    m_parent = nullptr;
}

OTUI::Creature::~Creature()
{

}
