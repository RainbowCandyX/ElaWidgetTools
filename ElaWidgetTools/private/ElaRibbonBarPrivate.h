#ifndef ELARIBBONBARPRIVATE_H
#define ELARIBBONBARPRIVATE_H

#include <QObject>
#include <QPointer>

#include "ElaDef.h"
#include "ElaProperty.h"

class QStackedWidget;
class QTimer;
class QVariantAnimation;
class ElaRibbonBar;
class ElaRibbonTabBar;
class ElaRibbonBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRibbonBar)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(bool, BackgroundTransparent)
public:
    explicit ElaRibbonBarPrivate(QObject* parent = nullptr);
    ~ElaRibbonBarPrivate() override;

    ElaRibbonTabBar* activeTabBar() const;

private:
    friend class ElaRibbonBar;
    QStackedWidget* _stack{nullptr};
    ElaRibbonTabBar* _internalTabBar{nullptr};
    QPointer<ElaRibbonTabBar> _externalTabBar;
    ElaThemeType::ThemeMode _themeMode;
    bool _isCollapsed{false};
    bool _isPinned{false};
    int _animationDuration{200};
    QVariantAnimation* _heightAnim{nullptr};
    QTimer* _autoHideTimer{nullptr};
};

#endif // ELARIBBONBARPRIVATE_H
