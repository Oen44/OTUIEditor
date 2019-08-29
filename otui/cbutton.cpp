#include "cbutton.h"

OTUI::CButton::CButton()
{

}

OTUI::CButton::CButton(QString widgetId, QString imagePath) : CWidget(widgetId, imagePath)
{

}

OTUI::CButton::~CButton()
{

}

void OTUI::CButton::draw(QPainter *painter)
{
    painter->save();
    painter->setPen(QColor("#dfdfdf"));
    painter->setFont(QFont("Verdana", 11));
    painter->drawText(x() + getParent()->x(), y() + getParent()->y(), width(), height(), Qt::AlignCenter, QString("Button"));
    painter->restore();
}
