#ifndef ELASPLITBUTTON_H
#define ELASPLITBUTTON_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaMenu;
class ElaSplitButtonPrivate;
class ELA_EXPORT ElaSplitButton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSplitButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, ElaIcon)

public:
    explicit ElaSplitButton(QWidget* parent = nullptr);
    ~ElaSplitButton();

    void setMenu(ElaMenu* menu);
    ElaMenu* getMenu() const;

Q_SIGNALS:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELASPLITBUTTON_H
