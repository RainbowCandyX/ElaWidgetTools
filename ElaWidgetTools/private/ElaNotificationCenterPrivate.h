#ifndef ELANOTIFICATIONCENTERPRIVATE_H
#define ELANOTIFICATIONCENTERPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaNotificationCenter.h"
class QPropertyAnimation;
class ElaNotificationCenterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaNotificationCenter)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, PanelWidth)

public:
    explicit ElaNotificationCenterPrivate(QObject* parent = nullptr);
    ~ElaNotificationCenterPrivate() override;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QList<ElaNotificationCenter::NotificationItem> _items;
    QPropertyAnimation* _slideAnimation{nullptr};
    bool _isVisible{false};
    ElaThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{8};
    int _scrollOffset{0};
    QWidget* _anchorWidget{nullptr};

    int _hitTestCard(const QPoint& pos) const;
    QRect _clearAllButtonRect() const;
    bool _hitTestClearAll(const QPoint& pos) const;
};

#endif // ELANOTIFICATIONCENTERPRIVATE_H
