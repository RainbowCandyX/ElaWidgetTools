#include "ElaCopyButton.h"

#include <QApplication>
#include <QClipboard>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaTheme.h"
#include "private/ElaCopyButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaCopyButton, int, BorderRadius)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaCopyButton, QString, CopyText)
Q_PROPERTY_CREATE_Q_CPP(ElaCopyButton, int, SuccessDuration)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaCopyButton, QString, SuccessText)

ElaCopyButton::ElaCopyButton(QWidget *parent) : QPushButton(parent), d_ptr(new ElaCopyButtonPrivate())
{
	Q_D(ElaCopyButton);
	d->q_ptr = this;
	d->_pBorderRadius = 3;
	d->_pSuccessDuration = 1500;
	d->_themeMode = eTheme->getThemeMode();
	setMouseTracking(true);
	setFixedSize(30, 30);
	QFont font = this->font();
	font.setPixelSize(15);
	setFont(font);
	setObjectName("ElaCopyButton");
	setStyleSheet("#ElaCopyButton{background-color:transparent;}");

	d->_resetTimer = new QTimer(this);
	d->_resetTimer->setSingleShot(true);
	connect(d->_resetTimer, &QTimer::timeout, this, [=]()
	{
		d->_isSuccess = false;
		QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "");
		connect(fadeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
		{
			d->_iconOpacity = value.toReal();
			update();
		});
		fadeAnimation->setStartValue(0.0);
		fadeAnimation->setEndValue(1.0);
		fadeAnimation->setDuration(200);
		fadeAnimation->setEasingCurve(QEasingCurve::InOutSine);
		fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	});

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
	});
}

ElaCopyButton::ElaCopyButton(const QString &text, QWidget *parent)
	: ElaCopyButton(parent)
{
	d_ptr->_pCopyText = text;
}

ElaCopyButton::~ElaCopyButton()
{
}

void ElaCopyButton::setText(const QString &text)
{
	QPushButton::setText(text);
	if (text.isEmpty())
	{
		setFixedSize(30, 30);
	}
	else
	{
		Q_D(ElaCopyButton);
		QFont iconFont("ElaAwesome");
		iconFont.setPixelSize(17);
		QFontMetrics iconFm(iconFont);
		int iconWidth = iconFm.horizontalAdvance(QChar((unsigned short) d->_normalIcon));
		QFontMetrics textFm(font());
		int textWidth = qMax(textFm.horizontalAdvance(text),
		                     textFm.horizontalAdvance(d->_pSuccessText));
		int padding = 2 * d->_shadowBorderWidth + 6 + 16;
		setFixedSize(iconWidth + textWidth + padding, 38);
	}
}

void ElaCopyButton::setElaIcon(ElaIconType::IconName icon)
{
	Q_D(ElaCopyButton);
	d->_normalIcon = icon;
	update();
}

void ElaCopyButton::setSuccessIcon(ElaIconType::IconName icon)
{
	Q_D(ElaCopyButton);
	d->_successIcon = icon;
	update();
}

void ElaCopyButton::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaCopyButton);
	d->_isPressed = true;
	QPushButton::mousePressEvent(event);
}

void ElaCopyButton::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaCopyButton);
	d->_isPressed = false;
	if (rect().contains(event->pos()) && !d->_isSuccess)
	{
		QApplication::clipboard()->setText(d->_pCopyText);
		d->_isSuccess = true;
		d->_resetTimer->start(d->_pSuccessDuration);

		QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "");
		connect(fadeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
		{
			d->_iconOpacity = value.toReal();
			update();
		});
		fadeAnimation->setStartValue(0.0);
		fadeAnimation->setEndValue(1.0);
		fadeAnimation->setDuration(200);
		fadeAnimation->setEasingCurve(QEasingCurve::InOutSine);
		fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

		Q_EMIT copyCompleted(d->_pCopyText);
	}
	QPushButton::mouseReleaseEvent(event);
}

void ElaCopyButton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaCopyButton);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

	painter.save();
	QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth,
	                     width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);

	if (d->_themeMode == ElaThemeType::Light)
	{
		painter.setPen(ElaThemeColor(ElaThemeType::Light, BasicBorder));
		if (d->_isSuccess)
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
		}
		else if (d->_isPressed)
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Light, BasicPress));
		}
		else if (underMouse())
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Light, BasicHover));
		}
		else
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Light, BasicBase));
		}
	}
	else
	{
		painter.setPen(Qt::NoPen);
		if (d->_isSuccess)
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
		}
		else if (d->_isPressed)
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Dark, BasicPress));
		}
		else if (underMouse())
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Dark, BasicHover));
		}
		else
		{
			painter.setBrush(ElaThemeColor(ElaThemeType::Dark, BasicBase));
		}
	}
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	if (!d->_isPressed)
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
		painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth,
		                 foregroundRect.width(), height() - d->_shadowBorderWidth);
	}

	ElaIconType::IconName currentIcon = d->_isSuccess ? d->_successIcon : d->_normalIcon;
	QColor iconColor = d->_isSuccess
		                   ? ElaThemeColor(d->_themeMode, PrimaryNormal)
		                   : (d->_themeMode == ElaThemeType::Light
			                      ? ElaThemeColor(ElaThemeType::Light, BasicText)
			                      : ElaThemeColor(ElaThemeType::Dark, BasicText));
	iconColor.setAlphaF(d->_iconOpacity);
	painter.setPen(iconColor);

	QFont iconFont = QFont("ElaAwesome");
	iconFont.setPixelSize(17);

	QString btnText = d->_isSuccess ? d->_pSuccessText : text();
	if (btnText.isEmpty())
	{
		painter.setFont(iconFont);
		painter.drawText(foregroundRect, Qt::AlignCenter, QChar((unsigned short) currentIcon));
	}
	else
	{
		QFontMetrics iconFm(iconFont);
		int iconWidth = iconFm.horizontalAdvance(QChar((unsigned short) currentIcon));

		QFont textFont = this->font();
		QColor textColor = d->_isSuccess
			                   ? ElaThemeColor(d->_themeMode, PrimaryNormal)
			                   : (d->_themeMode == ElaThemeType::Light
				                      ? ElaThemeColor(ElaThemeType::Light, BasicText)
				                      : ElaThemeColor(ElaThemeType::Dark, BasicText));
		textColor.setAlphaF(d->_iconOpacity);
		QFontMetrics textFm(textFont);
		int textWidth = textFm.horizontalAdvance(btnText);

		int spacing = 6;
		int totalWidth = iconWidth + spacing + textWidth;
		int startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;

		painter.setFont(iconFont);
		painter.drawText(QRect(startX, foregroundRect.y(), iconWidth, foregroundRect.height()),
		                 Qt::AlignCenter, QChar((unsigned short) currentIcon));

		painter.setPen(textColor);
		painter.setFont(textFont);
		painter.drawText(QRect(startX + iconWidth + spacing, foregroundRect.y(), textWidth, foregroundRect.height()),
		                 Qt::AlignCenter, btnText);
	}

	painter.restore();
}