#ifndef OTUICONST_H
#define OTUICONST_H

#include "project.h"
#include "widget.h"
#include "mainwindow.h"
#include "button.h"
#include "label.h"
#include "item.h"
#include "creature.h"

namespace OTUI {
    class Project;
    class Widget;
    class MainWindow;
    class Button;
    class Label;
    enum WidgetType {
        MainWindowType,
        ButtonType,
        LabelType,
        ItemType,
        CreatureType
    };
}

#endif // OTUICONST_H
