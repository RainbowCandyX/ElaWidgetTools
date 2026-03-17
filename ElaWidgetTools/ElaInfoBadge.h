#ifndef ELAINFOBADGE_H
#define ELAINFOBADGE_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaInfoBadgePrivate;
class ELA_EXPORT ElaInfoBadge : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaInfoBadge)
    Q_PROPERTY_CREATE_Q_H(int, Value)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, ElaIcon)
public:
    enum BadgeMode
    {
        Dot = 0,
        Value_,
        Icon
    };
    Q_ENUM(BadgeMode)

    explicit ElaInfoBadge(QWidget* parent = nullptr);
    explicit ElaInfoBadge(int value, QWidget* parent = nullptr);
    explicit ElaInfoBadge(ElaIconType::IconName icon, QWidget* parent = nullptr);
    ~ElaInfoBadge() override;

    void setBadgeMode(BadgeMode mode);
    BadgeMode getBadgeMode() const;

    void setMaxValue(int maxValue);
    int getMaxValue() const;

    enum Severity
    {
        Attention = 0,
        Informational,
        Success,
        Caution,
        Critical
    };
    Q_ENUM(Severity)

    void setSeverity(Severity severity);
    Severity getSeverity() const;

    void attachTo(QWidget* target);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELAINFOBADGE_H
