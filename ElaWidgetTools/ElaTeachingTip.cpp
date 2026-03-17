#include "ElaTeachingTip.h"

#include <QApplication>
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>

#include "ElaTheme.h"
#include "private/ElaTeachingTipPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, QString, SubTitle)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, ElaIconType::IconName, TipIcon)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, QPixmap, HeroImage)
Q_PROPERTY_CREATE_Q_CPP(ElaTeachingTip, bool, IsLightDismiss)

ElaTeachingTipPrivate::ElaTeachingTipPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaTeachingTipPrivate::~ElaTeachingTipPrivate()
{
}

QPoint ElaTeachingTipPrivate::_calculatePosition() const
{
	Q_Q(const ElaTeachingTip);
	if (!_target)
	{
		return QPoint(0, 0);
	}
	QPoint targetCenter = _target->mapToGlobal(QPoint(_target->width() / 2, _target->height() / 2));
	ElaTeachingTip::TailPosition pos = _tailPosition;
	if (pos == ElaTeachingTip::Auto)
	{
		pos = _calculateAutoTailPosition();
	}

	int gap = _tailSize + 4;
	QPoint result;
	switch (pos)
	{
		case ElaTeachingTip::Bottom:
			// Tip appears above target, tail points down
			result = QPoint(targetCenter.x() - q->width() / 2,
			                _target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
			break;
		case ElaTeachingTip::Top:
			// Tip appears below target, tail points up
			result = QPoint(targetCenter.x() - q->width() / 2,
			                _target->mapToGlobal(QPoint(0, _target->height())).y() + gap);
			break;
		case ElaTeachingTip::Right:
			// Tip appears left of target, tail points right
			result = QPoint(_target->mapToGlobal(QPoint(0, 0)).x() - q->width() - gap,
			                targetCenter.y() - q->height() / 2);
			break;
		case ElaTeachingTip::Left:
			// Tip appears right of target, tail points left
			result = QPoint(_target->mapToGlobal(QPoint(_target->width(), 0)).x() + gap,
			                targetCenter.y() - q->height() / 2);
			break;
		default:
			result = QPoint(targetCenter.x() - q->width() / 2,
			                _target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
			break;
	}

	// Screen boundary clamping
	QScreen *screen = QApplication::screenAt(targetCenter);
	if (screen)
	{
		QRect screenGeo = screen->availableGeometry();
		result.setX(qBound(screenGeo.left() + 4, result.x(), screenGeo.right() - q->width() - 4));
		result.setY(qBound(screenGeo.top() + 4, result.y(), screenGeo.bottom() - q->height() - 4));
	}
	return result;
}

ElaTeachingTip::TailPosition ElaTeachingTipPrivate::_calculateAutoTailPosition() const
{
	Q_Q(const ElaTeachingTip);
	if (!_target)
	{
		return ElaTeachingTip::Bottom;
	}
	QPoint targetGlobal = _target->mapToGlobal(QPoint(0, 0));
	QScreen *screen = QApplication::screenAt(targetGlobal);
	if (!screen)
	{
		return ElaTeachingTip::Bottom;
	}
	QRect screenGeo = screen->availableGeometry();
	int spaceAbove = targetGlobal.y() - screenGeo.top();
	int spaceBelow = screenGeo.bottom() - targetGlobal.y() - _target->height();

	// Prefer showing above (tail at bottom)
	if (spaceAbove >= q->sizeHint().height() + _tailSize + 4)
	{
		return ElaTeachingTip::Bottom;
	}
	if (spaceBelow >= q->sizeHint().height() + _tailSize + 4)
	{
		return ElaTeachingTip::Top;
	}
	return ElaTeachingTip::Bottom;
}

void ElaTeachingTipPrivate::_doShowAnimation()
{
	Q_Q(ElaTeachingTip);
	QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(q);
	q->setGraphicsEffect(effect);
	_pOpacity = 0.0;
	effect->setOpacity(0);

	QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", q);
	animation->setDuration(200);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->setEasingCurve(QEasingCurve::OutCubic);
	QObject::connect(animation, &QPropertyAnimation::finished, q, [=]()
	{
		q->setGraphicsEffect(nullptr);
	});
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

ElaTeachingTip::ElaTeachingTip(QWidget *parent)
	: QWidget{nullptr}, d_ptr(new ElaTeachingTipPrivate())
{
	Q_UNUSED(parent);
	Q_D(ElaTeachingTip);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pTitle = "";
	d->_pSubTitle = "";
	d->_pContent = "";
	d->_pTipIcon = ElaIconType::None;
	d->_pIsLightDismiss = true;
	d->_pOpacity = 1.0;
	setObjectName("ElaTeachingTip");
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setFixedWidth(320);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	hide();
}

ElaTeachingTip::~ElaTeachingTip()
{
}

void ElaTeachingTip::setTailPosition(TailPosition position)
{
	Q_D(ElaTeachingTip);
	d->_tailPosition = position;
}

ElaTeachingTip::TailPosition ElaTeachingTip::getTailPosition() const
{
	return d_ptr->_tailPosition;
}

void ElaTeachingTip::setTarget(QWidget *target)
{
	Q_D(ElaTeachingTip);
	if (d->_target)
	{
		d->_target->removeEventFilter(this);
	}
	d->_target = target;
	if (target)
	{
		target->installEventFilter(this);
	}
}

QWidget *ElaTeachingTip::getTarget() const
{
	return d_ptr->_target;
}

void ElaTeachingTip::setCloseButtonVisible(bool visible)
{
	Q_D(ElaTeachingTip);
	d->_closeButtonVisible = visible;
	update();
}

void ElaTeachingTip::addAction(const QString &text, const std::function<void()> &callback)
{
	Q_D(ElaTeachingTip);
	d->_actions.append({text, callback});
}

void ElaTeachingTip::clearActions()
{
	Q_D(ElaTeachingTip);
	d->_actions.clear();
}

void ElaTeachingTip::showTip()
{
	Q_D(ElaTeachingTip);
	if (d->_pIsLightDismiss)
	{
		qApp->installEventFilter(this);
	}
	// Calculate size based on content
	int contentHeight = d->_shadowBorderWidth * 2;
	if (!d->_pHeroImage.isNull())
	{
		QPixmap scaled = d->_pHeroImage.scaledToWidth(width() - d->_shadowBorderWidth * 2, Qt::SmoothTransformation);
		contentHeight += scaled.height();
	}
	contentHeight += 12; // top padding
	if (!d->_pTitle.isEmpty())
	{
		contentHeight += 24;
	}
	if (!d->_pSubTitle.isEmpty())
	{
		contentHeight += 20;
	}
	if (!d->_pContent.isEmpty())
	{
		QFont contentFont = font();
		contentFont.setPixelSize(13);
		QFontMetrics fm(contentFont);
		int textWidth = width() - d->_shadowBorderWidth * 2 - 30;
		QRect boundingRect = fm.boundingRect(QRect(0, 0, textWidth, 0), Qt::TextWordWrap, d->_pContent);
		contentHeight += boundingRect.height() + 8;
	}
	if (!d->_actions.isEmpty())
	{
		contentHeight += 44;
	}
	contentHeight += 12; // bottom padding
	setFixedHeight(contentHeight);

	QPoint pos = d->_calculatePosition();
	move(pos);
	show();
	raise();
	d->_doShowAnimation();
}

void ElaTeachingTip::closeTip()
{
	Q_D(ElaTeachingTip);
	qApp->removeEventFilter(this);
	hide();
	Q_EMIT closed();
}

void ElaTeachingTip::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaTeachingTip);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	// Shadow
	eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

	// Main body
	QRect bodyRect = rect().adjusted(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, PopupBorder), 1));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(bodyRect, d->_pBorderRadius, d->_pBorderRadius);

	int currentY = d->_shadowBorderWidth;
	int contentLeft = d->_shadowBorderWidth + 15;
	int contentWidth = bodyRect.width() - 30;

	// Hero image
	if (!d->_pHeroImage.isNull())
	{
		QPixmap scaled = d->_pHeroImage.scaledToWidth(bodyRect.width(), Qt::SmoothTransformation);
		QRect imageRect(d->_shadowBorderWidth, currentY, bodyRect.width(), scaled.height());

		// Clip to rounded top corners
		QPainterPath clipPath;
		clipPath.addRoundedRect(QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), scaled.height() + d->_pBorderRadius), d->_pBorderRadius, d->_pBorderRadius);
		painter.save();
		painter.setClipPath(clipPath);
		painter.drawPixmap(imageRect, scaled);
		painter.restore();

		currentY += scaled.height();
	}

	currentY += 12;

	// Close button
	if (d->_closeButtonVisible)
	{
		QFont closeFont = QFont("ElaAwesome");
		closeFont.setPixelSize(12);
		painter.setFont(closeFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		d->_closeButtonRect = QRect(bodyRect.right() - 28, d->_shadowBorderWidth + 8, 20, 20);
		painter.drawText(d->_closeButtonRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::Xmark));
	}

	// Icon + Title
	if (!d->_pTitle.isEmpty())
	{
		int titleX = contentLeft;
		if (d->_pTipIcon != ElaIconType::None)
		{
			QFont iconFont = QFont("ElaAwesome");
			iconFont.setPixelSize(16);
			painter.setFont(iconFont);
			painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
			painter.drawText(QRect(contentLeft, currentY, 20, 22), Qt::AlignVCenter | Qt::AlignLeft, QChar((unsigned short) d->_pTipIcon));
			titleX += 26;
		}
		QFont titleFont = font();
		titleFont.setPixelSize(14);
		titleFont.setBold(true);
		painter.setFont(titleFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		painter.drawText(QRect(titleX, currentY, contentWidth - (titleX - contentLeft) - 25, 22), Qt::AlignVCenter | Qt::AlignLeft, d->_pTitle);
		currentY += 24;
	}

	// SubTitle
	if (!d->_pSubTitle.isEmpty())
	{
		QFont subFont = font();
		subFont.setPixelSize(12);
		painter.setFont(subFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		painter.drawText(QRect(contentLeft, currentY, contentWidth, 18), Qt::AlignVCenter | Qt::AlignLeft, d->_pSubTitle);
		currentY += 20;
	}

	// Content text (word wrap)
	if (!d->_pContent.isEmpty())
	{
		currentY += 4;
		QFont contentFont = font();
		contentFont.setPixelSize(13);
		painter.setFont(contentFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QRect textRect(contentLeft, currentY, contentWidth, height() - currentY - d->_shadowBorderWidth);
		painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, d->_pContent);
		QFontMetrics fm(contentFont);
		QRect boundingRect = fm.boundingRect(textRect, Qt::TextWordWrap, d->_pContent);
		currentY += boundingRect.height() + 4;
	}

	// Action buttons area
	if (!d->_actions.isEmpty())
	{
		currentY += 8;
		int buttonWidth = 80;
		int buttonHeight = 30;
		int buttonSpacing = 8;
		int totalWidth = d->_actions.size() * buttonWidth + (d->_actions.size() - 1) * buttonSpacing;
		int startX = bodyRect.right() - 15 - totalWidth;

		QFont buttonFont = font();
		buttonFont.setPixelSize(12);
		painter.setFont(buttonFont);

		for (int i = 0; i < d->_actions.size(); ++i)
		{
			QRect btnRect(startX + i * (buttonWidth + buttonSpacing), currentY, buttonWidth, buttonHeight);
			if (i == d->_actions.size() - 1)
			{
				// Primary button (last)
				painter.setPen(Qt::NoPen);
				painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
				painter.drawRoundedRect(btnRect, 4, 4);
				painter.setPen(Qt::white);
			}
			else
			{
				painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
				painter.drawRoundedRect(btnRect, 4, 4);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
			}
			painter.drawText(btnRect, Qt::AlignCenter, d->_actions[i].text);
		}
	}

	// Tail/arrow
	if (d->_target)
	{
		ElaTeachingTip::TailPosition tailPos = d->_tailPosition;
		if (tailPos == ElaTeachingTip::Auto)
		{
			tailPos = d->_calculateAutoTailPosition();
		}
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));

		QPolygonF triangle;
		int ts = d->_tailSize;
		switch (tailPos)
		{
			case ElaTeachingTip::Bottom:
			{
				// Tail at bottom center
				int cx = width() / 2;
				int by = bodyRect.bottom();
				triangle << QPointF(cx - ts, by) << QPointF(cx + ts, by) << QPointF(cx, by + ts);
				break;
			}
			case ElaTeachingTip::Top:
			{
				int cx = width() / 2;
				int ty = bodyRect.top();
				triangle << QPointF(cx - ts, ty) << QPointF(cx + ts, ty) << QPointF(cx, ty - ts);
				break;
			}
			case ElaTeachingTip::Left:
			{
				int cy = height() / 2;
				int lx = bodyRect.left();
				triangle << QPointF(lx, cy - ts) << QPointF(lx, cy + ts) << QPointF(lx - ts, cy);
				break;
			}
			case ElaTeachingTip::Right:
			{
				int cy = height() / 2;
				int rx = bodyRect.right();
				triangle << QPointF(rx, cy - ts) << QPointF(rx, cy + ts) << QPointF(rx + ts, cy);
				break;
			}
			default:
				break;
		}
		if (!triangle.isEmpty())
		{
			QPainterPath tailPath;
			tailPath.addPolygon(triangle);
			tailPath.closeSubpath();
			painter.drawPath(tailPath);
		}
	}
}

