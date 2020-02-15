#include "recentproject.h"

RecentProject::RecentProject(QString name, QString path, QString date)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(8);

    this->name = name;
    this->path = path;
    this->date = date;

    QFont font("Roboto", 9);

    QLabel* projectName = new QLabel(this);
    projectName->setFont(font);
    projectName->setText(name);
    projectName->setStyleSheet("color: #fff;font-weight: bold;");

    QHBoxLayout* lay = new QHBoxLayout;
    lay->setMargin(0);
    lay->setSpacing(0);

    QLabel* projectDate = new QLabel(this);
    projectDate->setFont(font);
    projectDate->setText(date);
    projectDate->setStyleSheet("color: #aaa;font-weight: normal;");
    lay->addWidget(projectDate, 0, Qt::AlignRight);

    projectName->setLayout(lay);
    layout->addWidget(projectName, 0, Qt::AlignTop);

    QLabel* projectPath = new QLabel(this);
    projectPath->setFont(font);
    QFontMetrics metrics(projectPath->font());
    QString elidedText = metrics.elidedText(path, Qt::ElideRight, 360);
    projectPath->setText(elidedText);
    projectPath->setStyleSheet("color: #aaa;");
    layout->addWidget(projectPath, 0, Qt::AlignTop);

    setLayout(layout);
    setCursor(QCursor(Qt::PointingHandCursor));
}

RecentProject::RecentProject(QFrame *parent) : QFrame(parent)
{
}

void RecentProject::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        emit clicked(this);
}
