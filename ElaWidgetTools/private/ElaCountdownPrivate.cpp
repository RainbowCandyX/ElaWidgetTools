#include "ElaCountdownPrivate.h"

#include <QDateTime>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaCountdown.h"

ElaCountdownPrivate::ElaCountdownPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaCountdownPrivate::~ElaCountdownPrivate()
{
}

void ElaCountdownPrivate::onTimerTick()
{
    Q_Q(ElaCountdown);
    if (_useTargetMode)
    {
        qint64 secs = QDateTime::currentDateTime().secsTo(_targetDateTime);
        _remainingSeconds = qMax(static_cast<qint64>(0), secs);
    }
    else
    {
        _remainingSeconds = qMax(static_cast<qint64>(0), _remainingSeconds - 1);
    }

    int curSeconds = static_cast<int>(_remainingSeconds % 60);
    if (_prevSeconds != curSeconds)
    {
        _startFlipAnimation();
        _prevSeconds = curSeconds;
    }
    _prevMinutes = static_cast<int>((_remainingSeconds % 3600) / 60);
    _prevHours = static_cast<int>((_remainingSeconds % 86400) / 3600);
    _prevDays = static_cast<int>(_remainingSeconds / 86400);

    Q_EMIT q->tick(_remainingSeconds);
    q->update();

    if (_remainingSeconds <= 0)
    {
        _isRunning = false;
        _timer->stop();
        Q_EMIT q->timeout();
    }
}

void ElaCountdownPrivate::_startFlipAnimation()
{
    Q_Q(ElaCountdown);
    _pFlipAngle = 0;
    QPropertyAnimation* flipAnim = new QPropertyAnimation(this, "pFlipAngle");
    connect(flipAnim, &QPropertyAnimation::valueChanged, q, [=]() { q->update(); });
    flipAnim->setDuration(300);
    flipAnim->setStartValue(0.0);
    flipAnim->setEndValue(180.0);
    flipAnim->setEasingCurve(QEasingCurve::OutCubic);
    flipAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
