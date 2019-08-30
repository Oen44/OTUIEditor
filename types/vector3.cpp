#include "vector3.h"
#include "CustomTypes.h"

#include <QRegExp>

Vec3Property::Vec3Property(const QString& name /*= QString()*/, QObject* propertyObject /*= 0*/, QObject* parent /*= 0*/) : Property(name, propertyObject, parent)
{
    m_x = new Property("x", this, this);
    m_y = new Property("y", this, this);
    m_z = new Property("z", this, this);
    setEditorHints("minimumX=-2147483647;maximumX=2147483647;minimumY=-2147483647;maximumY=2147483647;minimumZ=-2147483647;maximumZ=2147483647;");
}

QVariant Vec3Property::value(int role) const
{
    QVariant data = Property::value();
    if (data.isValid() && role != Qt::UserRole)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return tr("[%1, %2, %3]").arg(data.value<Vec3>().X).arg(data.value<Vec3>().Y).arg(data.value<Vec3>().Z);
        case Qt::EditRole:
            return tr("%1, %2, %3").arg(data.value<Vec3>().X).arg(data.value<Vec3>().Y).arg(data.value<Vec3>().Z);
        };
    }
    return data;
}

void Vec3Property::setValue(const QVariant& value)
{
    if (value.type() == QVariant::String)
    {
        QString v = value.toString();
        QRegExp rx("([+-]?([0-9]*[\\.,])?[0-9]+(e[+-]?[0-9]+)?)");
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        int count = 0;
        int pos = 0;
        int x = 0, y = 0, z = 0;
        while ((pos = rx.indexIn(v, pos)) != -1)
        {
            if (count == 0)
                x = rx.cap(1).toInt();
            else if (count == 1)
                y = rx.cap(1).toInt();
            else if (count == 2)
                z = rx.cap(1).toInt();
            else if (count > 2)
                break;
            ++count;
            pos += rx.matchedLength();
        }
        m_x->setProperty("x", x);
        m_y->setProperty("y", y);
        m_z->setProperty("z", z);
        Property::setValue(QVariant::fromValue(Vec3(x, y, z)));
    }
    else
        Property::setValue(value);
}

void Vec3Property::setEditorHints(const QString& hints)
{
    m_x->setEditorHints(parseHints(hints, 'X'));
    m_y->setEditorHints(parseHints(hints, 'Y'));
    m_z->setEditorHints(parseHints(hints, 'Z'));
}

int Vec3Property::x() const
{
    return value().value<Vec3>().X;
}

void Vec3Property::setX(int x)
{
    Property::setValue(QVariant::fromValue(Vec3(x, y(), z())));
}

int Vec3Property::y() const
{
    return value().value<Vec3>().Y;
}

void Vec3Property::setY(int y)
{
    Property::setValue(QVariant::fromValue(Vec3(x(), y, z())));
}

int Vec3Property::z() const
{
    return value().value<Vec3>().Z;
}

void Vec3Property::setZ(int z)
{
    Property::setValue(QVariant::fromValue(Vec3(x(), y(), z)));
}

QString Vec3Property::parseHints(const QString& hints, const QChar component )
{
    QRegExp rx(QString("(.*)(")+component+QString("{1})(=\\s*)(.*)(;{1})"));
    rx.setMinimal(true);
    int pos = 0;
    QString componentHints;
    while ((pos = rx.indexIn(hints, pos)) != -1)
    {
        // cut off additional front settings (TODO create correct RegExp for that)
        if (rx.cap(1).lastIndexOf(';') != -1)
            componentHints += QString("%1=%2;").arg(rx.cap(1).remove(0, rx.cap(1).lastIndexOf(';')+1)).arg(rx.cap(4));
        else
            componentHints += QString("%1=%2;").arg(rx.cap(1)).arg(rx.cap(4));
        pos += rx.matchedLength();
    }
    return componentHints;
}

