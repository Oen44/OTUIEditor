#include "project.h"

#include <QDebug>

OTUI::Project::Project() {}

OTUI::Project::Project(QString fileName, QString name, QString path, QString dataPath)
{
    m_Name = name;
    m_Path = path;
    m_Data = dataPath;

    m_File = new QFile(path + "/" + fileName);

    if(!m_File->open(QIODevice::ReadWrite))
    {
        m_File = nullptr;
        return;
    }

    QDataStream data(m_File);
    data << m_Name;
    data << dataPath;
    m_File->flush();
}

OTUI::Project::Project(QDataStream &data, QString fileName, QString path)
{
    data >> m_Name;
    data >> m_Data;
    m_Path = path;

    m_File = new QFile(path + "/" + fileName);

    if(!m_File->open(QIODevice::ReadWrite))
        m_File = nullptr;
}

OTUI::Project::~Project()
{
    if(m_File && m_File->isOpen())
        m_File->close();
}

bool OTUI::Project::save()
{
    if(!m_File)
        return false;

    // Create backup
    if(!QFile::copy(m_File->fileName(), m_File->fileName() + ".backup"))
        return false;

    // Clear file content
    m_File->resize(0);

    // Write new data
    QDataStream data(m_File);
    data << m_Name;
    data << m_Data;

    // Save file
    if(m_File->flush())
    {
        // Remove backup
        QFile::remove(m_File->fileName() + ".backup");
        setChanged(false);
        return true;
    }

    return false;
}
