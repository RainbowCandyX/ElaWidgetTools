#include "ElaInfoBadge.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaInfoBadgePrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaInfoBadge, int, Value)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBadge, ElaIconType::IconName, ElaIcon)

ElaInfoBadgePrivate::ElaInfoBadgePrivate(QObject *parent)
	: QObject(parent)
{
}

ElaInfoBadgePrivate::~ElaInfoBadgePrivate()
{
}

void ElaInfoBadgePrivate::_updatePosition()
{
	Q_Q(ElaInfoBadge);
	if (!_target)
	{
		return;
	}
	// Position at inner top-right corner of target
	int x = _target->width() - q->width() - 2;
	int y = 2;
	q->move(x, y);
}

QColor ElaInfoBadgePrivate::_getSeverityColor() const
{
	switch (_severity)
	{
		case ElaInfoBadge::Attention:
			return ElaThemeColor(_themeMode, StatusDanger);
		case ElaInfoBadge::Informational:
			return ElaThemeColor(_themeMode, PrimaryNormal);
		case ElaInfoBadge::Success:
			return QColor(0x0F, 0x7B, 0x0F);
		case ElaInfoBadge::Caution:
			return QColor(0x9D, 0x5D, 0x00);
		case ElaInfoBadge::Critical:
			return ElaThemeColor(_themeMode, StatusDanger);
	}
	return ElaThemeColor(_themeMode, StatusDanger);
}

ElaInfoBadge::ElaInfoBadge(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaInfoBadgePrivate())
{
	Q_D(ElaInfoBadge);
	d->q_ptr = this;
	d->_pValue = 0;
	d->_pElaIcon = ElaIconType::None;
	setObjectName("ElaInfoBadge");
	setAttribute(Qt::WA_TransparentForMouseEvents);
	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	connect(this, &ElaInfoBadge::pValueChanged, this, [=]()
	{
		updateGeometry();
		adjustSize();
		d->_updatePosition();
		update();
	});
}

ElaInfoBadge::ElaInfoBadge(int value, QWidget *parent)
	: ElaInfoBadge(parent)
{
	Q_D(ElaInfoBadge);
	d->_badgeMode = Value_;
	d->_pValue = value;
}

ElaInfoBadge::ElaInfoBadge(ElaIconType::IconName icon, QWidget *parent)
	: ElaInfoBadge(parent)
{
	Q_D(ElaInfoBadge);
	d->_badgeMode = Icon;
	d->_pElaIcon = icon;
}

ElaInfoBadge::~ElaInfoBadge()
{
}

void ElaInfoBadge::setBadgeMode(BadgeMode mode)
{
	Q_D(ElaInfoBadge);
	d->_badgeMode = mode;
	updateGeometry();
	adjustSize();
	d->_updatePosition();
	update();
}

ElaInfoBadge::BadgeMode ElaInfoBadge::getBadgeMode() const
{
	return d_ptr->_badgeMode;
}

void ElaInfoBadge::setMaxValue(int maxValue)
{
	Q_D(ElaInfoBadge);
	d->_maxValue = maxValue;
	update();
}

int ElaInfoBadge::getMaxValue() const
{
	return d_ptr->_maxValue;
}

void ElaInfoBadge::setSeverity(Severity severity)
{
	Q_D(ElaInfoBadge);
	d->_severity = severity;
	update();
}

ElaInfoBadge::Severity ElaInfoBadge::getSeverity() const
{
	return d_ptr->_severity;
}

void ElaInfoBadge::attachTo(QWidget *target)
{
	Q_D(ElaInfoBadge);
	if (d->_target)
	{
		d->_target->removeEventFilter(this);
	}
	d->_target = target;
	if (target)
	{
		setParent(target);
		target->installEventFilter(this);
		raise();
		adjustSize();
		d->_updatePosition();
		show();
	}
}

bool ElaInfoBadge::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaInfoBadge);
	if (watched == d->_target)
	{
		if (event->type() == QEvent::Resize || event->type() == QEvent::Move)
		{
			d->_updatePosition();
			raise();
		}
	}
	return QWidget::eventFilter(watched, event);
}

void ElaInfoBadge::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaInfoBadge);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	QColor badgeColor = d->_getSeverityColor();
	painter.setBrush(badgeColor);

	// White outline for contrast
	painter.setPen(QPen(Qt::white, 2));

	switch (d->_badgeMode)
	{
		case Dot:
		{
			QRectF dotRect(1, 1, width() - 2, height() - 2);
			painter.drawEllipse(dotRect);
			painter.setPen(Qt::NoPen);
			painter.drawEllipse(dotRect.adjusted(1, 1, -1, -1));
			break;
		}
		case Value_:
		{
			QString text;
			if (d->_pValue > d->_maxValue)
			{
				text = QString::number(d->_maxValue) + "+";
			}
			else
			{
				text = QString::number(d->_pValue);
			}
			QRectF badgeRect(1, 1, width() - 2, height() - 2);
			painter.drawRoundedRect(badgeRect, badgeRect.height() / 2.0, badgeRect.height() / 2.0);
			painter.setPen(Qt::NoPen);
			painter.drawRoundedRect(badgeRect, badgeRect.height() / 2.0, badgeRect.height() / 2.0);
			painter.setPen(Qt::white);
			QFont font = this->font();
			font.setPixelSize(10);
			font.setBold(true);
			painter.setFont(font);
			painter.drawText(rect(), Qt::AlignCenter, text);
			break;
		}
		case Icon:
		{
			QRectF iconRect(1, 1, width() - 2, height() - 2);
			painter.drawEllipse(iconRect);
			painter.setPen(Qt::NoPen);
			painter.drawEllipse(iconRect);
			QFont iconFont = QFont("ElaAwesome");
			iconFont.setPixelSize(height() * 0.55);
			painter.setFont(iconFont);
			painter.setPen(Qt::white);
			painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short) d->_pElaIcon));
			break;
		}
	}
}

QSize ElaInfoBadge::sizeHint() const
{
	Q_D(const ElaInfoBadge);
	switch (d->_badgeMode)
	{
		case Dot:
			return QSize(10, 10);
		case Value_:
		{
			QString text;
			if (d->_pValue > d->_maxValue)
			{
				text = QString::number(d->_maxValue) + "+";
			}
			else
			{
				text = QString::number(d->_pValue);
			}
			QFont font = this->font();
			font.setPixelSize(10);
			font.setBold(true);
			QFontMetrics fm(font);
			int textWidth = fm.horizontalAdvance(text);
			int w = qMax(16, textWidth + 8);
			return QSize(w, 16);
		}
		case Icon:
			return QSize(16, 16);
	}
	return QSize(10, 10);
}