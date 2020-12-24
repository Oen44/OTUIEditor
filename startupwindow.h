#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QDateTime>
#include <vector>
#include <algorithm>
#include "corewindow.h"
#include "recentproject.h"

namespace Ui {
class StartupWindow;
}

class StartupWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = nullptr);
    ~StartupWindow();

    void recentProjectClicked(RecentProject *widget);

private slots:
    void on_newProject_clicked();

    void on_openProject_clicked();

    void on_browsePath_clicked();

    void on_createProject_clicked();

    void on_nameInput_textEdited(const QString &arg1);

    void on_pathInput_textEdited(const QString &arg1);

    void on_dataInput_textEdited(const QString &arg1);

    void on_browseData_clicked();

    void on_backButton_clicked();

private:
    Ui::StartupWindow *ui;
    CoreWindow *coreWindow = nullptr;

    std::vector<std::unique_ptr<RecentProject>> m_recentProjects;
};

#endif // STARTUPWINDOW_H
