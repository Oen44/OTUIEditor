#ifndef IMAGESOURCEBROWSER_H
#define IMAGESOURCEBROWSER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QTreeWidget>
#include <QDirIterator>
#include <QPixmap>
#include <QTableWidget>

class ImageSourceBrowser : public QFrame
{
    Q_OBJECT
public:
    explicit ImageSourceBrowser(QWidget *parent = nullptr);
    ~ImageSourceBrowser();

private slots:
    void handleCloseButton();
    void onItemClicked(QTreeWidgetItem *item, int column);

public:
    void initialize();
    void refresh();

    QString m_DataPath;

private:
    void recursivelyGetDirectory(QString path, QTreeWidgetItem *parent);
    void addImageToGrid(QString title, QString path);

    QFrame *topBar;
    QLabel *titleLabel;
    QPushButton *closeButton;
    QWidget *contentPanel;
    QTreeWidget *directoryList;
    QTableWidget *imagesGrid;

    int m_lastCol = 0;
    int m_lastRow = 0;
};

#endif // IMAGESOURCEBROWSER_H
