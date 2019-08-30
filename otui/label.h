#ifndef OTUILABEL_H
#define OTUILABEL_H

#include <QDebug>
#include <QApplication>
#include "widget.h"

namespace OTUI {
    class Label : public Widget
    {
        Q_OBJECT

        Q_PROPERTY(QString Text READ getText WRITE setText DESIGNABLE true USER true)
        Q_PROPERTY(QColor Color READ getColor WRITE setColor DESIGNABLE true USER true)

    public:
        Label();
        Label(QString widgetId, QString imagePath);
        ~Label();

    private:
        QString getText() const { return m_text; }
        void setText(const QString& text) {
            m_text = text;
        }

        QColor getColor() const { return m_color; }
        void setColor(const QColor& color) {
            m_color = color;
        }

    public:
        void draw(QPainter* painter);

    private:
        QString m_text = "Label";
        QColor m_color = QColor("#dfdfdf");
    };
}

#endif // OTUILABEL_H
