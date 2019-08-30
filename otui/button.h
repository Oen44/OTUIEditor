#ifndef OTUIBUTTON_H
#define OTUIBUTTON_H

#include "widget.h"

namespace OTUI {
    class Button : public Widget
    {
        Q_OBJECT

        Q_PROPERTY(QString Text READ getText WRITE setText DESIGNABLE true USER true)

    public:
        Button();
        Button(QString widgetId, QString imagePath);
        ~Button();

    private:
        QString getText() const { return m_text; }
        void setText(const QString& text) {
            m_text = text;
        }

    public:
        void draw(QPainter* painter);

    private:
        QString m_text = "Button";
    };
}

#endif // OTUIBUTTON_H
