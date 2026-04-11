#ifndef ELADASHBOARDGAUGEPRIVATE_H
#define ELADASHBOARDGAUGEPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaDashboardGauge;
class ElaDashboardGaugePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDashboardGauge)
    Q_PROPERTY_CREATE_D(qreal, Minimum)
    Q_PROPERTY_CREATE_D(qreal, Maximum)
    Q_PROPERTY_CREATE_D(qreal, Value)
    Q_PROPERTY_CREATE_D(int, MajorTickCount)
    Q_PROPERTY_CREATE_D(int, MinorTickCount)
    Q_PROPERTY_CREATE_D(int, StartAngle)
    Q_PROPERTY_CREATE_D(int, SpanAngle)
    Q_PROPERTY_CREATE_D(int, ArcWidth)
    Q_PROPERTY_CREATE_D(int, ValuePixelSize)
    Q_PROPERTY_CREATE_D(bool, IsAnimated)
    Q_PROPERTY_CREATE_D(int, Decimals)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Unit)
    Q_PROPERTY(qreal pAnimatedValue MEMBER _animatedValue)
public:
    explicit ElaDashboardGaugePrivate(QObject* parent = nullptr);
    ~ElaDashboardGaugePrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    qreal _dangerPercent{0.85};
    qreal _warningPercent{0.65};
    qreal _tickWarningPercent{0.7};
    qreal _animatedValue{0};
};

#endif // ELADASHBOARDGAUGEPRIVATE_H
