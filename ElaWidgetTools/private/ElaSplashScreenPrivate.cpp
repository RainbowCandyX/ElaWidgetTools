#include "ElaSplashScreenPrivate.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include "ElaSplashScreen.h"

ElaSplashScreenPrivate::ElaSplashScreenPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaSplashScreenPrivate::~ElaSplashScreenPrivate()
{
}

void ElaSplashScreenPrivate::_doShowAnimation()
{
    Q_Q(ElaSplashScreen);
    q->setWindowOpacity(0);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(q, "windowOpacity");
    opacityAnimation->setDuration(400);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaSplashScreenPrivate::_doCloseAnimation()
{
    Q_Q(ElaSplashScreen);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(q, "windowOpacity");
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(1.0);
    opacityAnimation->setEndValue(0.0);
    opacityAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    connect(opacityAnimation, &QPropertyAnimation::finished, q, [=]() {
        q->QWidget::close();
        Q_EMIT q->closed();
    });
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
