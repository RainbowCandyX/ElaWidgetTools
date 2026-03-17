#ifndef ELATEACHINGTIPPRIVATE_H
#define ELATEACHINGTIPPRIVATE_H

#include <QObject>
#include <functional>

#include "ElaDef.h"
#include "ElaTeachingTip.h"
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QPropertyAnimation;
class ElaTeachingTipPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTeachingTip)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, TipIcon)
    Q_PROPERTY_CREATE_D(QPixmap, HeroImage)
    Q_PROPERTY_CREATE_D(bool, IsLightDismiss)
    Q_PROPERTY_CREATE(qreal, Opacity)
public:
    explicit ElaTeachingTipPrivate(QObject* parent = nullptr);
    ~ElaTeachingTipPrivate() override;

private:
    struct ActionItem
    {
        QString text;
        std::function<void()> callback;
    };

    ElaTeachingTip::TailPosition _tailPosition{ElaTeachingTip::Auto};
    QWidget* _target{nullptr};
    bool _closeButtonVisible{true};
    QList<ActionItem> _actions;
    QRect _closeButtonRect;
    int _shadowBorderWidth{8};
    int _tailSize{8};
    ElaThemeType::ThemeMode _themeMode;

    QPoint _calculatePosition() const;
    ElaTeachingTip::TailPosition _calculateAutoTailPosition() const;
    void _doShowAnimation();
};

#endif // ELATEACHINGTIPPRIVATE_H
