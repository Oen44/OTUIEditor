#include "button.h"

OTUI::Button::Button() : Widget()
{
    m_parent = nullptr;
}

OTUI::Button::Button(QString widgetId, QString imagePath) : Widget(widgetId, imagePath)
{
    m_parent = nullptr;
}

OTUI::Button::~Button()
{

}

void OTUI::Button::draw(QPainter *painter)
{
    painter->save();
    painter->setPen(QColor("#dfdfdf"));
    painter->setFont(QFont("Verdana", 11));
    painter->drawText(x() + getParent()->x(), y() + getParent()->y(), width(), height(), Qt::AlignCenter, getText());
    painter->restore();
}
