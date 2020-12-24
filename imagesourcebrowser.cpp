#include "imagesourcebrowser.h"

ImageSourceBrowser::ImageSourceBrowser(QWidget *parent) : QFrame(parent)
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

    QDirIterator it("D:/Projects/Tibia/Revolution/otclientv8/data", QDir::NoDotAndDotDot | QDir::Dirs);
    while (it.hasNext()) {
        QString n = it.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(topLevelItem);
        item->setText(0, n.right(n.length() - n.lastIndexOf("/") - 1));
        recursivelyGetDirectory(n, item);
    }

    topLevelItem->setExpanded(true);

    // Right

    rightPanel = new QScrollArea(contentPanel);
    rightPanel->setObjectName("rightBrowserPanel");
    rightPanel->setAlignment(Qt::AlignHCenter);

    imagesGrid = new QWidget();
    imagesGrid->setObjectName("imagesBrowserGrid");
    rightPanel->setWidget(imagesGrid);

    imagesGridLayout = new QGridLayout(imagesGrid);
    imagesGridLayout->setMargin(5);
    imagesGridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    //

    contentLayout->addWidget(directoryList, 0, 0);
    contentLayout->addWidget(rightPanel, 0, 1);

    layout->addWidget(contentPanel);
}

ImageSourceBrowser::~ImageSourceBrowser()
{
}

void ImageSourceBrowser::handleCloseButton()
{
    this->hide();
}

void ImageSourceBrowser::onItemClicked(QTreeWidgetItem *item, int)
{
    qDeleteAll(imagesGrid->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly));

    QString path("D:/Projects/Tibia/Revolution/otclientv8/data/");
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
    while (it.hasNext()) {
        QString n = it.next();
        addImageToGrid(n.right(n.length() - n.lastIndexOf("/") - 1), n);
    }
}

void ImageSourceBrowser::addImageToGrid(QString title, QString path)
{
    QWidget *item = new QWidget;
    item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QVBoxLayout *itemLayout = new QVBoxLayout(item);
    itemLayout->setMargin(0);
    itemLayout->setSpacing(4);

    QLabel *imageWidget = new QLabel;
    imageWidget->setAlignment(Qt::AlignCenter);
    QPixmap pic(path);
    imageWidget->setPixmap(pic);

    QLabel *imageTitle = new QLabel(title);
    imageTitle->setAlignment(Qt::AlignCenter);

    itemLayout->addWidget(imageWidget);
    itemLayout->addWidget(imageTitle);

    imagesGridLayout->addWidget(item, imagesGridLayout->count() > 0 ? imagesGridLayout->count() / 2 : 0, imagesGridLayout->count() % 2 == 0 ? 0 : 1);
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
