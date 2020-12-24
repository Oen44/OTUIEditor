#include "widget.h"
#include "corewindow.h"

OTUI::Widget::Widget() : m_id("widgetid")
{
    m_parent = nullptr;
    m_font = QFont("Verdana", 11);
    m_color = QColor("#dfdfdf");
}

OTUI::Widget::Widget(QString widgetId)
{
    m_parent = nullptr;
    m_id.clear();
    m_id.append(widgetId);
    m_rect = QRect(0, 0, 32, 32);
    m_imageCrop.setRect(0, 0, -1, -1);
    m_font = QFont("Verdana", 11);
    m_color = QColor("#dfdfdf");
}

OTUI::Widget::Widget(QString widgetId, QString imagePath)
{
    m_parent = nullptr;
    m_id.clear();
    m_id.append(widgetId);
    if(imagePath != nullptr)
        m_image.load(imagePath);
    m_imageSize = QPoint(m_image.width(), m_image.height());
    m_rect = QRect(0, 0, m_image.width(), m_image.height());
    m_imageCrop.setRect(0, 0, -1, -1);
    m_font = QFont("Verdana", 11);
    m_color = QColor("#dfdfdf");
}

void OTUI::Widget::setId(const QString id) {
    m_id = id;
}

void OTUI::Widget::setIdProperty(const QString &id) {
    if(m_id == nullptr || id.size() == 0) return;

    SetIdEvent *event = new SetIdEvent();
    event->oldId = m_id;
    event->newId = id;
    m_id = id;
    qApp->postEvent(qApp->activeWindow(), event);
}

void OTUI::Widget::setPosition(const QVector2D &position)
{
    setPos(QPoint(position.x(), position.y()));
}

void OTUI::Widget::setSizeProperty(const QPoint &size)
{
    m_rect.setWidth(size.x());
    m_rect.setHeight(size.y());
    if(getParent() != nullptr)
    {
        if(getRect()->right() > getParent()->width())
            getRect()->setRight(getParent()->width());
        if(getRect()->bottom() > getParent()->height())
            getRect()->setBottom(getParent()->height());
    }
}
