#ifndef SETTINGSSAVEDEVENT_H
#define SETTINGSSAVEDEVENT_H

#include <QEvent>
#include <QVariant>

class SettingsSavedEvent : public QEvent
{
public:
    SettingsSavedEvent();
    ~SettingsSavedEvent();

public:
    static const QEvent::Type eventType = static_cast<QEvent::Type>(1021);
};
#endif // SETTINGSSAVEDEVENT_H
