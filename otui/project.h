#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QFile>
#include <QDataStream>

namespace OTUI {
    class Project
    {
    public:
        Project();
        Project(QString fileName, QString data, QString path, QString dataPath);
        Project(QDataStream &data, QString fileName, QString path);
        ~Project();

    public:
        bool save();

        bool loaded() const {
            return m_File && m_File->isOpen();
        }

        bool isChanged() const {
            return m_Changed;
        }

        void setChanged(bool v) {
            m_Changed = v;
        }

        QString getProjectName() const {
            return m_Name;
        }

        void setProjectName(QString name) {
            m_Name = name;
        }

        QString getProjectPath() const {
            return m_Path;
        }

        void setProjectPath(QString path) {
            m_Path = path;
        }

        QString getDataPath() const {
            return m_Data;
        }

        void setDataPath(QString v) {
            m_Data = v;
        }

        QFile *getProjectFile() {
            return m_File;
        }

    private:
        bool m_Changed = false;
        QString m_Name;
        QString m_Path;
        QString m_Data;
        QFile *m_File = nullptr;

    };
}

#endif // PROJECT_H
