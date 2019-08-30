#include "vector2.h"
#include "CustomTypes.h"

#include <QRegExp>

Vec2Property::Vec2Property(const QString& name /*= QString()*/, QObject* propertyObject /*= 0*/, QObject* parent /*= 0*/) : Property(name, propertyObject, parent)
{
    m_x = new Property("x", this, this);
    m_y = new Property("y", this, this);
    setEditorHints("minimumX=-2147483647;maximumX=2147483647;minimumY=-2147483647;maximumY=2147483647;");
}

QVariant Vec2Property::value(int role) const
{
    QVariant data = Property::value();
    if (data.isValid() && role != Qt::UserRole)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return tr("[%1, %2]").arg(data.value<Vec2>().X).arg(data.value<Vec2>().Y);
        case Qt::EditRole:
            return tr("%1, %2").arg(data.value<Vec2>().X).arg(data.value<Vec2>().Y);
        };
    }
    return data;
}

void Vec2Property::setValue(const QVariant& value)
{
    if (value.type() == QVariant::String)
    {
        QString v = value.toString();
        QRegExp rx("([+-]?([0-9]*[\\.,])?[0-9]+(e[+-]?[0-9]+)?)");
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        int count = 0;
        int pos = 0;
        int x = 0, y = 0;
        while ((pos = rx.indexIn(v, pos)) != -1)
        {
            if (count == 0)
                x = rx.cap(1).toInt();
            else if (count == 1)
                y = rx.cap(1).toInt();
            else if (count > 1)
                break;
            ++count;
            pos += rx.matchedLength();
        }
        m_x->setProperty("x", x);
        m_y->setProperty("y", y);
        Property::setValue(QVariant::fromValue(Vec2(x, y)));
    }
    else
        Property::setValue(value);
}

void Vec2Property::setEditorHints(const QString& hints)
{
    m_x->setEditorHints(parseHints(hints, 'X'));
    m_y->setEditorHints(parseHints(hints, 'Y'));
}

int Vec2Property::x() const
{
    return value().value<Vec2>().X;
}

void Vec2Property::setX(int x)
{
    Property::setValue(QVariant::fromValue(Vec2(x, y())));
}

int Vec2Property::y() const
{
    return value().value<Vec2>().Y;
}

void Vec2Property::setY(int y)
{
    Property::setValue(QVariant::fromValue(Vec2(x(), y)));
}

QString Vec2Property::parseHints(const QString& hints, const QChar component )
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

