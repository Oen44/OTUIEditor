#ifndef OTUIMAINWINDOW_H
#define OTUIMAINWINDOW_H

#include "widget.h"

namespace OTUI {
    class MainWindow : public Widget
    {
        Q_OBJECT

        Q_PROPERTY(QString Text READ getText WRITE setText DESIGNABLE true USER true)

    public:
        MainWindow();
        MainWindow(QString widgetId, QString imagePath);
        ~MainWindow();

    private:
        QString getText() const { return m_text; }
        void setText(const QString& text) {
            m_text = text;
        }

    public:
        void draw(QPainter* painter);

    private:
        QString m_text = "Window Title";
    };

}

#endif // OTUIMAINWINDOW_H
