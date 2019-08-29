#include "otuiwidget.h"

OTUI::CWidget::CWidget()
{
}

OTUI::CWidget::CWidget(QString widgetId, QString imagePath)
{
    m_id.clear();
    m_id.append(widgetId);
    if(imagePath != nullptr)
        m_image.load(imagePath);
    m_imageSize = QPoint(m_image.width(), m_image.height());
    m_rect = QRect(0, 0, m_image.width(), m_image.height());
    m_children.clear();
    m_imageCrop.setRect(0, 0, -1, -1);
}

OTUI::CWidget::~CWidget()
{
}