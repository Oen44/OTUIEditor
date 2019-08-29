#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "otui/otui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void ShowError(QString title, QString description);

private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void on_newMainWindow_triggered();

    void on_newButton_triggered();

    void on_newLabel_triggered();

    void on_actionDeleteWidget_triggered();

private:
    Ui::MainWindow *ui;

    void loadSettings();
    void saveSettings();
    QString m_sSettingsFile;
};

#endif // MAINWINDOW_H
