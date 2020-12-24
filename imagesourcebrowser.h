#ifndef IMAGESOURCEBROWSER_H
#define IMAGESOURCEBROWSER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QTreeWidget>
#include <QDirIterator>
#include <QPixmap>
#include <QtDebug>

class ImageSourceBrowser : public QFrame
{
    Q_OBJECT
public:
    explicit ImageSourceBrowser(QWidget *parent = nullptr);
    ~ImageSourceBrowser();

private slots:
    void handleCloseButton();
    void onItemClicked(QTreeWidgetItem *item, int column);

private:
    void recursivelyGetDirectory(QString path, QTreeWidgetItem *parent);
    void addImageToGrid(QString title, QString path);

    QFrame *topBar;
    QLabel *titleLabel;
    QPushButton *closeButton;
    QWidget *contentPanel;
    QScrollArea *rightPanel;
    QTreeWidget *directoryList;
    QGridLayout *imagesGridLayout;
    QWidget *imagesGrid;
};

#endif // IMAGESOURCEBROWSER_H
