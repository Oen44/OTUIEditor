#include "corewindow.h"
#include "ui_mainwindow.h"
#include "startupwindow.h"

#include <QSettings>
#include <QDebug>
#include <QFileDialog>

CoreWindow::CoreWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
}

CoreWindow::~CoreWindow()
{
    delete std::exchange(imagesBrowser, nullptr);
    delete std::exchange(m_projectSettings, nullptr);
    delete ui;
}

void CoreWindow::initializeWindow()
{
    setMinimumSize(860, 600);
    ui->setupUi(this);
    ui->openGLWidget->installEventFilter(this);

    model = new QStandardItemModel;
    ui->treeView->setModel(model);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection&) {
        QStandardItem *newitem = model->itemFromIndex(selected.indexes().first());
        m_selected = nullptr;

        selectWidgetById(newitem->text());

        if(m_selected != nullptr)
        {
            ui->openGLWidget->m_selected = m_selected;
        }
    });

    imagesBrowser = new ImageSourceBrowser(ui->centralWidget);
    imagesBrowser->hide();

    ElidedLabel *imageSourceLabel = new ElidedLabel("/images/ui/window.png", ui->ispContent);
    ui->ispContent->layout()->addWidget(imageSourceLabel);

    QPushButton *imageSourceOpen = new QPushButton("~", ui->ispContent);
    imageSourceOpen->setFixedWidth(20);
    imageSourceOpen->setFlat(true);
    imageSourceOpen->setCursor(Qt::PointingHandCursor);
    ui->ispContent->layout()->addWidget(imageSourceOpen);

    connect(imageSourceOpen, &QPushButton::pressed, [=]() {
        imagesBrowser->show();
        imagesBrowser->move(this->rect().center() - imagesBrowser->rect().center());
    });

    m_projectSettings = new ProjectSettings(this);
    m_projectSettings->hide();
}

void CoreWindow::ShowError(QString title, QString description)
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, title, description);
    messageBox.setFixedSize(300, 80);
}

void CoreWindow::startNewProject(QString fileName, QString name, QString path, QString dataPath)
{
    m_Project = new OTUI::Project(fileName, name, path, dataPath);

    if(!m_Project->loaded())
        return;

    initializeWindow();
    setWindowTitle(name + " - OTUI Editor");
    m_projectSettings->setProjectName(name);
    m_projectSettings->setDataPath(dataPath);
    imagesBrowser->m_DataPath = m_Project->getDataPath();
    imagesBrowser->initialize();
}

void CoreWindow::loadProjectData(QDataStream &data, QString fileName, QString path)
{
    m_Project = new OTUI::Project(data, fileName, path);

    if(!m_Project->loaded())
        return;

    initializeWindow();
    // TODO: Initialize widgets

    setWindowTitle(m_Project->getProjectName() + " - OTUI Editor");
    m_projectSettings->setProjectName(m_Project->getProjectName());
    m_projectSettings->setDataPath(m_Project->getDataPath());
    imagesBrowser->m_DataPath = m_Project->getDataPath();
    imagesBrowser->initialize();
}

bool CoreWindow::event(QEvent *event)
{
    if(event->type() == SetIdEvent::eventType)
    {
        SetIdEvent *setIdEvent = reinterpret_cast<SetIdEvent*>(event);
        QModelIndexList items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(setIdEvent->oldId), 1, Qt::MatchRecursive);
        if(!items.isEmpty())
        {
            model->itemFromIndex(items.at(0))->setText(setIdEvent->newId);
            setProjectChanged(true);
        }
    }
    else if(event->type() == SettingsSavedEvent::eventType)
    {
        m_Project->setProjectName(m_projectSettings->getProjectName());
        m_Project->setDataPath(m_projectSettings->getDataPath());
        imagesBrowser->m_DataPath = m_Project->getDataPath();
        imagesBrowser->refresh();
        setProjectChanged(true);

        ui->openGLWidget->sendEvent(event);
    }

    return QMainWindow::event(event);
}

