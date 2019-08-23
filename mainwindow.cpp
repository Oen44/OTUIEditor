#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/settings.ini";

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = settings.value("text", "").toString();
}

void MainWindow::saveSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    settings.setValue("text", "");
}
