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
    m_otuiWidgets.clear();
}

OpenGLWidget::~OpenGLWidget()
{
    m_otuiWidgets.clear();
    delete m_selected;
}

void OpenGLWidget::initializeGL()
{
    OTUI::CWidget* widget = new OTUI::CWidget("mainWindow", ":/images/main_window.png");
    widget->setRect(50, 50, 256, 256);
    OTUI::CWidget* child = new OTUI::CWidget("button", ":/images/button_rounded.png");
    child->m_parent = widget;
    child->setRect(50, 50, 106, 23);
    child->setImageCrop(0, 0, 22, 23);
    child->m_imageBorder = QRect(5, 5, 5, 5);
    widget->m_children.append(child);
    m_otuiWidgets.append(widget);
    setMouseTracking(true);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = QPoint(event->x(), event->y());
    if(m_selected != nullptr)
    {
        if(m_mousePressed)
        {
            if(m_selected->m_parent != nullptr)
            {
                if(m_selected->getParentRect().contains(m_mousePos))
                {
                    m_selected->setPos(m_mousePos - offset);
                }
            }
            else
            {
                if(m_selected->getRect().contains(m_mousePos))
                {
                    m_selected->setPos(m_mousePos - offset);
                }
            }
        }
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        m_mousePressedPos = QPoint(event->x(), event->y());
        m_mousePressed = true;
        bool selected = false;
        m_selected = nullptr;
        for(OTUI::CWidget* widget : m_otuiWidgets)
        {
            selected = checkChildrenOverlap(widget);

            if(selected)
                break;

            if(widget->getRect().contains(m_mousePressedPos))
            {
                m_selected = widget;
                selected = true;
                break;
            }
        }

        if(selected)
        {
            offset = m_mousePressedPos - m_selected->getRect().topLeft();
        }
    }
}

bool OpenGLWidget::checkChildrenOverlap(OTUI::CWidget* parent)
{
    bool selected = false;
    for(OTUI::CWidget* child : parent->m_children)
    {
        if(QRect(child->x() + parent->x(), child->y() + parent->y(), child->width(), child->height()).contains(m_mousePressedPos))
        {
            m_selected = child;
            selected = true;
            break;
        }

        selected = checkChildrenOverlap(child);
    }
    return selected;
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
        m_mousePressed = false;
}

void OpenGLWidget::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0);
    QPainter painter(this);

    for(OTUI::CWidget* widget : m_otuiWidgets)
    {
        if(widget->m_imageBorder.isNull())
        {
            painter.drawImage(widget->getPos(), widget->image(), widget->getImageCrop());
        }
        else
        {
            drawBorderImage(&painter, widget);
        }
        drawWidgetChildren(&painter, widget);

        if(widget == m_selected)
        {
            drawOutlines(&painter, widget->x() - LINE_WIDTH / 2, widget->y() - LINE_WIDTH / 2, widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
            drawPivots(&painter, widget->x(), widget->y(), widget->width(), widget->height());
        }
    }
    update();
}

