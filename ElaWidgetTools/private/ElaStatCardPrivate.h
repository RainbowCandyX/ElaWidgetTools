#ifndef ELASTATCARDPRIVATE_H
#define ELASTATCARDPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaStatCard;
class ElaStatCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaStatCard)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Value)
    Q_PROPERTY_CREATE_D(QString, Description)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, CardIcon)
public:
    explicit ElaStatCardPrivate(QObject* parent = nullptr);
    ~ElaStatCardPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaStatCard::TrendType _trend{ElaStatCard::None};
    QString _trendText;
};

#endif // ELASTATCARDPRIVATE_H
