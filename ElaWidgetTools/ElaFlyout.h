#ifndef ELAFLYOUT_H
#define ELAFLYOUT_H

#include <QWidget>

#include "ElaProperty.h"
class ElaFlyoutPrivate;
class ELA_EXPORT ElaFlyout : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaFlyout)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(bool, IsLightDismiss)
public:
    explicit ElaFlyout(QWidget* parent = nullptr);
    ~ElaFlyout() override;

    void setContentWidget(QWidget* widget);
    void showFlyout(QWidget* target);
    void closeFlyout();

Q_SIGNALS:
    void closed();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAFLYOUT_H
