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

    w = new CoreWindow();

    QVBoxLayout* layout = reinterpret_cast<QVBoxLayout*>(ui->recentList->layout());
    ui->recentList->layout()->setSpacing(0);
    ui->recentList->layout()->setMargin(0);

    QFile recentFile("recent.pro");

    if(recentFile.exists())
    {
        if(!recentFile.open(QIODevice::ReadOnly))
            return;

        QDataStream recentData(&recentFile);

        int8_t size;
        QString name;
        QString path;
        QString date;
        recentData >> size;
        for (int i = 0; i < size; i++) {
            recentData >> name;
            recentData >> path;
            recentData >> date;

            std::unique_ptr<RecentProject> rp = std::make_unique<RecentProject>(name, path, date);
            layout->addWidget(rp.get(), 0, Qt::AlignTop);
            connect(rp.get(), &RecentProject::clicked, this, &StartupWindow::recentProjectClicked);
            m_recentProjects.push_back(std::move(rp));
        }
    }
    recentFile.close();
}

StartupWindow::~StartupWindow()
{
    delete ui;
}

void StartupWindow::recentProjectClicked(RecentProject* widget) {
    QString fileName(widget->path + "/" + widget->name.toLower().replace(' ', '_') + ".pro");
    QFileInfo fileInfo(fileName);
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
        return;

    QDataStream projectData(&file);

    w->loadProjectData(projectData, fileInfo.absolutePath());
    w->show();
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

        w->loadProjectData(projectData, fileInfo.absolutePath());
        w->show();
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

void StartupWindow::on_createProject_clicked()
{
    QString name = ui->nameInput->text().trimmed();
    QString path = ui->pathInput->text().trimmed();

    if(name.isEmpty() || path.isEmpty())
    {
        ui->nameError->setHidden(!name.isEmpty());
        ui->pathError->setHidden(!path.isEmpty());
        return;
    }

    QString date = QDate::currentDate().toString(Qt::SystemLocaleShortDate) + " " + QTime::currentTime().toString(Qt::SystemLocaleShortDate);

    QFile recentFile("recent.pro");
    if(!recentFile.open(QIODevice::WriteOnly))
        return;

    recentFile.resize(0);

    QDataStream data(&recentFile);

    if(m_recentProjects.size() == 5)
    {
        m_recentProjects.pop_back();
    }

    std::unique_ptr<RecentProject> newProject = std::make_unique<RecentProject>(name, path, date);

    m_recentProjects.push_back(std::move(newProject));
    std::rotate(m_recentProjects.rbegin(), m_recentProjects.rbegin() + 1, m_recentProjects.rend());

    data << static_cast<uint8_t>(m_recentProjects.size());
    for(auto& recent : m_recentProjects) {
        data << recent->name;
        data << recent->path;
        data << recent->date;
    }

    recentFile.close();

    w->startNewProject(name, path);
    w->show();
    hide();
}

void StartupWindow::on_nameInput_textChanged(const QString& text)
{
    ui->nameError->setHidden(!text.trimmed().isEmpty());
}

void StartupWindow::on_pathInput_textChanged(const QString& text)
{
    ui->pathError->setHidden(!text.trimmed().isEmpty());
}
