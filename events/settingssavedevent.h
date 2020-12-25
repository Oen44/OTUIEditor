#ifndef SETTINGSSAVEDEVENT_H
#define SETTINGSSAVEDEVENT_H

#include <QEvent>
#include <QVariant>

#include "projectsettings.h"

class SettingsSavedEvent : public QEvent
{
public:
    SettingsSavedEvent();
    ~SettingsSavedEvent();

public:
    static const QEvent::Type eventType = static_cast<QEvent::Type>(1021);

    QString dataPath;
};
#endif // SETTINGSSAVEDEVENT_H
