#ifndef ELADASHBOARDGAUGE_H
#define ELADASHBOARDGAUGE_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaDashboardGaugePrivate;
class ELA_EXPORT ElaDashboardGauge : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDashboardGauge)
    Q_PROPERTY_CREATE_Q_H(qreal, Minimum)
    Q_PROPERTY_CREATE_Q_H(qreal, Maximum)
    Q_PROPERTY_CREATE_Q_H(qreal, Value)
    Q_PROPERTY_CREATE_Q_H(int, MajorTickCount)
    Q_PROPERTY_CREATE_Q_H(int, MinorTickCount)
    Q_PROPERTY_CREATE_Q_H(int, StartAngle)
    Q_PROPERTY_CREATE_Q_H(int, SpanAngle)
    Q_PROPERTY_CREATE_Q_H(int, ArcWidth)
    Q_PROPERTY_CREATE_Q_H(int, ValuePixelSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsAnimated)
    Q_PROPERTY_CREATE_Q_H(int, Decimals)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Title)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Unit)
public:
    explicit ElaDashboardGauge(QWidget* parent = nullptr);
    ~ElaDashboardGauge() override;

    void setDangerPercent(qreal percent);
    qreal getDangerPercent() const;

    void setWarningPercent(qreal percent);
    qreal getWarningPercent() const;

    void setTickWarningPercent(qreal percent);
    qreal getTickWarningPercent() const;

Q_SIGNALS:
    Q_SIGNAL void valueChanged(qreal value);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELADASHBOARDGAUGE_H
