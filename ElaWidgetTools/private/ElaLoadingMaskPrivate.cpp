#include "ElaLoadingMaskPrivate.h"

#include <QPropertyAnimation>

ElaLoadingMaskPrivate::ElaLoadingMaskPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaLoadingMaskPrivate::~ElaLoadingMaskPrivate()
{
}

void ElaLoadingMaskPrivate::_fadeTo(int endValue)
{
    if (!_maskAlphaAnimation)
    {
        return;
    }
    _maskAlphaAnimation->stop();
    _maskAlphaAnimation->setDuration(_pAnimationDuration);
    _maskAlphaAnimation->setStartValue(_pCurrentMaskAlpha);
    _maskAlphaAnimation->setEndValue(endValue);
    _maskAlphaAnimation->start();
}
