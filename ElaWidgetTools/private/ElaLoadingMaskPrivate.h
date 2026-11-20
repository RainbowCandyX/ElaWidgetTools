#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELALOADINGMASKPRIVATE_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELALOADINGMASKPRIVATE_H_

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaLoadingMask;
class ElaProgressRing;
class ElaText;
class QWidget;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class ElaLoadingMaskPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaLoadingMask);
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(int, MaskAlpha)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE(int, CurrentMaskAlpha)
public:
    explicit ElaLoadingMaskPrivate(QObject* parent = nullptr);
    ~ElaLoadingMaskPrivate() override;

private:
    QWidget* _target{nullptr};
    QWidget* _contentWidget{nullptr};
    ElaProgressRing* _progressRing{nullptr};
    ElaText* _textLabel{nullptr};
    QGraphicsOpacityEffect* _contentOpacityEffect{nullptr};
    QPropertyAnimation* _maskAlphaAnimation{nullptr};
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    bool _isRunning{false};
    bool _isFinishing{false};
    void _fadeTo(int endValue);
};

#endif // ELAFRAMEWORK_ELAWIDGETTOOLS_PRIVATE_ELALOADINGMASKPRIVATE_H_