void CoreWindow::resizeEvent(QResizeEvent*)
{
    if(isHidden()) return;

    if(imagesBrowser && imagesBrowser->isVisible())
        imagesBrowser->move(this->rect().center() - imagesBrowser->rect().center());

    if(m_projectSettings && m_projectSettings->isVisible())
        m_projectSettings->move(this->rect().center() - m_projectSettings->rect().center());
}

bool CoreWindow::eventFilter(QObject*, QEvent *event)
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
                    }
                }
            }
        }
    }
    return false;
}

void CoreWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {

    case Qt::Key_Delete:
    {
        auto idx = ui->treeView->currentIndex();
        if(idx.parent().isValid())
        {
            QStandardItem *item = model->itemFromIndex(idx);
            ui->openGLWidget->deleteWidget(item->text());
            model->removeRow(idx.row(), idx.parent());
            QStandardItem *root = model->invisibleRootItem();
            QModelIndex index = root->child(0)->index();
            ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            selectWidgetById(root->child(0)->text());
        }
        else
        {
            model->clear();
            ui->openGLWidget->clearWidgets();
            m_selected = nullptr;
        }
        setProjectChanged(true);
        break;
    }

    }
}

void CoreWindow::closeEvent(QCloseEvent *event)
{
    if(!m_Project->isChanged())
    {
        event->accept();
        return;
    }

    QMessageBox box;
    QMessageBox::StandardButton response = box.question(this, "Save Changes",
                 "Do you want to save this project before closing?",
                 QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                 QMessageBox::Yes);

    if(response == QMessageBox::Yes)
    {
        if(m_Project->save())
            event->accept();
        else
            event->ignore();
    }
    else if(response == QMessageBox::No)
        event->accept();
    else
        event->ignore();
}

