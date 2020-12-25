#include "imagesourcebrowser.h"

#include <QPixmapCache>
#include <QHeaderView>

ImageSourceBrowser::ImageSourceBrowser(QWidget *parent) : QFrame(parent)
{
}

ImageSourceBrowser::~ImageSourceBrowser()
{
}

void ImageSourceBrowser::initialize()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    this->setObjectName("imageSourceBrowser");
    this->setFixedSize(800, 500);
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
    titleLabel->setText("Image Source Browser");

    closeButton = new QPushButton("X", topBar);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    closeButton->setMaximumSize(16, 16);
    closeButton->setFlat(true);
    closeButton->setCursor(Qt::PointingHandCursor);

    connect(closeButton, &QPushButton::released, this, &ImageSourceBrowser::handleCloseButton);

    topBarLayout->addWidget(titleLabel);
    topBarLayout->addWidget(closeButton);
    topBarLayout->setAlignment(closeButton, Qt::AlignVCenter);
    topBarLayout->setAlignment(titleLabel, Qt::AlignVCenter);
    layout->addWidget(topBar);

    contentPanel = new QWidget(this);

    QGridLayout *contentLayout = new QGridLayout(contentPanel);
    contentLayout->setSpacing(0);
    contentLayout->setMargin(0);

    // Left

    directoryList = new QTreeWidget(contentPanel);
    directoryList->setObjectName("leftBrowserPanel");
    directoryList->setMaximumWidth(180);
    directoryList->setHeaderHidden(true);
    directoryList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    connect(directoryList, &QTreeWidget::itemClicked, this, &ImageSourceBrowser::onItemClicked);
    connect(directoryList, &QTreeWidget::itemActivated, this, &ImageSourceBrowser::onItemClicked);

    QGridLayout *directoryListLayout = new QGridLayout(directoryList);
    directoryListLayout->setContentsMargins(0, 0, 2, 0);
    directoryListLayout->setSpacing(0);

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(directoryList);
    directoryList->addTopLevelItem(topLevelItem);
    topLevelItem->setText(0, "data");

    QDirIterator it(m_DataPath, QDir::NoDotAndDotDot | QDir::Dirs);
    while (it.hasNext()) {
        QString n = it.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(topLevelItem);
        item->setText(0, n.right(n.length() - n.lastIndexOf("/") - 1));
        recursivelyGetDirectory(n, item);
    }

    topLevelItem->setExpanded(true);

    // Right
    imagesGrid = new QTableWidget(contentPanel);
    imagesGrid->setShowGrid(false);
    imagesGrid->setObjectName("imagesBrowserGrid");

    imagesGrid->horizontalHeader()->setDefaultSectionSize(148);
    imagesGrid->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    imagesGrid->horizontalHeader()->hide();

    imagesGrid->verticalHeader()->setDefaultSectionSize(158);
    imagesGrid->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    imagesGrid->verticalHeader()->hide();

    contentLayout->addWidget(directoryList, 0, 0);
    contentLayout->addWidget(imagesGrid, 0, 1);

    layout->addWidget(contentPanel);
}

void ImageSourceBrowser::refresh()
{
    delete directoryList->takeTopLevelItem(0);

    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(directoryList);
    directoryList->addTopLevelItem(topLevelItem);
    topLevelItem->setText(0, "data");

    QDirIterator it(m_DataPath, QDir::NoDotAndDotDot | QDir::Dirs);
    while (it.hasNext()) {
        QString n = it.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(topLevelItem);
        item->setText(0, n.right(n.length() - n.lastIndexOf("/") - 1));
        recursivelyGetDirectory(n, item);
    }

    topLevelItem->setExpanded(true);
}

void ImageSourceBrowser::handleCloseButton()
{
    this->hide();
}

void ImageSourceBrowser::onItemClicked(QTreeWidgetItem *item, int)
{
    //qDeleteAll(imagesGrid->findChildren<QTableWidgetItem *>(QString(), Qt::FindDirectChildrenOnly));
    imagesGrid->clearContents();
    m_lastCol = 0;
    m_lastRow = 0;

    QString path(m_DataPath + "/");
    if (item->text(0) != "data")
    {
        QTreeWidgetItem *parent = item->parent();
        QStringList parentList;
        while(parent && parent->text(0) != "data")
        {
            parentList.push_front(parent->text(0));
            parent = parent->parent();
        }
        path += parentList.join("/") + "/" + item->text(0);
    }

    QDirIterator it(path, QStringList() << "*.png", QDir::NoDotAndDotDot | QDir::Files);

    imagesGrid->setColumnCount(4);
    imagesGrid->setRowCount(255);
    int items = 0;
    while (it.hasNext()) {
        QString n = it.next();
        addImageToGrid(n.right(n.length() - n.lastIndexOf("/") - 1), n);
        items++;
    }
    imagesGrid->setColumnCount(qMin(4, items));
    imagesGrid->setRowCount(qMax(1, items / 4));
}

void ImageSourceBrowser::addImageToGrid(QString title, QString path)
{
    QWidget *item = new QWidget;
    item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QVBoxLayout *itemLayout = new QVBoxLayout(item);
    itemLayout->setMargin(5);
    itemLayout->setSpacing(4);

    QLabel *imageWidget = new QLabel;
    imageWidget->setMinimumSize(128, 128);
    imageWidget->setAlignment(Qt::AlignCenter);
    QPixmap pic;
    if (!QPixmapCache::find(path, &pic)) {
        pic.load(path);
        QPixmapCache::insert(path, pic);
    }
    if (pic.width() >= pic.height() && pic.width() > 128)
        imageWidget->setPixmap(pic.scaledToWidth(128));
    else if(pic.height() >= pic.width() && pic.height() > 128)
        imageWidget->setPixmap(pic.scaledToHeight(128));
    else
        imageWidget->setPixmap(pic);

    QLabel *imageTitle = new QLabel(title);
    imageTitle->setAlignment(Qt::AlignCenter);

    itemLayout->addWidget(imageWidget);
    itemLayout->addWidget(imageTitle);

    if(m_lastCol == 4)
    {
        m_lastRow++;
        m_lastCol = 0;
    }

    imagesGrid->setCellWidget(m_lastRow, m_lastCol++, item);
}

void ImageSourceBrowser::recursivelyGetDirectory(QString path, QTreeWidgetItem *parent)
{
    QDirIterator it(path, QDir::NoDotAndDotDot | QDir::Dirs);
    while (it.hasNext()) {
        QString n = it.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(parent);
        item->setText(0, n.right(n.length() - n.lastIndexOf("/") - 1));
        recursivelyGetDirectory(n, item);
    }
}
