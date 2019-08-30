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


#include "CustomTypes.h"
#include "point.h"
#include "vector2.h"
#include "vector3.h"

#include "Property.h"

namespace CustomTypes
{
	void registerTypes()
	{
		static bool registered = false;
		if (!registered)
		{
            qRegisterMetaType<Point>("Point");
            qRegisterMetaType<Vec2>("Vec2");
            qRegisterMetaType<Vec3>("Vec3");
			registered = true;
		}
	}

	Property* createCustomProperty(const QString& name, QObject* propertyObject, Property* parent)
	{
		int userType = propertyObject->property(qPrintable(name)).userType();
        if (userType == QMetaType::type("Point"))
            return new PointProperty(name, propertyObject, parent);
        else if (userType == QMetaType::type("Vec2"))
            return new Vec2Property(name, propertyObject, parent);
        else if (userType == QMetaType::type("Vec3"))
                return new Vec3Property(name, propertyObject, parent);
		else
			return new Property(name, propertyObject, parent);
	}
}
