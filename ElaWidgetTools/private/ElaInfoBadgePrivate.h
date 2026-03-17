#ifndef ELAINFOBADGEPRIVATE_H
#define ELAINFOBADGEPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaInfoBadge.h"
class ElaInfoBadgePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaInfoBadge)
    Q_PROPERTY_CREATE_D(int, Value)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, ElaIcon)
public:
    explicit ElaInfoBadgePrivate(QObject* parent = nullptr);
    ~ElaInfoBadgePrivate() override;

private:
    ElaInfoBadge::BadgeMode _badgeMode{ElaInfoBadge::Dot};
    ElaInfoBadge::Severity _severity{ElaInfoBadge::Attention};
    int _maxValue{99};
    QWidget* _target{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    void _updatePosition();
    QColor _getSeverityColor() const;
};

#endif // ELAINFOBADGEPRIVATE_H
