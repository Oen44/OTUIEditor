#include "widget.h"
#include "corewindow.h"

#include <QPixmapCache>

OTUI::Widget::Widget() : m_id("widgetid")
{
    m_parent = nullptr;
    m_font = QFont("Verdana", 11);
    m_color = QColor(223, 223, 223);
}

OTUI::Widget::Widget(QString widgetId)
{
    m_parent = nullptr;
    m_id.clear();
    m_id.append(widgetId);
    m_rect = QRect(0, 0, 32, 32);
    m_imageCrop.setRect(0, 0, -1, -1);
    m_font = QFont("Verdana", 11);
    m_color = QColor(223, 223, 223);
}

OTUI::Widget::Widget(QString widgetId, QString dataPath, QString imagePath)
{
    m_parent = nullptr;
    m_id.clear();
    m_id.append(widgetId);
    m_imageSource = imagePath;
    QString fullImagePath(dataPath + "/" + imagePath);
    if(!fullImagePath.isEmpty())
    {
        if (!QPixmapCache::find(fullImagePath, &m_image)) {
            m_image.load(fullImagePath);
            QPixmapCache::insert(fullImagePath, m_image);
        }
    }
    m_imageSize = QPoint(m_image.width(), m_image.height());
    m_rect = QRect(0, 0, m_image.width(), m_image.height());
    m_imageCrop.setRect(0, 0, m_image.width(), m_image.height());
    m_font = QFont("Verdana", 11);
    m_color = QColor(223, 223, 223);
}

void OTUI::Widget::event(QEvent *event)
{
    if(event->type() == SettingsSavedEvent::eventType)
    {
        SettingsSavedEvent *settings = reinterpret_cast<SettingsSavedEvent*>(event);
        QString fullImagePath(settings->dataPath + "/" + m_imageSource);
        if(!fullImagePath.isEmpty())
        {
            if (!QPixmapCache::find(fullImagePath, &m_image)) {
                m_image.load(fullImagePath);
                QPixmapCache::insert(fullImagePath, m_image);
            }
        }
        m_imageSize = QPoint(m_image.width(), m_image.height());
        m_imageCrop.setRect(0, 0, m_image.width(), m_image.height());
    }
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
