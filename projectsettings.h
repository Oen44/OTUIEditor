#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>

#include "events/settingssavedevent.h"

class ProjectSettings : public QFrame
{
    Q_OBJECT
public:
    explicit ProjectSettings(QWidget *parent = nullptr);
    ~ProjectSettings();

private slots:
    void handleCloseButton();
    void handleDataBrowse();
    void handleSaveButton();

public:
    void setProjectName(QString name) {
        projectNameInput->setText(name);
    }

    QString getProjectName() const {
        return projectNameInput->text();
    }

    void setDataPath(QString name) {
        dataPathInput->setText(name);
    }

    QString getDataPath() const {
        return dataPathInput->text();
    }

private:
    void addProjectName(QVBoxLayout *contentLayout);
    void addDataPath(QVBoxLayout *contentLayout);
    void addSaveButton(QVBoxLayout *contentLayout);

private:
    QFrame *topBar;
    QLabel *titleLabel;
    QPushButton *closeButton;
    QWidget *contentPanel;
    QLineEdit *projectNameInput;
    QLineEdit *dataPathInput;

};

#endif // PROJECTSETTINGS_H
