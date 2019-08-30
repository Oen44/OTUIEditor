#include "label.h"
#include "corewindow.h"

OTUI::Label::Label() : Widget()
{

}

OTUI::Label::Label(QString widgetId, QString imagePath) : Widget(widgetId, imagePath)
{

}

OTUI::Label::~Label()
{

}

void OTUI::Label::draw(QPainter *painter)
{
    painter->save();
    painter->setPen(getColor());
    painter->setFont(QFont("Verdana", 11));
    painter->drawText(x() + getParent()->x(), y() + getParent()->y(), width(), height(), Qt::AlignLeft | Qt::TextWrapAnywhere, getText());
    painter->restore();
}
