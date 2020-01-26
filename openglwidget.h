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
    OTUI::Widget* addWidget(OTUI::WidgetType type, QString widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder);
    OTUI::Widget* addWidgetChild(OTUI::WidgetType type, QString parentId, QString& widgetId, QString imagePath, QRect rect, QRect imageCrop, QRect imageBorder);
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
    std::unique_ptr<OTUI::Widget> initializeWidget(OTUI::WidgetType type, QString widgetId, QString imagePath6);

private:
    const uint8_t LINE_WIDTH = 2;
    const uint8_t PIVOT_WIDTH = 8;
    const uint8_t PIVOT_HEIGHT = 8;

    void drawBorderImage(QPainter *painter, OTUI::Widget const& widget);
    void drawBorderImage(QPainter *painter, OTUI::Widget const& widget, int x, int y);
    void drawOutlines(QPainter* painter, int left, int top, int width, int height);
    void drawPivots(QPainter* painter, int left, int top, int width, int height);

    std::vector<std::unique_ptr<OTUI::Widget>> m_otuiWidgets;

    QPoint m_mousePos;
    QPoint m_mousePressedPos;
    bool m_mousePressed;
    OTUI::Pivot m_mousePressedPivot;

    QBrush m_brushNormal;
    QBrush m_brushHover;
    QBrush m_brushSelected;
    QPoint offset;
};

#endif // OPENGLWIDGET_H
