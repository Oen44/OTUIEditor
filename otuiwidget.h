#ifndef OTUICWIDGET_H
#define OTUICWIDGET_H

#include <QImage>
#include <QVector>

namespace OTUI {
class CWidget
{
public:
    CWidget();
    CWidget(QString widgetId, QString imagePath);
    ~CWidget();

public:
    bool operator == (const CWidget &Ref) const
    {
        return(this->id == Ref.id);
    }

    QImage image() { return m_image; }

    int x() { return m_rect.x(); }
    int y() { return m_rect.y(); }
    int width() { return m_rect.x() + m_rect.width() - m_rect.left(); }
    int height() { return m_rect.y() + m_rect.height() - m_rect.top(); }
    QPoint getPos() { return m_rect.topLeft(); }
    QPoint getSize() { return QPoint(m_rect.width(), m_rect.height()); }
    QRect getRect() { return m_rect; }
    QRect getParentRect() { return QRect(getRect().left() + m_parent->getRect().left(),
                                         getRect().top() + m_parent->getRect().top(),
                                         getRect().right(), getRect().bottom()); }
    void setRect(int left, int top, int right, int bottom) {
        m_rect.setRect(left, top, right, bottom);
    }
    void setPos(QPoint topLeft) {
        m_rect.moveTopLeft(topLeft);
    }

    QPoint getImageSize() { return m_imageSize; }
    void setImageSize(int width, int height) {
        m_imageSize.setX(width);
        m_imageSize.setY(height);
    }

    QRect getImageCrop() { return m_imageCrop; }
    void setImageCrop(int left, int top, int right, int bottom) {
        m_imageCrop.setRect(left, top, right, bottom);
    }

    QString id;
    CWidget* m_parent = nullptr;
    QVector<CWidget*> m_children;
    QRect m_imageBorder;

private:
    QImage m_image;
    QPoint m_imageSize;
    QRect m_imageCrop;
    QRect m_rect;
};
}

#endif // OTUICWIDGET_H
