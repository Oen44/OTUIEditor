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
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    setMouseTracking(true);
}

void OpenGLWidget::addWidget(OTUI::WidgetType type, QString widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder)
{
    std::unique_ptr<OTUI::CWidget> widget = initializeWidget(type, widgetId, imagePath);

    if(widget == nullptr)
    {
        MainWindow::ShowError("Error", QString("Can't add %1 widget.\nWidget type is incorrect.").arg(widgetId));
        return;
    }

    m_selected = widget.get();

    widget->setRect(rect);
    widget->setImageCrop(imageCrop);
    widget->setImageBorder(imageBorder);
    m_otuiWidgets.push_back(std::move(widget));
}

void OpenGLWidget::addWidgetChild(OTUI::WidgetType type, QString parentId, QString widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder)
{
    OTUI::CWidget* parent = nullptr;
    for(auto& w : m_otuiWidgets)
    {
        if(w->getId() == parentId)
        {
            parent = w.get();
            break;
        }
    }

    if(parent == nullptr)
    {
        MainWindow::ShowError("Error", QString("Can't add %1 widget.\nParent with id %1 not found.").arg(widgetId).arg(parentId));
        return;
    }

    std::unique_ptr<OTUI::CWidget> widget = initializeWidget(type, widgetId, imagePath);

    if(widget == nullptr)
    {
        MainWindow::ShowError("Error", QString("Can't add %1 widget.\nWidget type is incorrect.").arg(widgetId));
        return;
    }

    m_selected = widget.get();

    widget->setRect(rect);
    widget->setImageCrop(imageCrop);
    widget->setImageBorder(imageBorder);
    widget->setParent(parent);
    parent->addChild(widget);
}

