#ifndef SETIDEVENT_H
#define SETIDEVENT_H

#include <QEvent>
#include <QString>

namespace OTUI
{
    class SetIdEvent : public QEvent
    {
    public:
        SetIdEvent();
        ~SetIdEvent();

    public:
        static const QEvent::Type eventType = static_cast<QEvent::Type>(1020);

        QString oldId;
        QString newId;
    };
}

#endif // SETIDEVENT_H
