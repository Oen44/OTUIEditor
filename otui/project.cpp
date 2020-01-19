#include "project.h"

OTUI::Project::Project() {}

OTUI::Project::Project(QString name, QString path)
{
    m_Name = name;
    m_Path = path;
    m_File = new QFile();
    QString fileName(name.toLower().replace(' ', '_') + ".pro");

    m_File->setFileName(path + "/" + fileName);
    if(!m_File->open(QIODevice::ReadWrite))
    {
        m_File = nullptr;
        return;
    }

    QDataStream data(m_File);
    data << m_Name;
    m_File->flush();
}

OTUI::Project::Project(QDataStream& data, QString path)
{
    data >> m_Name;
    m_Path = path;
    m_File = new QFile();

    QString fileName(m_Name.toLower().replace(' ', '_') + ".pro");
    m_File->setFileName(path + "/" + fileName);
    if(!m_File->open(QIODevice::ReadWrite))
        m_File = nullptr;
}

OTUI::Project::~Project()
{
    m_File->close();
}
