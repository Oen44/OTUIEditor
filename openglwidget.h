#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "otuiwidget.h"
#include <QPainter>
#include <QOpenGLWidget>
#include <QTime>

class CWidget;

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

private:
    const uint8_t LINE_WIDTH = 2;
    const uint8_t PIVOT_WIDTH = 8;
    const uint8_t PIVOT_HEIGHT = 8;

    void draw();
    void drawBorderImage(QPainter *painter, OTUI::CWidget* widget);
    void drawBorderImage(QPainter *painter, OTUI::CWidget* widget, int x, int y);
    void drawOutlines(QPainter* painter, int left, int top, int width, int height);
    void drawPivots(QPainter* painter, int left, int top, int width, int height);
    bool checkChildrenOverlap(OTUI::CWidget* parent);
    void drawWidgetChildren(QPainter* painter, OTUI::CWidget* parent);

    QVector<OTUI::CWidget*> m_otuiWidgets;
    OTUI::CWidget* m_selected;

    QPoint m_mousePos;
    QPoint m_mousePressedPos;
    bool m_mousePressed;

    QBrush m_brushNormal;
    QBrush m_brushHover;
    QBrush m_brushSelected;
    QPoint offset;
};

#endif // OPENGLWIDGET_H
