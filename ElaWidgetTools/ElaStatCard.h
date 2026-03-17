#ifndef ELASTATCARD_H
#define ELASTATCARD_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaStatCardPrivate;
class ELA_EXPORT ElaStatCard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaStatCard)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Title)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Value)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Description)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, CardIcon)
public:
    enum TrendType
    {
        None = 0,
        Up,
        Down,
        Neutral
    };
    Q_ENUM(TrendType)

    explicit ElaStatCard(QWidget* parent = nullptr);
    ~ElaStatCard() override;

    void setTrend(TrendType trend);
    TrendType getTrend() const;

    void setTrendText(const QString& text);
    QString getTrendText() const;

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELASTATCARD_H
