#ifndef OTUI_H
#define OTUI_H

#include "cmainwindow.h"
#include "cbutton.h"
#include "clabel.h"

class CMainWindow;
class CButton;
class CLabel;

namespace OTUI {
    enum WidgetType {
        MainWindow,
        Button,
        Label
    };
}

#endif // OTUI_H
