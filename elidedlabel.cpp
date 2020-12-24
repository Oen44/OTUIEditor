#include "elidedlabel.h"

#include <QPainter>
#include <QSizePolicy>
#include <QTextLayout>

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent)
    : QLineEdit(parent)
    , elided(false)
    , content(text)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    setEnabled(false);
    setToolTip(text);
}

void ElidedLabel::setText(const QString &newText)
{
    content = newText;
    setToolTip(newText);
    update();
}

void ElidedLabel::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainter painter(this);
    QFontMetrics fontMetrics = painter.fontMetrics();

    bool didElide = false;
    int lineSpacing = fontMetrics.lineSpacing();
    int y = 4;

    QTextLayout textLayout(content, painter.font());
    textLayout.beginLayout();
    forever {
        QTextLine line = textLayout.createLine();

        if (!line.isValid())
            break;

        line.setLineWidth(width() - 4);
        int nextLineY = y + lineSpacing;

        if (height() >= nextLineY + lineSpacing) {
            line.draw(&painter, QPoint(4, y));
            y = nextLineY;
        } else {
            QString lastLine = content.mid(line.textStart());
            QString elidedLastLine = fontMetrics.elidedText(lastLine, Qt::ElideRight, width() - 4);
            painter.drawText(QPoint(4, y + fontMetrics.ascent()), elidedLastLine);
            line = textLayout.createLine();
            didElide = line.isValid();
            break;
        }
    }
    textLayout.endLayout();

    if (didElide != elided) {
        elided = didElide;
        emit elisionChanged(didElide);
    }
}
