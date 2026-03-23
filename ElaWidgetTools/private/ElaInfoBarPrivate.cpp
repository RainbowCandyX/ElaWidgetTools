#include "ElaInfoBarPrivate.h"

#include <QPropertyAnimation>

#include "ElaInfoBar.h"
#include "ElaTheme.h"

ElaInfoBarPrivate::ElaInfoBarPrivate(QObject *parent) : QObject(parent)
{
}

ElaInfoBarPrivate::~ElaInfoBarPrivate()
{
}

QColor ElaInfoBarPrivate::_getSeverityColor() const
{
	switch (_pSeverity)
	{
		case ElaInfoBarType::Success:
			return QColor(0x0F, 0x7B, 0x0F);
		case ElaInfoBarType::Warning:
			return QColor(0x9D, 0x5D, 0x00);
		case ElaInfoBarType::Error:
			return QColor(0xC4, 0x2B, 0x1C);
		case ElaInfoBarType::Informational:
		default:
			return ElaThemeColor(_themeMode, PrimaryNormal);
	}
}

ElaIconType::IconName ElaInfoBarPrivate::_getSeverityIcon() const
{
	if (_pInfoBarIcon != ElaIconType::None)
	{
		return _pInfoBarIcon;
	}
	switch (_pSeverity)
	{
		case ElaInfoBarType::Success:
			return ElaIconType::CircleCheck;
		case ElaInfoBarType::Warning:
			return ElaIconType::TriangleExclamation;
		case ElaInfoBarType::Error:
			return ElaIconType::CircleXmark;
		case ElaInfoBarType::Informational:
		default:
			return ElaIconType::CircleInfo;
	}
}

void ElaInfoBarPrivate::_doCloseAnimation()
{
	Q_Q(ElaInfoBar);
	if (_isClosing)
	{
		return;
	}
	_isClosing = true;

	QPropertyAnimation *heightAnimation = new QPropertyAnimation(q, "maximumHeight", q);
	heightAnimation->setDuration(250);
	heightAnimation->setStartValue(q->height());
	heightAnimation->setEndValue(0);
	heightAnimation->setEasingCurve(QEasingCurve::InCubic);

	QObject::connect(heightAnimation, &QPropertyAnimation::finished, q, [=]()
	{
		q->hide();
		q->setMaximumHeight(QWIDGETSIZE_MAX);
		Q_EMIT q->closed();
	});
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}