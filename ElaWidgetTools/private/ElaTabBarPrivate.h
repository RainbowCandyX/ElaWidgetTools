#ifndef ELATABBARPRIVATE_H
#define ELATABBARPRIVATE_H

#include "ElaProperty.h"
#include <QMimeData>
#include <QObject>
#include <QPixmap>
class ElaTabBar;
class ElaTabBarStyle;
class ElaIconButton;
class QTabBarPrivate;
class ElaTabBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTabBar)
    Q_PROPERTY_CREATE_D(bool, TabAddButtonVisible)

public:
    explicit ElaTabBarPrivate(QObject* parent = nullptr);
    ~ElaTabBarPrivate() override;

private:
    QMimeData* _mimeData{nullptr};
    ElaTabBarStyle* _style{nullptr};
    ElaIconButton* _tabAddButton{nullptr};
    QTabBarPrivate* _tabBarPrivate{nullptr};
};

#endif // ELATABBARPRIVATE_H
