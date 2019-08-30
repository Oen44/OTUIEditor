// *************************************************************************************************
//
// This code is part of the Sample Application to demonstrate the use of the QPropertyEditor library.
// It is distributed as public domain and can be modified and used by you without any limitations.
// 
// Your feedback is welcome!
//
// Author: Volker Wiendl
// Enum enhancement by Roman alias banal from qt-apps.org
//
// --------------------------------------
//
// Modified by Chen Guanzhou in 2014/5/25 for Qt5 support
//
// *************************************************************************************************

#ifndef CUSTOMTYPES_H_
#define CUSTOMTYPES_H_

#include <QVariant>

class Property;
class QObject;

struct Point
{
    Point() : width(0), height(0) {}
    Point(int _width, int _height) : width(_width), height(_height) {}
    int width, height;

    bool operator == (const Point& other) const {return width == other.width && height == other.height;}
    bool operator != (const Point& other) const {return width != other.width || height != other.height;}

};
Q_DECLARE_METATYPE(Point)

struct Vec2
{
    Vec2() : X(0), Y(0) {}
    Vec2(int x, int y) : X(x), Y(y) {}
    int X, Y;

    bool operator == (const Vec2& other) const {return X == other.X && Y == other.Y;}
    bool operator != (const Vec2& other) const {return X != other.X || Y != other.Y;}

};
Q_DECLARE_METATYPE(Vec2)

struct Vec3
{
    Vec3() : X(0), Y(0), Z(0) {}
    Vec3(int x, int y, int z) : X(x), Y(y), Z(z) {}
    int X, Y, Z;

    bool operator == (const Vec3& other) const {return X == other.X && Y == other.Y && Z == other.Z;}
    bool operator != (const Vec3& other) const {return X != other.X || Y != other.Y || Z != other.Z;}

};
Q_DECLARE_METATYPE(Vec3)


namespace CustomTypes
{
	void registerTypes();
	Property* createCustomProperty(const QString& name, QObject* propertyObject, Property* parent);

}
#endif
