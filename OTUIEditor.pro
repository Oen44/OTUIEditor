#-------------------------------------------------
#
# Project created by QtCreator 2019-08-22T14:29:26
#
#-------------------------------------------------

QT       += core opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OTUIEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14 debug_and_release

SOURCES += \
        corewindow.cpp \
        events/setidevent.cpp \
        main.cpp \
        openglwidget.cpp \
        otui/button.cpp \
        otui/creature.cpp \
        otui/item.cpp \
        otui/label.cpp \
        otui/mainwindow.cpp \
        otui/project.cpp \
        otui/widget.cpp \
        recentproject.cpp \
        startupwindow.cpp \
        types/CustomTypes.cpp \
        types/point.cpp \
        types/vector2.cpp \
        types/vector3.cpp

HEADERS += \
        Property.h \
        const.h \
        corewindow.h \
        events/setidevent.h \
        openglwidget.h \
        otui/button.h \
        otui/creature.h \
        otui/item.h \
        otui/label.h \
        otui/mainwindow.h \
        otui/otui.h \
        otui/project.h \
        otui/widget.h \
        qpropertyeditorwidget.h \
        recentproject.h \
        startupwindow.h \
        types/CustomTypes.h \
        types/point.h \
        types/vector2.h \
        types/vector3.h

FORMS += \
        mainwindow.ui \
        startupwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    stylesheet.css

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lQPropertyEditor -lOpengl32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lQPropertyEditord -lOpengl32
else:unix: LIBS += -L$$PWD/lib/ -lQPropertyEditor -lOpengl32

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
