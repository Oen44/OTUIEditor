#include <QtWidgets>
#include <QDebug>

#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_mousePos(0, 0), m_mousePressedPos(0, 0)
{
    m_mousePressed = false;
    m_brushNormal = QBrush(QColor(0, 255, 0));
    m_brushHover = QBrush(QColor(255, 0, 0));
    m_brushSelected = QBrush(QColor(0, 0, 255));
    scale = 1.0;

    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(update()));
    pTimer->start(1000 / 60.0);

    m_background.load(":/images/background.png");
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0);

    QPainter painter(this);
    painter.scale(scale, scale);
    painter.drawTiledPixmap(QRect(0, 0, this->width() / scale, this->height() / scale), m_background);
    for(auto const &widget : m_otuiWidgets)
    {
        OTUI::Widget *parent = widget->getParent();
        if(!widget->image().isNull())
        {
            if(widget->getImageBorder().isNull())
            {
                if(parent)
                    painter.drawImage(widget->getPos() + parent->getPos(), widget->image(), widget->getImageCrop());
                else
                    painter.drawImage(*widget->getRect(), widget->image(), widget->getImageCrop());
            }
            else
            {
                if(parent)
                    drawBorderImage(painter, *widget, widget->x() + parent->x(), widget->y() + parent->y());
                else
                    drawBorderImage(painter, *widget);
            }
        }

        widget->draw(painter);

        if(!m_selected) continue;

        if(widget->getId() == m_selected->getId())
        {
            if(parent)
            {
                drawOutlines(painter, (widget->x() - LINE_WIDTH / 2)  + parent->x(), (widget->y() - LINE_WIDTH / 2) + parent->y(), widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
                drawPivots(painter, widget->x() + parent->x(), widget->y() + parent->y(), widget->width(), widget->height());
            }
            else
            {
                drawOutlines(painter, widget->x() - LINE_WIDTH / 2, widget->y() - LINE_WIDTH / 2, widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
                drawPivots(painter, widget->x(), widget->y(), widget->width(), widget->height());
            }

        }
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = QPoint(event->x() / scale, event->y() / scale);
    if(m_selected)
    {
        OTUI::Widget *parent = m_selected->getParent();
        QRect parentBorder = QRect();
        if(parent)
        {
            parentBorder = parent->getImageBorder();
        }
        if(m_mousePressed)
        {
            if(m_mousePressedPivot != OTUI::NoPivot)
            {
                QRect *rect = m_selected->getRect();
                QPoint parentOffset(0, 0);
                if(parent)
                {
                    parentOffset = QPoint(parent->x(), parent->y());
                }

                switch (m_mousePressedPivot) {
                case OTUI::TopLeft: {
                    rect->setTopLeft(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::Top: {
                    rect->setTop(m_mousePos.y() - parentOffset.y());
                    break;
                }
                case OTUI::TopRight: {
                    rect->setTopRight(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::Left: {
                    rect->setLeft(m_mousePos.x() - parentOffset.x());
                    break;
                }
                case OTUI::Right: {
                    rect->setRight(m_mousePos.x() - parentOffset.x());
                    break;
                }
                case OTUI::BottomLeft: {
                    rect->setBottomLeft(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::Bottom: {
                    rect->setBottom(m_mousePos.y() - parentOffset.y());
                    break;
                }
                case OTUI::BottomRight: {
                    rect->setBottomRight(m_mousePos - parentOffset);
                    break;
                }
                default:
                    break;
                }

                int left = 1;
                int top = 1;

                if(parent)
                {
                    left = parentBorder.x();
                    top = parentBorder.y();
                }

                if(rect->left() < left)
                    rect->setLeft(left);
                if(rect->top() < top)
                    rect->setTop(top);

                if(rect->width() < 1)
                    rect->setWidth(1);
                if(rect->height() < 1)
                    rect->setHeight(1);

                if(parent)
                {
                    if(rect->right() > parent->width() - parentBorder.width())
                        rect->setRight(parent->width() - parentBorder.width());
                    if(rect->bottom() > parent->height() - parentBorder.height())
                        rect->setBottom(parent->height() - parentBorder.height());
                }
            }
            else
            {
                if(parent)
                {
                    if(m_selected->getParentRect().contains(m_mousePos))
                    {
                        setInBounds(m_selected, QPoint(m_mousePos - offset));
                    }
                }
                else
                {
                    if(m_selected->getRect()->contains(m_mousePos))
                    {
                        m_selected->setPos(m_mousePos - offset);
                    }
                }
            }
        }
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        m_mousePressedPos = QPoint(event->x() / scale, event->y() / scale);
        m_mousePressed = true;
        bool selected = false;
        m_selected = nullptr;
        for(auto &widget : reverse(m_otuiWidgets))
        {
            OTUI::Widget *parent = widget->getParent();
            if(parent)
            {
                QRect widgetRect(widget->x() + parent->x() - PIVOT_WIDTH / 2,
                                 widget->y() + parent->y() - PIVOT_HEIGHT / 2,
                                 widget->width() + PIVOT_WIDTH,
                                 widget->height() + PIVOT_HEIGHT);
                if(widgetRect.contains(m_mousePressedPos))
                {
                    m_selected = widget.get();
                    selected = true;
                    break;
                }
            }
            else
            {
                QRect widgetRect(widget->getRect()->x() - PIVOT_WIDTH / 2,
                                 widget->getRect()->y() - PIVOT_HEIGHT / 2,
                                 widget->getRect()->width() + PIVOT_WIDTH,
                                 widget->getRect()->height() + PIVOT_HEIGHT);
                if(widgetRect.contains(m_mousePressedPos))
                {
                    m_selected = widget.get();
                    selected = true;
                    break;
                }
            }
        }

        if(selected)
        {
            offset = m_mousePressedPos - (m_selected->getRect()->topLeft());
        }
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        m_mousePressed = false;
        m_mousePressedPivot = OTUI::NoPivot;
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(!m_selected) return;

    QRect *rect = m_selected->getRect();
    QPoint newPos(rect->topLeft());

    switch(event->key())
    {
    case Qt::Key_Up:
    {
        newPos.setY(newPos.y() - 1);
        break;
    }
    case Qt::Key_Down:
    {
        newPos.setY(newPos.y() + 1);
        break;
    }
    case Qt::Key_Left:
    {
        newPos.setX(newPos.x() - 1);
        break;
    }
    case Qt::Key_Right:
    {
        newPos.setX(newPos.x() + 1);
        break;
    }
    }

    OTUI::Widget *parent = m_selected->getParent();

    if(parent)
    {
        m_selected->setPos(newPos);

        QRect parentBorder = parent->getImageBorder();

        if(m_selected->x() < parentBorder.x())
            newPos.setX(parentBorder.x());
        if(m_selected->y() < parentBorder.y())
            newPos.setY(parentBorder.y());

        if(m_selected->x() + m_selected->width() > parent->width() - parentBorder.width())
            newPos.setX(parent->width() - m_selected->width() - parentBorder.width());
        if(m_selected->y() + m_selected->height() > parent->height() - parentBorder.height())
            newPos.setY(parent->height() - m_selected->height() - parentBorder.height());

        m_selected->setPos(newPos);
    }
    else
    {
        m_selected->setPos(newPos);
    }
}

void OpenGLWidget::drawBorderImage(QPainter &painter, OTUI::Widget const &widget)
{
    drawBorderImage(painter, widget, widget.x(), widget.y());
}

void OpenGLWidget::drawBorderImage(QPainter &painter, OTUI::Widget const &widget, int x, int y)
{
    int top = widget.getImageBorder().y();
    int bottom = widget.getImageBorder().height();
    int left =  widget.getImageBorder().x();
    int right  = widget.getImageBorder().width();

    // calculates border coords
    const QRect clip = widget.getImageCrop();
    QRect leftBorder(clip.left(), clip.top() + top, left, clip.height() - top - bottom);
    QRect rightBorder(clip.right() - right + 1, clip.top() + top, right, clip.height() - top - bottom);
    QRect topBorder(clip.left() + left, clip.top(), clip.width() - right - left, top);
    QRect bottomBorder(clip.left() + left, clip.bottom() - bottom + 1, clip.width() - right - left, bottom);
    QRect topLeftCorner(clip.left(), clip.top(), left, top);
    QRect topRightCorner(clip.right() - right + 1, clip.top(), right, top);
    QRect bottomLeftCorner(clip.left(), clip.bottom() - bottom + 1, left, bottom);
    QRect bottomRightCorner(clip.right() - right + 1, clip.bottom() - bottom + 1, right, bottom);
    QRect center(clip.left() + left, clip.top() + top, clip.width() - right - left, clip.height() - top - bottom);
    QPoint bordersSize(leftBorder.width() + rightBorder.width(), topBorder.height() + bottomBorder.height());
    QPoint centerSize = widget.getSize() - bordersSize;
    QRect rectCoords;
    QRect drawRect(x, y, widget.width(), widget.height());

    // first the center
    if((centerSize.x()*centerSize.y()) > 0) {
        rectCoords = QRect(drawRect.left() + leftBorder.width(),
                           drawRect.top() + topBorder.height(),
                           centerSize.x(),
                           centerSize.y());
        painter.drawImage(rectCoords, widget.image(), center);
    }
    // top left corner
    rectCoords = QRect(drawRect.topLeft(), topLeftCorner.size());
    painter.drawImage(rectCoords, widget.image(), topLeftCorner);
    // top
    rectCoords = QRect(drawRect.left() + topLeftCorner.width(), drawRect.topLeft().y(), centerSize.x(), topBorder.height());
    painter.drawImage(rectCoords, widget.image(), topBorder);
    // top right corner
    rectCoords = QRect(QPoint(drawRect.left() + topLeftCorner.width() + centerSize.x(), drawRect.top()), topRightCorner.size());
    painter.drawImage(rectCoords, widget.image(), topRightCorner);
    // left
    rectCoords = QRect(drawRect.left(), drawRect.top() + topLeftCorner.height(), leftBorder.width(), centerSize.y());
    painter.drawImage(rectCoords, widget.image(), leftBorder);
    // right
    rectCoords = QRect(drawRect.left() + leftBorder.width() + centerSize.x(), drawRect.top() + topRightCorner.height(), rightBorder.width(), centerSize.y());
    painter.drawImage(rectCoords, widget.image(), rightBorder);
    // bottom left corner
    rectCoords = QRect(QPoint(drawRect.left(), drawRect.top() + topLeftCorner.height() + centerSize.y()), bottomLeftCorner.size());
    painter.drawImage(rectCoords, widget.image(), bottomLeftCorner);
    // bottom
    rectCoords = QRect(drawRect.left() + bottomLeftCorner.width(), drawRect.top() + topBorder.height() + centerSize.y(), centerSize.x(), bottomBorder.height());
    painter.drawImage(rectCoords, widget.image(), bottomBorder);
    // bottom right corner
    rectCoords = QRect(QPoint(drawRect.left() + bottomLeftCorner.width() + centerSize.x(), drawRect.top() + topRightCorner.height() + centerSize.y()), bottomRightCorner.size());
    painter.drawImage(rectCoords, widget.image(), bottomRightCorner);
}

void OpenGLWidget::drawOutlines(QPainter &painter, int left, int top, int width, int height)
{
    painter.setPen(QPen(Qt::white, LINE_WIDTH, Qt::DashLine, Qt::SquareCap));
    painter.drawLine(left, top, left + width, top);
    painter.drawLine(left + width, top, left + width, top + height);
    painter.drawLine(left + width, top + height, left, top + height);
    painter.drawLine(left, top + height, left, top);
}

void OpenGLWidget::drawPivots(QPainter &painter, int left, int top, int width, int height)
{
    QRect rect(left - PIVOT_WIDTH / 2, top - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    QBrush brush = m_brushNormal;
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::TopLeft))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::TopLeft;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width / 2 - PIVOT_WIDTH / 2, top - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::Top))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::Top;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width - PIVOT_WIDTH / 2, top - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::TopRight))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::TopRight;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left - PIVOT_WIDTH / 2, top + height / 2 - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::Left))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::Left;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width - PIVOT_WIDTH / 2, top + height / 2 - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::Right))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::Right;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left - PIVOT_WIDTH / 2, top + height - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::BottomLeft))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::BottomLeft;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width / 2 - PIVOT_WIDTH / 2, top + height - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::Bottom))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::Bottom;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width - PIVOT_WIDTH / 2, top + height - PIVOT_HEIGHT / 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed &&(m_mousePressedPivot == OTUI::NoPivot || m_mousePressedPivot == OTUI::BottomRight))
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::BottomRight;
        }
        else
            brush = m_brushHover;
    }

    painter.fillRect(rect, brush);

}
