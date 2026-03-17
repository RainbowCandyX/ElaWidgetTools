#include "ElaSheetPanelPrivate.h"

#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "ElaMaskWidget.h"
#include "ElaSheetPanel.h"
#include "ElaTheme.h"

ElaSheetPanelPrivate::ElaSheetPanelPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaSheetPanelPrivate::~ElaSheetPanelPrivate()
{
}

void ElaSheetPanelPrivate::_doOpenAnimation(ElaSheetPanelType::DetentLevel level)
{
    Q_Q(ElaSheetPanel);
    int targetSize = _getTargetSize(level);
    QWidget* parent = q->parentWidget();
    if (!parent)
    {
        return;
    }

    if (_pDirection == ElaSheetPanelType::Bottom)
    {
        _panelWidget->setGeometry(0, parent->height(), parent->width(), 0);
        QPropertyAnimation* animation = new QPropertyAnimation(_panelWidget, "geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0, parent->height(), parent->width(), 0));
        animation->setEndValue(QRect(0, parent->height() - targetSize, parent->width(), targetSize));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (_pDirection == ElaSheetPanelType::Right)
    {
        _panelWidget->setGeometry(parent->width(), 0, 0, parent->height());
        QPropertyAnimation* animation = new QPropertyAnimation(_panelWidget, "geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(parent->width(), 0, 0, parent->height()));
        animation->setEndValue(QRect(parent->width() - targetSize, 0, targetSize, parent->height()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _panelWidget->setGeometry(-targetSize, 0, 0, parent->height());
        QPropertyAnimation* animation = new QPropertyAnimation(_panelWidget, "geometry");
        animation->setDuration(350);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(QRect(0, 0, 0, parent->height()));
        animation->setEndValue(QRect(0, 0, targetSize, parent->height()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    _panelWidget->setStyleSheet(
        QString("background-color: %1; border-top-left-radius: %2px; border-top-right-radius: %2px;")
            .arg(ElaThemeColor(_themeMode, DialogBase).name())
            .arg(_pBorderRadius));

    _dragHandle->setStyleSheet(
        QString("background-color: %1; border-radius: 2px;")
            .arg(ElaThemeColor(_themeMode, BasicBorderDeep).name()));
    _dragHandle->setVisible(_pDragHandleVisible);
}

void ElaSheetPanelPrivate::_doCloseAnimation()
{
    Q_Q(ElaSheetPanel);
    QWidget* parent = q->parentWidget();
    if (!parent)
    {
        return;
    }
    _maskWidget->doMaskAnimation(0);

    QRect startRect = _panelWidget->geometry();
    QRect endRect;
    if (_pDirection == ElaSheetPanelType::Bottom)
    {
        endRect = QRect(0, parent->height(), parent->width(), 0);
    }
    else if (_pDirection == ElaSheetPanelType::Right)
    {
        endRect = QRect(parent->width(), 0, 0, parent->height());
    }
    else
    {
        endRect = QRect(0, 0, 0, parent->height());
    }

    QPropertyAnimation* animation = new QPropertyAnimation(_panelWidget, "geometry");
    connect(animation, &QPropertyAnimation::finished, q, [=]() {
        _panelWidget->setVisible(false);
        _isOpened = false;
        Q_EMIT q->closed();
    });
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InCubic);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaSheetPanelPrivate::_updatePanelGeometry(int targetSize, bool animate)
{
    Q_Q(ElaSheetPanel);
    QWidget* parent = q->parentWidget();
    if (!parent)
    {
        return;
    }
    QRect targetRect;
    if (_pDirection == ElaSheetPanelType::Bottom)
    {
        targetRect = QRect(0, parent->height() - targetSize, parent->width(), targetSize);
    }
    else if (_pDirection == ElaSheetPanelType::Right)
    {
        targetRect = QRect(parent->width() - targetSize, 0, targetSize, parent->height());
    }
    else
    {
        targetRect = QRect(0, 0, targetSize, parent->height());
    }

    if (animate)
    {
        QPropertyAnimation* animation = new QPropertyAnimation(_panelWidget, "geometry");
        animation->setDuration(250);
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setStartValue(_panelWidget->geometry());
        animation->setEndValue(targetRect);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _panelWidget->setGeometry(targetRect);
    }
}

int ElaSheetPanelPrivate::_getTargetSize(ElaSheetPanelType::DetentLevel level) const
{
    Q_Q(const ElaSheetPanel);
    QWidget* parent = q->parentWidget();
    if (!parent)
    {
        return 100;
    }
    int totalSize;
    if (_pDirection == ElaSheetPanelType::Bottom)
    {
        totalSize = parent->height();
    }
    else
    {
        totalSize = parent->width();
    }
    switch (level)
    {
    case ElaSheetPanelType::Peek:
        return static_cast<int>(totalSize * _pPeekRatio);
    case ElaSheetPanelType::Half:
        return static_cast<int>(totalSize * _pHalfRatio);
    case ElaSheetPanelType::Full:
        return static_cast<int>(totalSize * _pFullRatio);
    default:
        return static_cast<int>(totalSize * _pHalfRatio);
    }
}

ElaSheetPanelType::DetentLevel ElaSheetPanelPrivate::_getNearestDetent(int currentSize) const
{
    int peekSize = _getTargetSize(ElaSheetPanelType::Peek);
    int halfSize = _getTargetSize(ElaSheetPanelType::Half);
    int fullSize = _getTargetSize(ElaSheetPanelType::Full);

    int peekDist = qAbs(currentSize - peekSize);
    int halfDist = qAbs(currentSize - halfSize);
    int fullDist = qAbs(currentSize - fullSize);

    if (peekDist <= halfDist && peekDist <= fullDist)
    {
        return ElaSheetPanelType::Peek;
    }
    else if (halfDist <= fullDist)
    {
        return ElaSheetPanelType::Half;
    }
    return ElaSheetPanelType::Full;
}