std::unique_ptr<OTUI::CWidget> OpenGLWidget::initializeWidget(OTUI::WidgetType type, QString widgetId, QString imagePath)
{
    std::unique_ptr<OTUI::CWidget> widget = nullptr;
    switch(type)
    {
    case OTUI::MainWindow:
    {
        widget = std::make_unique<OTUI::CMainWindow>(widgetId, imagePath);
        break;
    }
    case OTUI::Button:
    {
        widget = std::make_unique<OTUI::CButton>(widgetId, imagePath);
        break;
    }
    case OTUI::Label:
    {
        widget = std::make_unique<OTUI::CLabel>(widgetId, imagePath);
        break;
    }
    }

    return widget;
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
            if(m_mousePressedPivot != OTUI::NoPivot)
            {
                QRect* rect = m_selected->getRect();
                QPoint parentOffset(0, 0);
                if(m_selected->getParent() != nullptr)
                {
                    parentOffset = QPoint(m_selected->getParent()->x(), m_selected->getParent()->y());
                }

                switch (m_mousePressedPivot) {
                case OTUI::TopLeft: {
                    rect->setTopLeft(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::TopRight: {
                    rect->setTopRight(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::BottomLeft: {
                    rect->setBottomLeft(m_mousePos - parentOffset);
                    break;
                }
                case OTUI::BottomRight: {
                    rect->setBottomRight(m_mousePos - parentOffset);
                    break;
                }
                default:
                    break;
                }

                if(rect->left() < 1)
                    rect->setLeft(1);
                if(rect->top() < 1)
                    rect->setTop(1);

                if(m_selected->getParent() != nullptr)
                {
                    if(rect->right() > m_selected->getParent()->width())
                        rect->setRight(m_selected->getParent()->width());
                    if(rect->bottom() > m_selected->getParent()->height())
                        rect->setBottom(m_selected->getParent()->height());
                }
            }
            else
            {
                if(m_selected->getParent() != nullptr)
                {
                    if(m_selected->getParentRect().contains(m_mousePos))
                    {
                        QPoint newPos(m_mousePos - offset);
                        m_selected->setPos(newPos);

                        if(m_selected->x() < 0)
                            newPos.setX(0);
                        if(m_selected->y() < 0)
                            newPos.setY(0);

                        if(m_selected->x() + m_selected->width() > m_selected->getParent()->width())
                            newPos.setX(m_selected->getParent()->width() - m_selected->width());
                        if(m_selected->y() + m_selected->height() > m_selected->getParent()->height())
                            newPos.setY(m_selected->getParent()->height() - m_selected->height());

                        m_selected->setPos(newPos);
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
        m_mousePressedPos = QPoint(event->x(), event->y());
        m_mousePressed = true;
        bool selected = false;
        m_selected = nullptr;
        for(auto& widget : reverse(m_otuiWidgets))
        {
            selected = checkChildrenOverlap(widget);

            if(selected)
                break;

            if(widget->getRect()->contains(m_mousePressedPos))
            {
                m_selected = widget.get();
                selected = true;
                break;
            }
        }

        if(selected)
        {
            offset = m_mousePressedPos - m_selected->getRect()->topLeft();
        }
    }
}

bool OpenGLWidget::checkChildrenOverlap(std::unique_ptr<OTUI::CWidget> const& parent)
{
    bool selected = false;
    for(auto& child : reverse(parent->getChildren()))
    {
        if(QRect(child->x() + parent->x(), child->y() + parent->y(), child->width(), child->height()).contains(m_mousePressedPos))
        {
            m_selected = child.get();
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
    {
        m_mousePressed = false;
        m_mousePressedPivot = OTUI::NoPivot;
    }
}

void OpenGLWidget::draw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0);
    QPainter painter(this);

    for(auto const& widget : m_otuiWidgets)
    {
        if(!widget->image().isNull())
        {
            if(widget->getImageBorder().isNull())
            {
                painter.drawImage(*widget->getRect(), widget->image(), widget->getImageCrop());
            }
            else
            {
                drawBorderImage(&painter, *widget);
            }
        }

        widget->draw(&painter);

        drawWidgetChildren(&painter, *widget);

        if(m_selected == nullptr) continue;

        if(widget->getId() == m_selected->getId())
        {
            drawOutlines(&painter, widget->x() - LINE_WIDTH / 2, widget->y() - LINE_WIDTH / 2, widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
            drawPivots(&painter, widget->x(), widget->y(), widget->width(), widget->height());
        }
    }
    update();
}

void OpenGLWidget::drawWidgetChildren(QPainter *painter, OTUI::CWidget const& parent)
{
    for(auto& widget : parent.getChildren())
    {
        if(!widget->image().isNull())
        {
            if(widget->getImageBorder().isNull())
            {
                painter->drawImage(widget->getPos() + parent.getPos(), widget->image(), widget->getImageCrop());
            }
            else
            {
                drawBorderImage(painter, *widget, widget->x() + parent.x(), widget->y() + parent.y());
            }
        }

        widget->draw(painter);

        if(m_selected == nullptr) continue;

        if(widget->getId() == m_selected->getId())
        {
            drawOutlines(painter, (widget->x() - LINE_WIDTH / 2)  + parent.x(), (widget->y() - LINE_WIDTH / 2) + parent.y(), widget->width() + LINE_WIDTH, widget->height() + LINE_WIDTH);
            drawPivots(painter, widget->x() + parent.x(), widget->y() + parent.y(), widget->width(), widget->height());
        }
    }
}

void OpenGLWidget::drawBorderImage(QPainter *painter, OTUI::CWidget const& widget)
{
    drawBorderImage(painter, widget, widget.x(), widget.y());
}

void OpenGLWidget::drawBorderImage(QPainter *painter, OTUI::CWidget const& widget, int x, int y)
{
    int top = widget.getImageBorder().top();
    int bottom = widget.getImageBorder().bottom();
    int left =  widget.getImageBorder().left();
    int right  = widget.getImageBorder().right();

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
        painter->drawImage(rectCoords, widget.image(), center);
    }
    // top left corner
    rectCoords = QRect(drawRect.topLeft(), topLeftCorner.size());
    painter->drawImage(rectCoords, widget.image(), topLeftCorner);
    // top
    rectCoords = QRect(drawRect.left() + topLeftCorner.width(), drawRect.topLeft().y(), centerSize.x(), topBorder.height());
    painter->drawImage(rectCoords, widget.image(), topBorder);
    // top right corner
    rectCoords = QRect(QPoint(drawRect.left() + topLeftCorner.width() + centerSize.x(), drawRect.top()), topRightCorner.size());
    painter->drawImage(rectCoords, widget.image(), topRightCorner);
    // left
    rectCoords = QRect(drawRect.left(), drawRect.top() + topLeftCorner.height(), leftBorder.width(), centerSize.y());
    painter->drawImage(rectCoords, widget.image(), leftBorder);
    // right
    rectCoords = QRect(drawRect.left() + leftBorder.width() + centerSize.x(), drawRect.top() + topRightCorner.height(), rightBorder.width(), centerSize.y());
    painter->drawImage(rectCoords, widget.image(), rightBorder);
    // bottom left corner
    rectCoords = QRect(QPoint(drawRect.left(), drawRect.top() + topLeftCorner.height() + centerSize.y()), bottomLeftCorner.size());
    painter->drawImage(rectCoords, widget.image(), bottomLeftCorner);
    // bottom
    rectCoords = QRect(drawRect.left() + bottomLeftCorner.width(), drawRect.top() + topBorder.height() + centerSize.y(), centerSize.x(), bottomBorder.height());
    painter->drawImage(rectCoords, widget.image(), bottomBorder);
    // bottom right corner
    rectCoords = QRect(QPoint(drawRect.left() + bottomLeftCorner.width() + centerSize.x(), drawRect.top() + topRightCorner.height() + centerSize.y()), bottomRightCorner.size());
    painter->drawImage(rectCoords, widget.image(), bottomRightCorner);
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
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed)
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::TopLeft;
        }
        else
            brush = m_brushHover;
    }

    painter->fillRect(rect, brush);

    brush = m_brushNormal;
    rect.setRect(left + width - 6, top - 2, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed)
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::TopRight;
        }
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left + width - 6, top - 2, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

    brush = m_brushNormal;
    rect.setRect(left - 2, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed)
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::BottomLeft;
        }
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left - 2, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

    brush = m_brushNormal;
    rect.setRect(left + width - 6, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT);
    if(rect.contains(m_mousePos))
    {
        if(m_mousePressed)
        {
            brush = m_brushSelected;
            m_mousePressedPivot = OTUI::BottomRight;
        }
        else
            brush = m_brushHover;
    }

    painter->fillRect(QRect(left + width - 6, top + height - 6, PIVOT_WIDTH, PIVOT_HEIGHT), brush);

}
