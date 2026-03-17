#ifndef ELACOUNTDOWNPRIVATE_H
#define ELACOUNTDOWNPRIVATE_H

#include <QDateTime>
#include <QObject>

#include "ElaDef.h"
class QTimer;
class ElaCountdown;
class ElaCountdownPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCountdown)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsShowDays)
    Q_PROPERTY_CREATE_D(bool, IsShowHours)
    Q_PROPERTY_CREATE_D(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_D(bool, IsShowSeconds)
    Q_PROPERTY_CREATE_D(int, DigitWidth)
    Q_PROPERTY_CREATE_D(int, DigitHeight)
    Q_PROPERTY_CREATE_D(int, DigitSpacing)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
    Q_PROPERTY_CREATE(qreal, FlipAngle)
public:
    explicit ElaCountdownPrivate(QObject* parent = nullptr);
    ~ElaCountdownPrivate() override;

    Q_SLOT void onTimerTick();
    void _startFlipAnimation();

private:
    ElaThemeType::ThemeMode _themeMode;
    QTimer* _timer{nullptr};
    QDateTime _targetDateTime;
    qint64 _remainingSeconds{0};
    bool _isRunning{false};
    bool _useTargetMode{false};

    int _prevSeconds{-1};
    int _prevMinutes{-1};
    int _prevHours{-1};
    int _prevDays{-1};

    friend class ElaCountdown;
};

#endif // ELACOUNTDOWNPRIVATE_H
