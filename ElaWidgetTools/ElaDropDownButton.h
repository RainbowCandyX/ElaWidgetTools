#ifndef ELADROPDOWNBUTTON_H
#define ELADROPDOWNBUTTON_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMenu;
class ElaDropDownButtonPrivate;
class ELA_EXPORT ElaDropDownButton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDropDownButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, ElaIcon)
public:
    explicit ElaDropDownButton(QWidget* parent = nullptr);
    ~ElaDropDownButton() override;

    void setMenu(ElaMenu* menu);
    ElaMenu* getMenu() const;

protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
};

#endif // ELADROPDOWNBUTTON_H
