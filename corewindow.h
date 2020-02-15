#ifndef COREWINDOW_H
#define COREWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include "otui/otui.h"
#include "events/setidevent.h"

namespace Ui {
class MainWindow;
}

class CoreWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CoreWindow(QWidget *parent = nullptr);
    ~CoreWindow();
    static void ShowError(QString title, QString description);

    void startNewProject(QString name, QString path);
    void loadProjectData(QDataStream& data, QString path);

private slots:
    void on_newMainWindow_triggered();

    void on_newButton_triggered();

    void on_newLabel_triggered();

    void on_actionDeleteWidget_triggered();

    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void on_actionNewProject_triggered();

    void on_actionSaveProject_triggered();

    void on_actionCloseProject_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_newUIItem_triggered();

    void on_newUICreature_triggered();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void keyReleaseEvent(QKeyEvent* event);
    bool event(QEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow *ui;

    OTUI::Project m_Project;

    QStandardItemModel* model = nullptr;

    void addChildToTree(QString label);
    void selectWidgetById(QString widgetId);

    void loadSettings();
    void saveSettings();
    QString m_sSettingsFile;

    OTUI::Widget* m_selected = nullptr;
};

#endif // COREWINDOW_H