bool ElaTeachingTip::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaTeachingTip);
	if (watched == d->_target)
	{
		if (event->type() == QEvent::Move || event->type() == QEvent::Resize)
		{
			if (isVisible())
			{
				move(d->_calculatePosition());
			}
		}
		else if (event->type() == QEvent::Hide || event->type() == QEvent::Destroy)
		{
			closeTip();
		}
	}
	// Light dismiss: click outside closes the tip
	if (d->_pIsLightDismiss && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (watched != this && !this->geometry().contains(this->mapFromGlobal(mouseEvent->globalPos())))
		{
			closeTip();
		}
	}
	return QWidget::eventFilter(watched, event);
}

void ElaTeachingTip::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}

void ElaTeachingTip::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaTeachingTip);
	// Close button hit test
	if (d->_closeButtonVisible && d->_closeButtonRect.contains(event->pos()))
	{
		Q_EMIT closeButtonClicked();
		closeTip();
		return;
	}
	// Action button hit test
	if (!d->_actions.isEmpty())
	{
		QRect bodyRect = rect().adjusted(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
		int buttonWidth = 80;
		int buttonHeight = 30;
		int buttonSpacing = 8;
		int totalWidth = d->_actions.size() * buttonWidth + (d->_actions.size() - 1) * buttonSpacing;
		int startX = bodyRect.right() - 15 - totalWidth;

		// Approximate button Y
		int buttonY = height() - d->_shadowBorderWidth - 12 - buttonHeight;

		for (int i = 0; i < d->_actions.size(); ++i)
		{
			QRect btnRect(startX + i * (buttonWidth + buttonSpacing), buttonY, buttonWidth, buttonHeight);
			if (btnRect.contains(event->pos()))
			{
				if (d->_actions[i].callback)
				{
					d->_actions[i].callback();
				}
				return;
			}
		}
	}
	QWidget::mousePressEvent(event);
}