void CoreWindow::addChildToTree(QString label)
{
    QModelIndex index = ui->treeView->currentIndex();
    QStandardItem *item = model->itemFromIndex(index);
    QStandardItem *newItem = new QStandardItem(label);
    newItem->setEditable(false);
    item->appendRow(newItem);
    ui->treeView->expand(index);
    ui->treeView->selectionModel()->select(newItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui->treeView->selectionModel()->setCurrentIndex(newItem->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    selectWidgetById(newItem->text());
    setProjectChanged(true);
}

void CoreWindow::selectWidgetById(QString widgetId)
{
    for(auto &widget : ui->openGLWidget->getOTUIWidgets())
    {
        if(widget->getId() == widgetId)
        {
            m_selected = widget.get();
            break;
        }
    }
}

void CoreWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    ui->actionDeleteWidget->setEnabled(ui->treeView->indexAt(pos).isValid());

    menu.addAction(ui->actionDeleteWidget);

    menu.addSeparator();

    QMenu *newMenu = menu.addMenu("New...");

    newMenu->addAction(ui->newMainWindow);
    newMenu->addAction(ui->newButton);
    newMenu->addAction(ui->newLabel);
    newMenu->addAction(ui->newUIItem);
    newMenu->addAction(ui->newUICreature);
    newMenu->addSeparator();

    QMenu *customMenu = newMenu->addMenu("Custom");
    customMenu->setDisabled(true);
    customMenu->addAction(ui->newButton);

    menu.exec(ui->treeView->mapToGlobal(pos));
}

void CoreWindow::on_actionDeleteWidget_triggered()
{
    auto idx = ui->treeView->currentIndex();
    if(idx.parent().isValid())
    {
        QStandardItem *item = model->itemFromIndex(idx);
        ui->openGLWidget->deleteWidget(item->text());
        model->removeRow(idx.row(), idx.parent());
        QStandardItem *root = model->invisibleRootItem();
        QModelIndex index = root->child(0)->index();
        ui->treeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        selectWidgetById(root->child(0)->text());
    }
    else
    {
        model->clear();
        ui->openGLWidget->clearWidgets();
        m_selected = nullptr;
    }
    setProjectChanged(true);
}

void CoreWindow::on_actionNewProject_triggered()
{
    if(m_Project->isChanged())
    {
        QMessageBox box;
        QMessageBox::StandardButton response = box.question(this, "Save Changes",
                     "Do you want to save this project before closing?",
                     QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                     QMessageBox::Yes);

        if(response == QMessageBox::Yes)
        {
            if(!m_Project->save())
                return;
        }
        else if(response == QMessageBox::Cancel)
            return;
    }

    if(m_Project->getProjectFile()->isOpen())
    {
        m_Project->getProjectFile()->close();
    }

    // Clear tree
    model->clear();

    // Clear selected
    m_selected = nullptr;

    // Clear widgets
    ui->openGLWidget->clearWidgets();
}

void CoreWindow::on_actionSaveProject_triggered()
{
    if(m_Project->save())
        setWindowTitle(m_Project->getProjectName() + " - OTUI Editor");
}

void CoreWindow::on_actionCloseProject_triggered()
{
    if(m_Project->isChanged())
    {
        QMessageBox box;
        QMessageBox::StandardButton response = box.question(this, "Save Changes",
                     "Do you want to save this project before closing?",
                     QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                     QMessageBox::Yes);

        if(response == QMessageBox::Yes)
        {
            if(!m_Project->save())
                return;
        }
        else if(response == QMessageBox::Cancel)
            return;
    }

    if(m_Project)
    {
        m_Project->getProjectFile()->close();
    }

    StartupWindow *w = new StartupWindow();
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
        QStandardItem *root = model->invisibleRootItem();
        QStandardItem *item = new QStandardItem(widgetId);
        item->setEditable(false);
        root->appendRow(item);
        model->setHeaderData(0, Qt::Horizontal, "Widgets List");
        ui->treeView->selectionModel()->select(item->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        ui->treeView->selectionModel()->setCurrentIndex(item->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        selectWidgetById(item->text());
    }

    m_selected = ui->openGLWidget->addWidget<OTUI::MainWindow>(widgetId, m_Project->getDataPath(), "/images/ui/window.png", QRect(50, 50, 256, 256), QRect(0, 0, 256, 256), QRect(6, 27, 6, 6));
    setProjectChanged(true);
}

void CoreWindow::on_newButton_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("button");
        m_selected = ui->openGLWidget->addWidgetChild<OTUI::Button>("mainWindow", widgetId, m_Project->getDataPath(), "/images/ui/button_rounded.png", QRect(50, 50, 106, 23), QRect(0, 0, 22, 23), QRect(5, 5, 5, 5));
        addChildToTree(m_selected->getId());
        setProjectChanged(true);
    }
}

void CoreWindow::on_newLabel_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("label");
        m_selected = ui->openGLWidget->addWidgetChild<OTUI::Label>("mainWindow", widgetId, m_Project->getDataPath(), "", QRect(0, 0, 106, 23), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        addChildToTree(m_selected->getId());
        setProjectChanged(true);
    }
}

void CoreWindow::on_newUIItem_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("item");
        m_selected = ui->openGLWidget->addWidgetChild<OTUI::Item>("mainWindow", widgetId, m_Project->getDataPath(), "", QRect(0, 0, 32, 32), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        addChildToTree(m_selected->getId());
        setProjectChanged(true);
    }
}

void CoreWindow::on_newUICreature_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        QString widgetId("creature");
        m_selected = ui->openGLWidget->addWidgetChild<OTUI::Creature>("mainWindow", widgetId, m_Project->getDataPath(), "", QRect(0, 0, 48, 48), QRect(0, 0, 0, 0), QRect(0, 0, 0, 0));
        addChildToTree(m_selected->getId());
        setProjectChanged(true);
    }
}

void CoreWindow::on_actionProject_Settings_triggered()
{
    m_projectSettings->move(this->rect().center() - m_projectSettings->rect().center());
    m_projectSettings->show();
}

void CoreWindow::setProjectChanged(bool v) {
    if(v)
        setWindowTitle(m_Project->getProjectName() + " * - OTUI Editor");
    else
        setWindowTitle(m_Project->getProjectName() + " - OTUI Editor");
    m_Project->setChanged(v);
}
