#include "ElaFloatButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaMenu.h"
#include "ElaTheme.h"
#include "private/ElaFloatButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaFloatButton, int, ButtonSize)

void ElaFloatButton::setMargin(int Margin)
{
	Q_D(ElaFloatButton);
	d->_pMargin = Margin;
	d->updatePosition();
	Q_EMIT pMarginChanged();
}

int ElaFloatButton::getMargin() const
{
	return d_ptr->_pMargin;
}

ElaFloatButton::ElaFloatButton(QWidget *parent) : QWidget{parent}, d_ptr(new ElaFloatButtonPrivate())
{
	Q_D(ElaFloatButton);
	d->q_ptr = this;
	d->_pButtonSize = 56;
	d->_pMargin = 30;
	d->_pHoverAlpha = 0;
	d->_themeMode = eTheme->getThemeMode();

	setFixedSize(d->_pButtonSize, d->_pButtonSize);
	setCursor(Qt::PointingHandCursor);
	setMouseTracking(true);
	raise();

	if (parent)
	{
		parent->installEventFilter(this);
		d->updatePosition();
	}

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaFloatButton::ElaFloatButton(ElaIconType::IconName icon, QWidget *parent)
	: ElaFloatButton(parent)
{
	Q_D(ElaFloatButton);
	d->_icon = icon;
}

ElaFloatButton::ElaFloatButton(ElaIconType::IconName icon, Position position, QWidget *parent)
	: ElaFloatButton(parent)
{
	Q_D(ElaFloatButton);
	d->_icon = icon;
	d->_position = (int) position;
	d->updatePosition();
}

ElaFloatButton::~ElaFloatButton()
{
}

void ElaFloatButton::setIcon(ElaIconType::IconName icon)
{
	Q_D(ElaFloatButton);
	d->_icon = icon;
	update();
}

ElaIconType::IconName ElaFloatButton::getIcon() const
{
	return d_ptr->_icon;
}

void ElaFloatButton::setPosition(Position position)
{
	Q_D(ElaFloatButton);
	d->_position = (int) position;
	d->updatePosition();
}

ElaFloatButton::Position ElaFloatButton::getPosition() const
{
	return (Position) d_ptr->_position;
}

void ElaFloatButton::setMenu(ElaMenu *menu)
{
	Q_D(ElaFloatButton);
	d->_menu = menu;
}

ElaMenu *ElaFloatButton::getMenu() const
{
	return d_ptr->_menu;
}

void ElaFloatButton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaFloatButton);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	eTheme->drawEffectShadow(&painter, rect(), 6, width() / 2);

	QRect foregroundRect = rect().adjusted(6, 6, -6, -6);
	int radius = foregroundRect.width() / 2;

	painter.setPen(Qt::NoPen);
	if (d->_isPressed)
	{
		painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryPress));
	}
	else if (d->_isAlphaAnimationFinished && underMouse())
	{
		painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryHover));
	}
	else
	{
		QColor baseColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
		if (d->_pHoverAlpha > 0)
		{
			QColor hoverColor = ElaThemeColor(d->_themeMode, PrimaryHover);
			qreal t = d->_pHoverAlpha / 255.0;
			baseColor = QColor(
				baseColor.red() + (hoverColor.red() - baseColor.red()) * t,
				baseColor.green() + (hoverColor.green() - baseColor.green()) * t,
				baseColor.blue() + (hoverColor.blue() - baseColor.blue()) * t);
		}
		painter.setBrush(baseColor);
	}
	painter.drawEllipse(foregroundRect);

	painter.setPen(Qt::white);
	QFont iconFont = QFont("ElaAwesome");
	iconFont.setPixelSize(22);
	painter.setFont(iconFont);
	painter.drawText(foregroundRect, Qt::AlignCenter, QChar((unsigned short) d->_icon));
}

void ElaFloatButton::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaFloatButton);
	if (event->button() == Qt::LeftButton)
	{
		d->_isPressed = true;
		update();
	}
	QWidget::mousePressEvent(event);
}

void ElaFloatButton::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaFloatButton);
	if (d->_isPressed && event->button() == Qt::LeftButton)
	{
		d->_isPressed = false;
		update();
		if (rect().contains(event->pos()))
		{
			if (d->_menu)
			{
				QPoint menuPos;
				if (d->_position == ElaFloatButton::TopLeft || d->_position == ElaFloatButton::TopRight)
				{
					menuPos = mapToGlobal(QPoint(0, height() + 5));
				}
				else
				{
					menuPos = mapToGlobal(QPoint(0, -d->_menu->sizeHint().height() - 5));
				}
				d->_menu->popup(menuPos);
			}
			Q_EMIT clicked();
		}
	}
	QWidget::mouseReleaseEvent(event);
}

bool ElaFloatButton::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaFloatButton);
	if (watched == parentWidget() && event->type() == QEvent::Resize)
	{
		d->updatePosition();
	}
	return QWidget::eventFilter(watched, event);
}

bool ElaFloatButton::event(QEvent *event)
{
	Q_D(ElaFloatButton);
	switch (event->type())
	{
		case QEvent::Enter:
		{
			d->_isAlphaAnimationFinished = false;
			QPropertyAnimation *alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
			connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
			connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() { d->_isAlphaAnimationFinished = true; });
			alphaAnimation->setDuration(200);
			alphaAnimation->setStartValue(d->_pHoverAlpha);
			alphaAnimation->setEndValue(255);
			alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			break;
		}
		case QEvent::Leave:
		{
			d->_isAlphaAnimationFinished = false;
			QPropertyAnimation *alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
			connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=]() { update(); });
			connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() { d->_isAlphaAnimationFinished = true; });
			alphaAnimation->setDuration(200);
			alphaAnimation->setStartValue(d->_pHoverAlpha);
			alphaAnimation->setEndValue(0);
			alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
			break;
		}
		default:
			break;
	}
	return QWidget::event(event);
}

ElaFloatButtonPrivate::ElaFloatButtonPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaFloatButtonPrivate::~ElaFloatButtonPrivate()
{
}

void ElaFloatButtonPrivate::updatePosition()
{
	Q_Q(ElaFloatButton);
	QWidget *parent = q->parentWidget();
	if (!parent)
	{
		return;
	}
	int x = 0;
	int y = 0;
	switch (_position)
	{
		case ElaFloatButton::BottomRight:
			x = parent->width() - q->width() - _pMargin;
			y = parent->height() - q->height() - _pMargin;
			break;
		case ElaFloatButton::BottomLeft:
			x = _pMargin;
			y = parent->height() - q->height() - _pMargin;
			break;
		case ElaFloatButton::TopRight:
			x = parent->width() - q->width() - _pMargin;
			y = _pMargin;
			break;
		case ElaFloatButton::TopLeft:
			x = _pMargin;
			y = _pMargin;
			break;
	}
	q->move(x, y);
	q->raise();
}