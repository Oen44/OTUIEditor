#ifndef VECTOR2_H
#define VECTOR2_H

#include "Property.h"

class Vec2Property : public Property
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX DESIGNABLE true USER true)
    Q_PROPERTY(int y READ y WRITE setY DESIGNABLE true USER true)

public:
    Vec2Property(const QString& name = QString(), QObject* propertyObject = nullptr, QObject* parent = nullptr);

    QVariant value(int role = Qt::UserRole) const;
    virtual void setValue(const QVariant& value);

    void setEditorHints(const QString& hints);

    int x() const;
    void setX(int x);

    int y() const;
    void setY(int y);

private:
    QString parseHints(const QString& hints, const QChar component);

    Property*	m_x;
    Property*	m_y;
};

#endif // VECTOR2_H
