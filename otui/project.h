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
        Project(QString data, QString path);
        Project(QDataStream& data, QString path);
        ~Project();

        bool loaded() const {
            return m_File && m_File->isOpen();
        }

        bool isChanged() const {
            return m_Changed;
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

        QFile* getProjectFile() {
            return m_File;
        }

    private:
        bool m_Changed = false;
        QString m_Name;
        QString m_Path;
        QFile* m_File = nullptr;

    };
}

#endif // PROJECT_H
