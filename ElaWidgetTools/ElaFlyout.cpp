#include "ElaFlyout.h"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>

#include "ElaFlyoutPrivate.h"
#include "ElaTheme.h"
Q_PROPERTY_CREATE_Q_CPP(ElaFlyout, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaFlyout, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaFlyout, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(ElaFlyout, bool, IsLightDismiss)

ElaFlyout::ElaFlyout(QWidget *parent)
	: QWidget{nullptr}, d_ptr(new ElaFlyoutPrivate())
{
	Q_D(ElaFlyout);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pIsLightDismiss = true;
	setObjectName("ElaFlyout");
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	d->_mainLayout = new QVBoxLayout(this);
	int margin = d->_shadowBorderWidth + 8;
	d->_mainLayout->setContentsMargins(margin, margin, margin, margin);

	d->_opacityEffect = new QGraphicsOpacityEffect(this);
	d->_opacityEffect->setOpacity(0);
	setGraphicsEffect(d->_opacityEffect);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	hide();
}

ElaFlyout::~ElaFlyout()
{
}

void ElaFlyout::setContentWidget(QWidget *widget)
{
	Q_D(ElaFlyout);
	if (!widget || widget == this)
	{
		return;
	}
	if (d->_contentWidget)
	{
		d->_mainLayout->removeWidget(d->_contentWidget);
		d->_contentWidget->deleteLater();
	}
	d->_contentWidget = widget;
	d->_mainLayout->addWidget(widget);
}

void ElaFlyout::showFlyout(QWidget *target)
{
	Q_D(ElaFlyout);
	if (!target)
	{
		return;
	}
	// Calculate size based on content
	if (!d->_contentWidget)
	{
		setFixedWidth(280);
		int contentHeight = d->_shadowBorderWidth * 2 + 12;
		int textWidth = 280 - d->_shadowBorderWidth * 2 - 30;
		if (!d->_pTitle.isEmpty())
		{
			QFont titleFont = font();
			titleFont.setPixelSize(14);
			titleFont.setBold(true);
			QFontMetrics tfm(titleFont);
			contentHeight += tfm.height() + 6;
		}
		if (!d->_pContent.isEmpty())
		{
			QFont contentFont = font();
			contentFont.setPixelSize(13);
			QFontMetrics cfm(contentFont);
			QRect br = cfm.boundingRect(QRect(0, 0, textWidth, 0), Qt::TextWordWrap, d->_pContent);
			contentHeight += br.height() + 8;
		}
		contentHeight += 12;
		setFixedHeight(contentHeight);
	}
	else
	{
		setMinimumWidth(0);
		setMaximumWidth(QWIDGETSIZE_MAX);
		setMinimumHeight(0);
		setMaximumHeight(QWIDGETSIZE_MAX);
		adjustSize();
	}
	QPoint targetGlobal = target->mapToGlobal(QPoint(0, target->height()));
	int x = targetGlobal.x() + (target->width() - width()) / 2;
	int y = targetGlobal.y() + 5;

	QScreen *screen = QApplication::screenAt(targetGlobal);
	if (screen)
	{
		QRect screenRect = screen->availableGeometry();
		if (x < screenRect.left())
		{
			x = screenRect.left();
		}
		if (x + width() > screenRect.right())
		{
			x = screenRect.right() - width();
		}
		if (y + height() > screenRect.bottom())
		{
			y = targetGlobal.y() - target->height() - height() - 5;
		}
		if (y < screenRect.top())
		{
			y = screenRect.top();
		}
	}
	move(x, y);
	show();
	raise();
	if (d->_pIsLightDismiss)
	{
		qApp->installEventFilter(d);
	}
	d->_doShowAnimation();
}

void ElaFlyout::closeFlyout()
{
	Q_D(ElaFlyout);
	qApp->removeEventFilter(d);
	hide();
	Q_EMIT closed();
}

void ElaFlyout::paintEvent(QPaintEvent *event)
{
	Q_D(ElaFlyout);
	QPainter painter(this);
	painter.save();
	painter.setRenderHint(QPainter::Antialiasing);
	// Shadow
	eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);
	QRect foregroundRect = rect();
	foregroundRect.adjust(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	if (!d->_contentWidget)
	{
		int textX = foregroundRect.x() + 12;
		int textY = foregroundRect.y() + 12;
		int textWidth = foregroundRect.width() - 24;

		// Title
		if (!d->_pTitle.isEmpty())
		{
			QFont titleFont = painter.font();
			titleFont.setPixelSize(14);
			titleFont.setBold(true);
			painter.setFont(titleFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
			QRect titleRect(textX, textY, textWidth, painter.fontMetrics().height());
			painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, d->_pTitle);
			textY += titleRect.height() + 6;
		}

		// Content
		if (!d->_pContent.isEmpty())
		{
			QFont contentFont = painter.font();
			contentFont.setPixelSize(13);
			contentFont.setBold(false);
			painter.setFont(contentFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
			QRect contentRect(textX, textY, textWidth, foregroundRect.bottom() - textY - 12);
			painter.drawText(contentRect, Qt::AlignLeft | Qt::TextWordWrap, d->_pContent);
		}
	}
	painter.restore();
}

// ElaFlyoutPrivate

ElaFlyoutPrivate::ElaFlyoutPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaFlyoutPrivate::~ElaFlyoutPrivate()
{
}

bool ElaFlyoutPrivate::eventFilter(QObject *watched, QEvent *event)
{
	Q_Q(ElaFlyout);
	if (event->type() == QEvent::MouseButtonPress)
	{
		QWidget *clickedWidget = qobject_cast<QWidget *>(watched);
		if (clickedWidget && !q->isAncestorOf(clickedWidget) && clickedWidget != q)
		{
			q->closeFlyout();
			return false;
		}
	}
	return QObject::eventFilter(watched, event);
}

void ElaFlyoutPrivate::_doShowAnimation()
{
	QPropertyAnimation *animation = new QPropertyAnimation(_opacityEffect, "opacity");
	animation->setDuration(250);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->setEasingCurve(QEasingCurve::InOutCubic);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}