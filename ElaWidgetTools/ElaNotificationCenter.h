#ifndef ELANOTIFICATIONCENTER_H
#define ELANOTIFICATIONCENTER_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaNotificationCenterPrivate;
class ELA_EXPORT ElaNotificationCenter : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNotificationCenter)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, PanelWidth)

public:
    struct NotificationItem
    {
        QString title;
        QString content;
        QString timestamp;
        ElaIconType::IconName icon = ElaIconType::None;
    };

    explicit ElaNotificationCenter(QWidget* parent = nullptr);
    ~ElaNotificationCenter();

    void addNotification(const NotificationItem& item);
    void clearAll();
    int getNotificationCount() const;

    void showPanel(QWidget* anchor);
    void hidePanel();
    bool isPanelVisible() const;

Q_SIGNALS:
    Q_SIGNAL void notificationClicked(int index);
    Q_SIGNAL void panelVisibilityChanged(bool visible);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELANOTIFICATIONCENTER_H
