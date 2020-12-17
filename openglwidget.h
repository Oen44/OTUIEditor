#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <memory>
#include "const.h"
#include "otui/otui.h"
#include "corewindow.h"
#include <QPainter>
#include <QOpenGLWidget>
#include <QTime>

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);
    ~OpenGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent* event);

public:
    template <class T>
    OTUI::Widget* addWidget(QString widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder)
    {
        std::unique_ptr<OTUI::Widget> widget = initializeWidget<T>(widgetId, imagePath);

        if(widget == nullptr)
        {
            CoreWindow::ShowError("Error", QString("Can't add %1 widget.\nWidget type is incorrect.").arg(widgetId));
            return nullptr;
        }

        m_selected = widget.get();

        widget->setRect(rect);
        widget->setImageCrop(imageCrop);
        widget->setImageBorder(imageBorder);
        m_otuiWidgets.emplace_back(std::move(widget));

        return m_selected;
    }

    template <class T>
    OTUI::Widget* addWidgetChild(QString parentId, QString& widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder)
    {
        OTUI::Widget* parent = nullptr;
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
            CoreWindow::ShowError("Error", QString("Couldn't add %1 widget.\nParent with id %1 not found.").arg(widgetId).arg(parentId));
            return nullptr;
        }

        std::unique_ptr<OTUI::Widget> widget = initializeWidget<T>(widgetId, imagePath);

        if(widget == nullptr)
        {
            CoreWindow::ShowError("Error", QString("Couldn't add %1 widget.\nWidget type is incorrect.").arg(widgetId));
            return nullptr;
        }

        m_selected = widget.get();

        widget->setRect(rect);
        widget->setImageCrop(imageCrop);
        widget->setImageBorder(imageBorder);
        widget->setParent(parent);
        setInBounds(widget.get(), QPoint());
        m_otuiWidgets.emplace_back(std::move(widget));

        return m_selected;
    }

    std::vector<std::unique_ptr<OTUI::Widget>> const& getOTUIWidgets() const { return m_otuiWidgets; }
    void deleteWidget(QString widgetId) {
        auto itr = std::find_if(std::begin(m_otuiWidgets),
                                std::end(m_otuiWidgets),
                                [widgetId](auto &element) { return element.get()->getId() == widgetId;});
        m_otuiWidgets.erase(itr);
        m_selected = nullptr;
    }
    void clearWidgets() {
        m_selected = nullptr;
        m_otuiWidgets.clear();
    }

    OTUI::Widget* m_selected = nullptr;

    double scale;

private:
    template <class T>
    std::unique_ptr<T> initializeWidget(QString widgetId, QString imagePath)
    {
        std::unique_ptr<T> widget = std::make_unique<T>(widgetId, imagePath);

        uint8_t found = 0;

        for(auto const& w : m_otuiWidgets)
        {
            if(w->getId() == widgetId)
                found++;
            else if(w->getId() == QString("%1_%2").arg(widgetId).arg(found))
                found++;
        }

        if(found > 0)
        {
            widget->setId(QString("%1_%2").arg(widgetId).arg(found));
        }

        return widget;
    }

    void setInBounds(OTUI::Widget* widget, QPoint newPos)
    {
        OTUI::Widget* parent = widget->getParent();
        QRect parentBorder = QRect();
        if(parent != nullptr)
        {
            parentBorder = parent->getImageBorder();
            widget->setPos(newPos);

            if(widget->x() < parentBorder.x())
                newPos.setX(parentBorder.x());
            if(widget->y() < parentBorder.y())
                newPos.setY(parentBorder.y());

            if(widget->x() + widget->width() > parent->width() - parentBorder.width())
                newPos.setX(parent->width() - widget->width() - parentBorder.width());
            if(widget->y() + widget->height() > parent->height() - parentBorder.height())
                newPos.setY(parent->height() - widget->height() - parentBorder.height());

            widget->setPos(newPos);
        }
    }

private:
    const uint8_t LINE_WIDTH = 2;
    const uint8_t PIVOT_WIDTH = 8;
    const uint8_t PIVOT_HEIGHT = 8;

    void drawBorderImage(QPainter &painter, OTUI::Widget const& widget);
    void drawBorderImage(QPainter &painter, OTUI::Widget const& widget, int x, int y);
    void drawOutlines(QPainter &painter, int left, int top, int width, int height);
    void drawPivots(QPainter &painter, int left, int top, int width, int height);

    std::vector<std::unique_ptr<OTUI::Widget>> m_otuiWidgets;

    QPoint m_mousePos;
    QPoint m_mousePressedPos;
    bool m_mousePressed;
    OTUI::Pivot m_mousePressedPivot;

    QBrush m_brushNormal;
    QBrush m_brushHover;
    QBrush m_brushSelected;
    QPoint offset;

    QPixmap m_background;

    QTimer *pTimer;
};

#endif // OPENGLWIDGET_H
