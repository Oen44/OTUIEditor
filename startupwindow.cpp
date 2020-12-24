#include "startupwindow.h"
#include "ui_startupwindow.h"

StartupWindow::StartupWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartupWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ui->nameError->setHidden(true);
    ui->pathError->setHidden(true);
    ui->dataError->setHidden(true);

    coreWindow = new CoreWindow();

    QWidget *recentList = reinterpret_cast<QWidget*>(ui->recentListContent);

    QFile recentFile("recent.pro");

    if(recentFile.exists())
    {
        if(!recentFile.open(QIODevice::ReadOnly))
            return;

        QDataStream recentData(&recentFile);

        int8_t size;
        recentData >> size;
        for (int i = 0; i < size; i++) {
            QString fileName, path, date;
            recentData >> fileName;
            recentData >> path;
            recentData >> date;

            if(QFile::exists(path + "/" + fileName)) {
                QString name;

                QFile projectFile(path + "/" + fileName);
                if(!projectFile.open(QIODevice::ReadOnly))
                    continue;

                QDataStream projectData(&projectFile);
                projectData >> name;
                projectFile.close();

                std::unique_ptr<RecentProject> rp = std::make_unique<RecentProject>(fileName, name, path, date);
                rp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
                recentList->layout()->addWidget(rp.get());
                connect(rp.get(), &RecentProject::clicked, this, &StartupWindow::recentProjectClicked);
                m_recentProjects.push_back(std::move(rp));
            }
        }
    }
    recentFile.close();
}

StartupWindow::~StartupWindow()
{
    delete coreWindow;
    delete ui;
}

void StartupWindow::recentProjectClicked(RecentProject *widget) {
    QString fileName(widget->path + "/" + widget->fileName);
    QFileInfo fileInfo(fileName);
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
        return;

    QDataStream projectData(&file);

    coreWindow->loadProjectData(projectData, widget->fileName, fileInfo.absolutePath());
    coreWindow->show();
    hide();

    file.close();
}

void StartupWindow::on_newProject_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartupWindow::on_openProject_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Project", QDir::homePath(),
                                                    "Project File (*.pro)");
    if(!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        QFile file(fileName);

        if(!file.open(QIODevice::ReadOnly))
            return;

        QDataStream projectData(&file);

        coreWindow->loadProjectData(projectData, fileInfo.fileName(), fileInfo.absolutePath());
        coreWindow->show();
        hide();

        file.close();
    }
}

void StartupWindow::on_browsePath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select project path",
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->pathInput->setText(path);
}

void StartupWindow::on_browseData_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select client data path",
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->dataInput->setText(path);
}

void StartupWindow::on_createProject_clicked()
{
    QString name = ui->nameInput->text().trimmed();
    QString path = ui->pathInput->text().trimmed();
    QString clientData = ui->dataInput->text().trimmed();

    if(name.isEmpty() || path.isEmpty())
    {
        ui->nameError->setHidden(!name.isEmpty());
        ui->pathError->setHidden(!path.isEmpty());
        ui->dataError->setHidden(!clientData.isEmpty());
        return;
    }

    QString date = QDate::currentDate().toString(Qt::SystemLocaleShortDate) + " " + QTime::currentTime().toString(Qt::SystemLocaleShortDate);

    QFile recentFile("recent.pro");
    if(!recentFile.open(QIODevice::WriteOnly))
        return;

    recentFile.resize(0);

    QDataStream data(&recentFile);

    QString fileName(name.toLower().replace(' ', '_') + ".pro");
    std::unique_ptr<RecentProject> newProject = std::make_unique<RecentProject>(fileName, name, path, date);

    m_recentProjects.push_back(std::move(newProject));
    std::rotate(m_recentProjects.rbegin(), m_recentProjects.rbegin() + 1, m_recentProjects.rend());

    data << static_cast<uint8_t>(m_recentProjects.size());
    for(auto &recent : m_recentProjects) {
        data << recent->fileName;
        data << recent->path;
        data << recent->date;
    }

    recentFile.close();

    coreWindow->startNewProject(fileName, name, path, clientData);
    coreWindow->show();
    hide();
}

void StartupWindow::on_nameInput_textEdited(const QString &text)
{
    ui->nameError->setHidden(!text.trimmed().isEmpty());
}

void StartupWindow::on_pathInput_textEdited(const QString &text)
{
    ui->pathError->setHidden(!text.trimmed().isEmpty());
}

void StartupWindow::on_dataInput_textEdited(const QString &text)
{
    ui->dataError->setHidden(!text.trimmed().isEmpty());
}

void StartupWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    ui->nameError->setHidden(true);
    ui->pathError->setHidden(true);
    ui->dataError->setHidden(true);

    ui->nameInput->clear();
    ui->pathInput->clear();
    ui->dataInput->clear();
}
