#ifndef VECTOR3_H
#define VECTOR3_H

#include "Property.h"

class Vec3Property : public Property
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX DESIGNABLE true USER true)
    Q_PROPERTY(int y READ y WRITE setY DESIGNABLE true USER true)
    Q_PROPERTY(int z READ z WRITE setZ DESIGNABLE true USER true)

public:
    Vec3Property(const QString& name = QString(), QObject* propertyObject = nullptr, QObject* parent = nullptr);

    QVariant value(int role = Qt::UserRole) const;
    virtual void setValue(const QVariant& value);

    void setEditorHints(const QString& hints);

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

    int z() const;
    void setZ(int z);

private:
    QString parseHints(const QString& hints, const QChar component);

    Property*	m_x;
    Property*	m_y;
    Property*	m_z;
};

#endif // VECTOR3_H
