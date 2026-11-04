#ifndef ELAITEMVIEWTOOLTIP_H
#define ELAITEMVIEWTOOLTIP_H

#include <QWidget>

#include "ElaProperty.h"

class ElaItemViewToolTipPrivate;
class QAbstractItemView;

class ELA_EXPORT ElaItemViewToolTip : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaItemViewToolTip)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)
    Q_PROPERTY_CREATE_Q_H(int, ShowDelayMsec)
    Q_PROPERTY_CREATE_Q_H(int, HideDelayMsec)
    Q_PROPERTY_CREATE_Q_H(QString, ToolTip)
public:
    explicit ElaItemViewToolTip(QAbstractItemView* view);
    ~ElaItemViewToolTip() override;

    void updatePos();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif
