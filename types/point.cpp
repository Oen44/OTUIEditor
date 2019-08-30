#include "point.h"
#include "CustomTypes.h"

#include <QRegExp>

PointProperty::PointProperty(const QString& name /*= QString()*/, QObject* propertyObject /*= 0*/, QObject* parent /*= 0*/) : Property(name, propertyObject, parent)
{
    m_width = new Property("width", this, this);
    m_height = new Property("height", this, this);
    setEditorHints("minimumWidth=-2147483647;maximumWidth=2147483647;minimumHeight=-2147483647;maximumHeight=2147483647;");
}

QVariant PointProperty::value(int role) const
{
    QVariant data = Property::value();
    if (data.isValid() && role != Qt::UserRole)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return tr("[%1, %2]").arg(data.value<Point>().width).arg(data.value<Point>().height);
        case Qt::EditRole:
            return tr("%1, %2").arg(data.value<Point>().width).arg(data.value<Point>().height);
        };
    }
    return data;
}

void PointProperty::setValue(const QVariant& value)
{
    if (value.type() == QVariant::String)
    {
        QString v = value.toString();
        QRegExp rx("([+-]?([0-9]*[\\.,])?[0-9]+(e[+-]?[0-9]+)?)");
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        int count = 0;
        int pos = 0;
        int width = 0, height = 0;
        while ((pos = rx.indexIn(v, pos)) != -1)
        {
            if (count == 0)
                width = rx.cap(1).toInt();
            else if (count == 1)
                height = rx.cap(1).toInt();
            else if (count > 1)
                break;
            ++count;
            pos += rx.matchedLength();
        }
        m_width->setProperty("width", width);
        m_height->setProperty("height", height);
        Property::setValue(QVariant::fromValue(Point(width, height)));
    }
    else
        Property::setValue(value);
}

void PointProperty::setEditorHints(const QString& hints)
{
    m_width->setEditorHints(parseHints(hints, 'w'));
    m_height->setEditorHints(parseHints(hints, 'h'));
}

int PointProperty::width() const
{
    return value().value<Point>().width;
}

void PointProperty::setWidth(int width)
{
    Property::setValue(QVariant::fromValue(Point(width, height())));
}

int PointProperty::height() const
{
    return value().value<Point>().height;
}

void PointProperty::setHeight(int height)
{
    Property::setValue(QVariant::fromValue(Point(width(), height)));
}

QString PointProperty::parseHints(const QString& hints, const QChar component )
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

