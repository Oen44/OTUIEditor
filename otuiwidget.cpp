#include "otuiwidget.h"

using namespace OTUI;

CWidget::CWidget()
{
}

CWidget::CWidget(QString widgetId, QString imagePath)
{
    id.clear();
    id.append(widgetId);
    m_image.load(imagePath);
    m_imageSize = QPoint(m_image.width(), m_image.height());
    m_rect = QRect(0, 0, m_image.width(), m_image.height());
    m_children.clear();
    m_imageCrop.setRect(0, 0, -1, -1);
}

CWidget::~CWidget()
{
}
