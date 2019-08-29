#include "clabel.h"

OTUI::CLabel::CLabel()
{

}

OTUI::CLabel::CLabel(QString widgetId, QString imagePath) : CWidget(widgetId, imagePath)
{

}

OTUI::CLabel::~CLabel()
{

}

void OTUI::CLabel::draw(QPainter *painter)
{
    painter->save();
    painter->setPen(QColor("#dfdfdf"));
    painter->setFont(QFont("Verdana", 11));
    painter->drawText(x() + getParent()->x(), y() + getParent()->y(), width(), height(), Qt::AlignLeft | Qt::TextWrapAnywhere, QString("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer molestie dui non vestibulum consequat."));
    painter->restore();
}
