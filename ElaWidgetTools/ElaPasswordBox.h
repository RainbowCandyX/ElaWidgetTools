#ifndef ELAPASSWORDBOX_H
#define ELAPASSWORDBOX_H

#include <QLineEdit>

#include "ElaProperty.h"

class ElaPasswordBoxPrivate;
class ELA_EXPORT ElaPasswordBox : public QLineEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaPasswordBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsPasswordVisible)
public:
    explicit ElaPasswordBox(QWidget* parent = nullptr);
    ~ElaPasswordBox() override;

Q_SIGNALS:
    Q_SIGNAL void focusIn(QString text);
    Q_SIGNAL void focusOut(QString text);
    Q_SIGNAL void wmFocusOut(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELAPASSWORDBOX_H
