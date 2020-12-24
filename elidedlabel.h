#ifndef ELIDEDLABEL_H
#define ELIDEDLABEL_H

#include <QLineEdit>
#include <QString>

class ElidedLabel : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool isElided READ isElided)

public:
    explicit ElidedLabel(const QString &text, QWidget *parent = 0);

    void setText(const QString &text);
    const QString  &text() const { return content; }
    bool isElided() const { return elided; }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void elisionChanged(bool elided);

private:
    bool elided;
    QString content;
};

#endif // ELIDEDLABEL_H