void OpenGLWidget::drawWidgetChildren(QPainter *painter, OTUI::CWidget* parent)
{
    for(OTUI::CWidget* widget : parent->m_children)
    {
        if(widget->m_imageBorder.isNull())
        {
            painter->drawImage(widget->getPos() + parent->getPos(), widget->image(), widget->getImageCrop());
        }
        else
        {
            drawBorderImage(painter, widget, widget->x() + parent->x(), widget->y() + parent->y());
        }

        if(widget == m_selected)
        {
            drawOutlines(painter, (widget->x() - LINE_WIDTH / 2)  + parent->x(), (widget->y() - LINE_WIDTH / 2) + parent->y(), widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
            drawPivots(painter, widget->x() + parent->x(), widget->y() + parent->y(), widget->width(), widget->height());
        }
    }
}

void OpenGLWidget::drawBorderImage(QPainter *painter, OTUI::CWidget* widget)
{
    drawBorderImage(painter, widget, widget->x(), widget->y());
}

void OpenGLWidget::drawBorderImage(QPainter *painter, OTUI::CWidget* widget, int x, int y)
{
    int top = widget->m_imageBorder.top();
    int bottom = widget->m_imageBorder.bottom();
    int left =  widget->m_imageBorder.left();
    int right  = widget->m_imageBorder.right();

    // calculates border coords
    const QRect clip = widget->getImageCrop();
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
    QPoint centerSize = widget->getSize() - bordersSize;
    QRect rectCoords;
    QRect drawRect(x, y, widget->width(), widget->height());

    // first the center
    if((centerSize.x()*centerSize.y()) > 0) {
        rectCoords = QRect(drawRect.left() + leftBorder.width(),
                           drawRect.top() + topBorder.height(),
                           centerSize.x(),
                           centerSize.y());
        painter->drawImage(rectCoords, widget->image(), center);
    }
    // top left corner
    rectCoords = QRect(drawRect.topLeft(), topLeftCorner.size());
    painter->drawImage(rectCoords, widget->image(), topLeftCorner);
    // top
    rectCoords = QRect(drawRect.left() + topLeftCorner.width(), drawRect.topLeft().y(), centerSize.x(), topBorder.height());
    painter->drawImage(rectCoords, widget->image(), topBorder);
    // top right corner
    rectCoords = QRect(QPoint(drawRect.left() + topLeftCorner.width() + centerSize.x(), drawRect.top()), topRightCorner.size());
    painter->drawImage(rectCoords, widget->image(), topRightCorner);
    // left
    rectCoords = QRect(drawRect.left(), drawRect.top() + topLeftCorner.height(), leftBorder.width(), centerSize.y());
    painter->drawImage(rectCoords, widget->image(), leftBorder);
    // right
    rectCoords = QRect(drawRect.left() + leftBorder.width() + centerSize.x(), drawRect.top() + topRightCorner.height(), rightBorder.width(), centerSize.y());
    painter->drawImage(rectCoords, widget->image(), rightBorder);
    // bottom left corner
    rectCoords = QRect(QPoint(drawRect.left(), drawRect.top() + topLeftCorner.height() + centerSize.y()), bottomLeftCorner.size());
    painter->drawImage(rectCoords, widget->image(), bottomLeftCorner);
    // bottom
    rectCoords = QRect(drawRect.left() + bottomLeftCorner.width(), drawRect.top() + topBorder.height() + centerSize.y(), centerSize.x(), bottomBorder.height());
    painter->drawImage(rectCoords, widget->image(), bottomBorder);
    // bottom right corner
    rectCoords = QRect(QPoint(drawRect.left() + bottomLeftCorner.width() + centerSize.x(), drawRect.top() + topRightCorner.height() + centerSize.y()), bottomRightCorner.size());
    painter->drawImage(rectCoords, widget->image(), bottomRightCorner);
}


void OpenGLWidget::drawOutlines(QPainter *painter, int left, int top, int width, int height)
{
    painter->setPen(QPen(Qt::white, LINE_WIDTH, Qt::DashLine, Qt::SquareCap));
    painter->drawLine(left, top, left + width, top);
    painter->drawLine(left + width, top, left + width, top + height);
    painter->drawLine(left + width, top + height, left, top + height);
    painter->drawLine(left, top + height, left, top);
}

void OpenGLWidget::drawPivots(QPainter *painter, int left, int top, int width, int height)
{
    QRect rect(left - 2, top - 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    QBrush brush = m_brushNormal;
    if(rect.contains(m_mousePressedPos))
    {
        if(m_mousePressed)
            brush = m_brushSelected;
        else
            brush = m_brushHover;
    }

    painter->fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width - 6, top - 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePressedPos))
    {
        if(m_mousePressed)
            brush = m_brushSelected;
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left + width - 6, top - 2, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

    brush = m_brushNormal;
    rect.setRect(left - 2, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePressedPos))
    {
        if(m_mousePressed)
            brush = m_brushSelected;
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left - 2, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

    brush = m_brushNormal;
    rect.setRect(left + width - 6, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePressedPos))
    {
        if(m_mousePressed)
            brush = m_brushSelected;
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left + width - 6, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

}
