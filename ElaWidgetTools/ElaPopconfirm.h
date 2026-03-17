#ifndef ELAPOPCONFIRM_H
#define ELAPOPCONFIRM_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaPopconfirmPrivate;
class ELA_EXPORT ElaPopconfirm : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPopconfirm)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(QString, ConfirmButtonText)
    Q_PROPERTY_CREATE_Q_H(QString, CancelButtonText)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, Icon)
    Q_PROPERTY_CREATE_Q_H(bool, IsLightDismiss)
public:
    explicit ElaPopconfirm(QWidget* parent = nullptr);
    ~ElaPopconfirm() override;

    void showPopconfirm(QWidget* target);
    void closePopconfirm();

Q_SIGNALS:
    Q_SIGNAL void confirmed();
    Q_SIGNAL void cancelled();
    Q_SIGNAL void closed();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPOPCONFIRM_H
