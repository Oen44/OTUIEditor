#ifndef POINTPROPERTY_H
#define POINTPROPERTY_H

#include "Property.h"

class PointProperty : public Property
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth DESIGNABLE true USER true)
    Q_PROPERTY(int height READ height WRITE setHeight DESIGNABLE true USER true)

public:
    PointProperty(const QString& name = QString(), QObject* propertyObject = nullptr, QObject* parent = nullptr);

    QVariant value(int role = Qt::UserRole) const;
    virtual void setValue(const QVariant& value);

    void setEditorHints(const QString& hints);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

private:
    QString parseHints(const QString& hints, const QChar component);

    Property*	m_width;
    Property*	m_height;
};

#endif // POINTPROPERTY_H
