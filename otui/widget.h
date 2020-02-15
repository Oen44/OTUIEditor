#ifndef OTUIWIDGET_H
#define OTUIWIDGET_H

#include <memory>
#include <QObject>
#include <QImage>
#include <QPainter>
#include <QCoreApplication>
#include "events/setidevent.h"
#include "types/CustomTypes.h"
#include "const.h"

namespace OTUI {
    template<typename T = int>
    struct EdgeGroup {
        EdgeGroup() { top = right = bottom = left = T(); }
        void set(T value) { top = right = bottom = left = value; }
        T top;
        T right;
        T bottom;
        T left;
    };
    class Widget : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString Id READ getId WRITE setIdProperty DESIGNABLE true USER true)
        Q_PROPERTY(Vec2 Position READ getPosition WRITE setPosition DESIGNABLE true USER true)
        Q_PROPERTY(Point Size READ getSizeProperty WRITE setSizeProperty DESIGNABLE true USER true)

    public:
        Widget();
        Widget(QString widgetId);
        Widget(QString widgetId, QString imagePath);
        virtual ~Widget() = default;

    public:
        virtual void draw(QPainter*) {}

    protected:
        QString m_id;
        QRect m_rect;
        QPoint m_virtualOffset;
        bool m_enabled;
        bool m_visible = true;
        bool m_focusable = true;
        bool m_fixedSize = false;
        bool m_phantom = false;
        bool m_draggable = false;
        bool m_destroyed = false;
        bool m_clipping = false;

        OTUI::Widget* m_parent;

    public:
        bool operator == (const Widget &Ref) const
        {
            return(this->getId() == Ref.getId());
        }

        QString getId() const { return m_id; }
        void setIdProperty(const QString& id);
        void setId(const QString id);

        Vec2 getPosition() const { return Vec2(getPos().x(), getPos().y()); }
        void setPosition(const Vec2& position);

        Point getSizeProperty() const { return Point(width(), height()); }
        void setSizeProperty(const Point& size);

        QImage image() const { return m_image; }

        int x() const { return m_rect.x(); }
        int y() const { return m_rect.y(); }
        int width() const { return m_rect.x() + m_rect.width() - m_rect.left(); }
        int height() const { return m_rect.y() + m_rect.height() - m_rect.top(); }
        QPoint getPos() const { return m_rect.topLeft(); }
        QPoint getSize() const { return QPoint(m_rect.width(), m_rect.height()); }
        QRect* getRect() { return &m_rect; }
        QRect getParentRect() const { return QRect(m_rect.left() + m_parent->getRect()->left(),
                                             m_rect.top() + m_parent->getRect()->top(),
                                             m_rect.right(), m_rect.bottom()); }
        void setRect(int left, int top, int right, int bottom) {
            m_rect.setRect(left, top, right, bottom);
        }
        void setRect(QRect rect) {
            m_rect = rect;
        }
        void setPos(QPoint topLeft) {
            m_rect.moveTopLeft(topLeft);
        }

        QPoint getImageSize() const { return m_imageSize; }
        void setImageSize(int width, int height) {
            m_imageSize.setX(width);
            m_imageSize.setY(height);
        }

        QRect getImageCrop() const { return m_imageCrop; }
        void setImageCrop(int left, int top, int right, int bottom) {
            m_imageCrop.setRect(left, top, right, bottom);
        }
        void setImageCrop(QRect crop) {
            m_imageCrop = crop;
        }

        QRect getImageBorder() const { return m_imageBorder; }
        void setImageBorder(QRect border) {
            m_imageBorder = border;
        }

        OTUI::Widget* getParent() const { return m_parent; }
        void setParent(OTUI::Widget* parent) {
            m_parent = parent;
        }

    protected:

        QImage m_image;
        QPoint m_imageSize;
        QRect m_imageCrop;
        QRect m_imageBorder;

        QColor m_color;
        QColor m_backgroundColor;

        QImage m_icon;
        QColor m_iconColor;
        QRect m_iconRect;
        QRect m_iconClipRect;
        OTUI::AlignmentFlag m_iconAlign;
        EdgeGroup<QColor> m_borderColor;
        EdgeGroup<int> m_borderWidth;
        EdgeGroup<int> m_margin;
        EdgeGroup<int> m_padding;
        float m_opacity;
        float m_rotation;
    };
}

#endif // OTUIWIDGET_H
