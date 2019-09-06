#ifndef RECENTPROJECT_H
#define RECENTPROJECT_H

#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>

class RecentProject : public QFrame
{
    Q_OBJECT
public:
    RecentProject(QString name, QString path, QString date);
    explicit RecentProject(QFrame *parent = nullptr);

signals:
    void clicked(RecentProject* widget);

protected:
    void mouseReleaseEvent(QMouseEvent*);

public:
    QString name;
    QString path;
    QString date;
};

#endif // RECENTPROJECT_H
