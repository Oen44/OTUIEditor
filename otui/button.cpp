#include "button.h"

OTUI::Button::Button() : Widget()
{
    m_parent = nullptr;
}

OTUI::Button::Button(QString widgetId, QString dataPath, QString imagePath) : Widget(widgetId, dataPath, imagePath)
{
    m_parent = nullptr;
}

OTUI::Button::~Button()
{

}

void OTUI::Button::draw(QPainter &painter)
{
    painter.save();
    painter.setPen(getColor());
    painter.setFont(getFont());
    painter.drawText(x() + getParent()->x(), y() + getParent()->y(), width(), height(), Qt::AlignCenter, getText());
    painter.restore();
}
