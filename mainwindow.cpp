#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDebug>

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

void MainWindow::ShowError(QString title, QString description)
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, title, description);
    messageBox.setFixedSize(300, 80);
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

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    ui->actionDeleteWidget->setEnabled(ui->treeWidget->itemAt(pos) != nullptr);

    menu.addAction(ui->actionDeleteWidget);

    menu.addSeparator();

    QMenu* newMenu = menu.addMenu("New...");

    newMenu->addAction(ui->newMainWindow);
    newMenu->addAction(ui->newButton);
    newMenu->addAction(ui->newLabel);
    newMenu->addSeparator();

    QMenu* customMenu = newMenu->addMenu("Custom");
    customMenu->setDisabled(true);
    customMenu->addAction(ui->newButton);

    if(ui->treeWidget->itemAt(pos))
        ui->actionDeleteWidget->setData(QVariant(pos));

    menu.exec( ui->treeWidget->mapToGlobal(pos) );
}

void MainWindow::on_newMainWindow_triggered()
{
    ui->openGLWidget->addWidget(OTUI::MainWindow, "mainWindow", ":/images/main_window.png", QRect(50, 50, 256, 256), QRect(0, 0, 256, 256), QRect(6, 27, 6, 6));
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, QString("mainWindow (MainWindow)"));
    treeItem->setSelected(true);
    ui->treeWidget->expandItem(treeItem);
}

void MainWindow::on_newButton_triggered()
{
    QTreeWidgetItem* clickedItem = ui->treeWidget->itemAt(ui->actionDeleteWidget->data().toPoint());
    ui->openGLWidget->addWidgetChild(OTUI::Button, "mainWindow", "button", ":/images/button_rounded.png", QRect(50, 50, 106, 23), QRect(0, 0, 22, 23), QRect(5, 5, 5, 5));
    QTreeWidgetItem* child = new QTreeWidgetItem();
    child->setText(0, QString("button (Button)"));
    child->setSelected(true);
    clickedItem->addChild(child);
}

void MainWindow::on_newLabel_triggered()
{
    QTreeWidgetItem* clickedItem = ui->treeWidget->itemAt(ui->actionDeleteWidget->data().toPoint());
    ui->openGLWidget->addWidgetChild(OTUI::Label, "mainWindow", "label", nullptr, QRect(0, 0, 106, 23), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
    QTreeWidgetItem* child = new QTreeWidgetItem();
    child->setText(0, QString("label (Label)"));
    child->setSelected(true);
    clickedItem->addChild(child);
}

void MainWindow::on_actionDeleteWidget_triggered()
{
   QTreeWidgetItem* clickedItem = ui->treeWidget->itemAt(ui->actionDeleteWidget->data().toPoint());
   delete clickedItem;
}
