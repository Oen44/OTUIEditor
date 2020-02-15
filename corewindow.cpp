#include "corewindow.h"
#include "ui_mainwindow.h"
#include "types/CustomTypes.h"
#include "startupwindow.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>

CoreWindow::CoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //m_sSettingsFile = QApplication::applicationDirPath().left(1) + ":/settings.ini";
    ui->setupUi(this);
    ui->openGLWidget->installEventFilter(this);

    CustomTypes::registerTypes();
    ui->propertyEditor->registerCustomPropertyCB(CustomTypes::createCustomProperty);

    model = new QStandardItemModel;
    ui->treeView->setModel(model);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection&) {
        QStandardItem* newitem = model->itemFromIndex(selected.indexes().first());
        m_selected = nullptr;

        selectWidgetById(newitem->text());

        if(m_selected != nullptr)
        {
            ui->openGLWidget->m_selected = m_selected;
            ui->propertyEditor->setObject(m_selected);
        }
    } );
}

CoreWindow::~CoreWindow()
{
    delete ui;
}

void CoreWindow::ShowError(QString title, QString description)
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, title, description);
    messageBox.setFixedSize(300, 80);
}

void CoreWindow::startNewProject(QString name, QString path)
{
    OTUI::Project m_Project(name, path);

    if(!m_Project.loaded())
        return;

    setWindowTitle(name + " - OTUI Editor");
}

void CoreWindow::loadProjectData(QDataStream& data, QString path)
{
    OTUI::Project m_Project(data, path);

    if(!m_Project.loaded())
        return;

    // TODO: Initialize widgets

    setWindowTitle(m_Project.getProjectName() + " - OTUI Editor");
}

void CoreWindow::loadSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString sText = settings.value("text", "").toString();
}

void CoreWindow::saveSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    settings.setValue("text", "");
}

void CoreWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    ui->actionDeleteWidget->setEnabled(ui->treeView->indexAt(pos).isValid());

    menu.addAction(ui->actionDeleteWidget);

    menu.addSeparator();

    QMenu* newMenu = menu.addMenu("New...");

    newMenu->addAction(ui->newMainWindow);
    newMenu->addAction(ui->newButton);
    newMenu->addAction(ui->newLabel);
    newMenu->addAction(ui->newUIItem);
    newMenu->addAction(ui->newUICreature);
    newMenu->addSeparator();

    QMenu* customMenu = newMenu->addMenu("Custom");
    customMenu->setDisabled(true);
    customMenu->addAction(ui->newButton);

    menu.exec(ui->treeView->mapToGlobal(pos));
}

void CoreWindow::on_actionDeleteWidget_triggered()
{
    auto idx = ui->treeView->currentIndex();
    if(idx.parent().isValid())
    {
        QStandardItem* item = model->itemFromIndex(idx);
        ui->openGLWidget->deleteWidget(item->text());
        model->removeRow(idx.row(), idx.parent());
        QStandardItem* root = model->invisibleRootItem();
        QModelIndex index = root->child(0)->index();
        ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        selectWidgetById(root->child(0)->text());
        ui->propertyEditor->setObject(m_selected);
    }
    else
    {
        model->clear();
        ui->openGLWidget->clearWidgets();
        m_selected = nullptr;
        ui->propertyEditor->clear();
    }
}

