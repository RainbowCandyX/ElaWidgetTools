#include "ElaSpotlightPrivate.h"

#include <QPropertyAnimation>

#include "ElaPushButton.h"
#include "ElaSpotlight.h"
#include "ElaText.h"
#include "ElaTheme.h"

ElaSpotlightPrivate::ElaSpotlightPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaSpotlightPrivate::~ElaSpotlightPrivate()
{
}

void ElaSpotlightPrivate::_showStep(int index, bool animate)
{
    Q_Q(ElaSpotlight);
    if (index < 0 || index >= _steps.size())
    {
        return;
    }
    _currentStep = index;
    const ElaSpotlight::SpotlightStep& step = _steps[_currentStep];
    _currentTarget = step.target;
    _pIsCircle = step.isCircle;

    _tipTitle->setText(step.title);
    _tipContent->setText(step.content);
    _tipTitle->setVisible(!step.title.isEmpty());
    _tipContent->setVisible(!step.content.isEmpty());

    if (_steps.size() > 1)
    {
        _prevButton->setVisible(_currentStep > 0);
        _nextButton->setText(_currentStep >= _steps.size() - 1 ? "完成" : "下一步");
        _stepIndicator->setVisible(true);
        _stepIndicator->setText(QString("%1 / %2").arg(_currentStep + 1).arg(_steps.size()));
    }

    QRectF targetRect = _getTargetRect(_currentTarget);

    if (animate && _pSpotlightRect.isValid())
    {
        QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pSpotlightRect");
        connect(moveAnimation, &QPropertyAnimation::valueChanged, q, [=]() {
            q->update();
            _updateTipPosition();
        });
        moveAnimation->setDuration(350);
        moveAnimation->setEasingCurve(QEasingCurve::InOutCubic);
        moveAnimation->setStartValue(_pSpotlightRect);
        moveAnimation->setEndValue(targetRect);
        moveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _pSpotlightRect = targetRect;
        q->update();
    }

    _updateTipPosition();
    _tipWidget->setVisible(true);
    _tipWidget->raise();

    Q_EMIT q->stepChanged(_currentStep);
}

void ElaSpotlightPrivate::_updateOverlay()
{
    Q_Q(ElaSpotlight);
    if (_currentTarget)
    {
        _pSpotlightRect = _getTargetRect(_currentTarget);
        q->update();
    }
}

void ElaSpotlightPrivate::_updateTipPosition()
{
    Q_Q(ElaSpotlight);
    if (!_currentTarget)
    {
        return;
    }
    _tipWidget->adjustSize();
    int tipWidth = qMin(300, q->width() - 40);
    _tipWidget->setFixedWidth(tipWidth);
    _tipWidget->adjustSize();

    QRectF spotRect = _pSpotlightRect;
    int tipHeight = _tipWidget->sizeHint().height();
    int margin = 12;

    int tipX = static_cast<int>(spotRect.center().x() - tipWidth / 2);
    int tipY = static_cast<int>(spotRect.bottom() + margin);

    if (tipY + tipHeight > q->height() - 10)
    {
        tipY = static_cast<int>(spotRect.top() - tipHeight - margin);
    }

    tipX = qMax(10, qMin(tipX, q->width() - tipWidth - 10));
    tipY = qMax(10, tipY);

    _tipWidget->move(tipX, tipY);

    _tipWidget->setStyleSheet(
        QString("background-color: %1; border-radius: 8px; border: 1px solid %2;")
            .arg(ElaThemeColor(_themeMode, DialogBase).name())
            .arg(ElaThemeColor(_themeMode, PopupBorder).name()));
}

QRectF ElaSpotlightPrivate::_getTargetRect(QWidget* target) const
{
    Q_Q(const ElaSpotlight);
    if (!target || !q->parentWidget())
    {
        return QRectF();
    }
    QPoint topLeft = target->mapTo(q->parentWidget(), QPoint(0, 0));
    QRectF rect(topLeft, target->size());
    rect.adjust(-_pPadding, -_pPadding, _pPadding, _pPadding);
    return rect;
}
