#include "projectsettings.h"
#include "corewindow.h"

#include <QFileDialog>
#include <QCoreApplication>
#include <QScrollArea>

ProjectSettings::ProjectSettings(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    this->setObjectName("projectSettings");
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(400, 100);
    this->setLineWidth(0);
    this->setFrameStyle(0);
    this->setFrameShadow(QFrame::Plain);

    topBar = new QFrame(this);
    topBar->setObjectName("topBar");
    topBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    topBar->setMinimumHeight(20);
    topBar->setMaximumHeight(20);
    topBar->setLineWidth(0);

    QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setSpacing(0);
    topBarLayout->setContentsMargins(3, 0, 3, 0);

    titleLabel = new QLabel(topBar);
    titleLabel->setText("Project Settings");

    closeButton = new QPushButton("X", topBar);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    closeButton->setMaximumSize(16, 16);
    closeButton->setFlat(true);
    closeButton->setCursor(Qt::PointingHandCursor);

    connect(closeButton, &QPushButton::released, this, &ProjectSettings::handleCloseButton);

    topBarLayout->addWidget(titleLabel);
    topBarLayout->addWidget(closeButton);
    topBarLayout->setAlignment(closeButton, Qt::AlignVCenter);
    topBarLayout->setAlignment(titleLabel, Qt::AlignVCenter);
    layout->addWidget(topBar);

    contentPanel = new QWidget(this);

    QVBoxLayout *contentLayout = new QVBoxLayout(contentPanel);
    contentLayout->setSpacing(5);
    contentLayout->setMargin(5);

    // Content
    addProjectName(contentLayout);
    addDataPath(contentLayout);
    addSaveButton(contentLayout);

    layout->addWidget(contentPanel);
}

ProjectSettings::~ProjectSettings()
{
}

void ProjectSettings::addProjectName(QVBoxLayout *contentLayout)
{
    QWidget *setting = new QWidget(contentPanel);
    QHBoxLayout *settingLayout = new QHBoxLayout(setting);
    settingLayout->setSpacing(5);
    settingLayout->setMargin(0);

    QLabel *label = new QLabel("Project Name", setting);
    label->setFixedWidth(100);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    projectNameInput = new QLineEdit(setting);
    projectNameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    settingLayout->addWidget(label);
    settingLayout->addWidget(projectNameInput);

    contentLayout->addWidget(setting);
}

void ProjectSettings::addDataPath(QVBoxLayout *contentLayout)
{
    QWidget *setting = new QWidget(contentPanel);
    QHBoxLayout *settingLayout = new QHBoxLayout(setting);
    settingLayout->setSpacing(5);
    settingLayout->setMargin(0);

    QLabel *label = new QLabel("Client Data Path", setting);
    label->setFixedWidth(100);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    dataPathInput = new QLineEdit(setting);
    dataPathInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dataPathInput->setEnabled(false);

    QPushButton *dataPathButton = new QPushButton("Browse", setting);
    dataPathButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dataPathButton->setMaximumSize(50, 20);
    dataPathButton->setFlat(true);
    dataPathButton->setCursor(Qt::PointingHandCursor);

    connect(dataPathButton, &QPushButton::released, this, &ProjectSettings::handleDataBrowse);

    settingLayout->addWidget(label);
    settingLayout->addWidget(dataPathInput);
    settingLayout->addWidget(dataPathButton);

    contentLayout->addWidget(setting);
}

void ProjectSettings::addSaveButton(QVBoxLayout *contentLayout)
{
    QPushButton *button = new QPushButton("Save", this);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button->setFixedSize(70, 25);
    button->setFlat(true);
    button->setCursor(Qt::PointingHandCursor);

    connect(button, &QPushButton::released, this, &ProjectSettings::handleSaveButton);

    contentLayout->addWidget(button, 0, Qt::AlignHCenter);
}

void ProjectSettings::handleDataBrowse()
{
    QDir::setCurrent(QDir::homePath());
    QString path = QFileDialog::getExistingDirectory(this, "Select client data path",
                                                    getDataPath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!path.isEmpty())
        dataPathInput->setText(path);
}

void ProjectSettings::handleSaveButton()
{
    SettingsSavedEvent *event = new SettingsSavedEvent();
    event->dataPath = getDataPath();
    qApp->postEvent(qApp->activeWindow(), event);
    this->hide();
}

void ProjectSettings::handleCloseButton()
{
    this->hide();
}