void CoreWindow::addChildToTree(QString label)
{
    QModelIndex index = ui->treeView->currentIndex();
    QStandardItem* item = model->itemFromIndex(index);
    QStandardItem* newItem = new QStandardItem(label);
    newItem->setEditable(false);
    item->appendRow(newItem);
    ui->treeView->expand(index);
    ui->treeView->selectionModel()->select(newItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->treeView->selectionModel()->setCurrentIndex(newItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    selectWidgetById(newItem->text());
}

void CoreWindow::selectWidgetById(QString widgetId)
{
    for(auto& widget : ui->openGLWidget->getOTUIWidgets())
    {
        if(widget->getId() == widgetId)
        {
            m_selected = widget.get();
            break;
        }
    }
}

bool CoreWindow::eventFilter(QObject*, QEvent* event)
{
    if(event->type() == QEvent::MouseButtonRelease) {
        if(m_selected != nullptr)
        {
            if(ui->openGLWidget->m_selected != nullptr)
            {
                if(ui->openGLWidget->m_selected != m_selected)
                {
                    m_selected = ui->openGLWidget->m_selected;
                    QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(m_selected->getId()), 1, Qt::MatchRecursive);
                    if(!items.isEmpty())
                    {
                        ui->treeView->selectionModel()->select(items.at(0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                        ui->treeView->selectionModel()->setCurrentIndex(items.at(0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                        ui->propertyEditor->setObject(m_selected);
                    }
                }
                else
                {
                    ui->propertyEditor->updateObject(ui->openGLWidget->m_selected);
                }
            }
            else
            {
                ui->propertyEditor->updateObject(m_selected);
            }
        }
    }
    return false;
}

void CoreWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {

    case Qt::Key_Delete:
    {
        auto idx = ui->treeView->currentIndex();
        if(idx.parent().isValid())
        {
            QStandardItem* item = model->itemFromIndex(idx);
            ui->openGLWidget->deleteWidget(item->text());
            model->removeRow(idx.row(), idx.parent());
            QStandardItem* root = model->invisibleRootItem();
            QModelIndex index = root->child(0)->index();
            ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            selectWidgetById(root->child(0)->text());
            ui->propertyEditor->setObject(m_selected);
        }
        else
        {
            model->clear();
            ui->openGLWidget->clearWidgets();
            m_selected = nullptr;
            ui->propertyEditor->clear();
        }
        break;
    }

    }
}

bool CoreWindow::event(QEvent* event)
{
    if(event->type() == OTUI::SetIdEvent::eventType)
    {
        OTUI::SetIdEvent* setIdEvent = reinterpret_cast<OTUI::SetIdEvent*>(event);
        QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(setIdEvent->oldId), 1, Qt::MatchRecursive);
        if(!items.isEmpty())
        {
            model->itemFromIndex(items.at(0))->setText(setIdEvent->newId);
        }
    }

    return QMainWindow::event(event);
}

void CoreWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox box;
    QMessageBox::StandardButton response = box.question(this, "Save Changes",
                 "Do you want to save this project before closing?",
                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                 QMessageBox::Yes);

    if(response == QMessageBox::Yes)
    {
        // TODO: Save project
        event->accept();
    }
    else if(response == QMessageBox::No)
        event->accept();
    else
        event->ignore();
}

void CoreWindow::on_actionNewProject_triggered()
{
    // TODO: ask to save current project

    if(m_Project.getProjectFile()->isOpen())
    {
        m_Project.getProjectFile()->close();
    }

    // Clear tree
    model->clear();

    // Clear properties
    ui->propertyEditor->clear();

    // Clear selected
    m_selected = nullptr;

    // Clear widgets
    ui->openGLWidget->clearWidgets();
}

void CoreWindow::on_actionSaveProject_triggered()
{
    // Save changes
}

void CoreWindow::on_actionCloseProject_triggered()
{
    if(m_Project.isChanged())
    {
        QMessageBox box;
        QMessageBox::StandardButton response = box.question(this, "Save Changes",
                     "Do you want to save this project before closing?",
                     QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                     QMessageBox::Yes);

        if(response == QMessageBox::Yes)
        {
            // TODO: Save changes
        }
        else if(response == QMessageBox::Cancel)
            return;
    }

    m_Project.getProjectFile()->close();
    StartupWindow* w = new StartupWindow();
    w->show();
    hide();

}

void CoreWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->openGLWidget->scale = value / 100.0;
    ui->zoomLabel->setText(QString::number(value) + "%");
}

void CoreWindow::on_newMainWindow_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    QString widgetId("mainWindow");
    if(index.isValid())
    {
        addChildToTree(widgetId);
    }
    else
    {
        QStandardItem* root = model->invisibleRootItem();
        QStandardItem* item = new QStandardItem(widgetId);
        item->setEditable(false);
        root->appendRow(item);
        model->setHeaderData(0, Qt::Horizontal, "Widgets List");
        ui->treeView->selectionModel()->select(item->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->treeView->selectionModel()->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        selectWidgetById(item->text());
    }

    m_selected = ui->openGLWidget->addWidget(OTUI::MainWindowType, widgetId, ":/images/main_window.png", QRect(50, 50, 256, 256), QRect(0, 0, 256, 256), QRect(6, 27, 6, 6));
    if(m_selected != nullptr)
    {
        ui->propertyEditor->addObject(m_selected);
    }
}

void CoreWindow::on_newButton_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("button");
        m_selected = ui->openGLWidget->addWidgetChild(OTUI::ButtonType, "mainWindow", widgetId, ":/images/button_rounded.png", QRect(50, 50, 106, 23), QRect(0, 0, 22, 23), QRect(5, 5, 5, 5));
        if(m_selected != nullptr)
        {
            ui->propertyEditor->setObject(m_selected);
        }
        addChildToTree(m_selected->getId());
    }
}

void CoreWindow::on_newLabel_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("label");
        m_selected = ui->openGLWidget->addWidgetChild(OTUI::LabelType, "mainWindow", widgetId, nullptr, QRect(0, 0, 106, 23), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        if(m_selected != nullptr)
        {
            ui->propertyEditor->setObject(m_selected);
        }
        addChildToTree(m_selected->getId());
    }
}

void CoreWindow::on_newUIItem_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("item");
        m_selected = ui->openGLWidget->addWidgetChild(OTUI::ItemType, "mainWindow", widgetId, nullptr, QRect(0, 0, 32, 32), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        if(m_selected != nullptr)
        {
            ui->propertyEditor->setObject(m_selected);
        }
        addChildToTree(m_selected->getId());
    }
}

void CoreWindow::on_newUICreature_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("creature");
        m_selected = ui->openGLWidget->addWidgetChild(OTUI::CreatureType, "mainWindow", widgetId, nullptr, QRect(0, 0, 48, 48), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        if(m_selected != nullptr)
        {
            ui->propertyEditor->setObject(m_selected);
        }
        addChildToTree(m_selected->getId());
    }
}
