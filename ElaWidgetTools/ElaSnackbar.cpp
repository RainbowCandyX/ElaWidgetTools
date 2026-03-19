#include "ElaSnackbar.h"

#include <QGraphicsOpacityEffect>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>

#include "ElaTheme.h"
#include "private/ElaSnackbarPrivate.h"

static int s_maxSnackbars = 5;
static const int s_spacing = 10;
static QList<ElaSnackbar *> s_activeSnackbars;
static int s_baseBottomY = 0;
static int s_centerX = 0;

static int s_calcTargetY(int index)
{
	int offset = 0;
	for (int i = s_activeSnackbars.size() - 1; i > index; --i)
	{
		offset += s_activeSnackbars[i]->height() + s_spacing;
	}
	return s_baseBottomY - s_activeSnackbars[index]->height() - offset;
}

void ElaSnackbar::_reflowAll()
{
	for (int i = 0; i < s_activeSnackbars.size(); ++i)
	{
		ElaSnackbar *snackbar = s_activeSnackbars[i];
		int targetY = s_calcTargetY(i);
		if (snackbar->y() != targetY)
		{
			QPropertyAnimation *moveAnim = new QPropertyAnimation(snackbar, "pos", snackbar);
			moveAnim->setDuration(200);
			moveAnim->setStartValue(snackbar->pos());
			moveAnim->setEndValue(QPoint(s_centerX, targetY));
			moveAnim->setEasingCurve(QEasingCurve::OutCubic);
			moveAnim->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}
}

Q_PROPERTY_CREATE_Q_CPP(ElaSnackbar, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSnackbar, int, DisplayMsec)

ElaSnackbar::ElaSnackbar(SnackbarType type, const QString &text, const QString &actionText, int displayMsec, QWidget *parent)
	: QWidget{nullptr}, d_ptr(new ElaSnackbarPrivate())
{
	Q_D(ElaSnackbar);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pDisplayMsec = displayMsec;
	d->_snackbarType = type;
	d->_text = text;
	d->_actionText = actionText;

	setObjectName("ElaSnackbar");
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);
	setMouseTracking(true);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});

	QFont textFont = font();
	textFont.setPixelSize(14);
	QFontMetrics fm(textFont);
	int textWidth = fm.horizontalAdvance(text);
	int actionWidth = 0;
	if (!actionText.isEmpty())
	{
		QFont actionFont = textFont;
		actionFont.setBold(true);
		QFontMetrics afm(actionFont);
		actionWidth = afm.horizontalAdvance(actionText) + 24;
	}
	int totalWidth = textWidth + 80 + actionWidth;
	totalWidth = qBound(280, totalWidth, 560);
	setFixedHeight(48 + d->_shadowBorderWidth * 2);
	setFixedWidth(totalWidth + d->_shadowBorderWidth * 2);

	while (s_activeSnackbars.size() >= s_maxSnackbars)
	{
		s_activeSnackbars.first()->dismiss();
	}

	if (parent)
	{
		QRect parentRect = parent->geometry();
		s_centerX = parentRect.x() + (parentRect.width() - width()) / 2;
		s_baseBottomY = parentRect.y() + parentRect.height() - 30;
	}
	else
	{
		QScreen *screen = QGuiApplication::primaryScreen();
		if (screen)
		{
			QRect screenRect = screen->availableGeometry();
			s_centerX = screenRect.x() + (screenRect.width() - width()) / 2;
			s_baseBottomY = screenRect.y() + screenRect.height() - 30;
		}
	}

	s_activeSnackbars.append(this);
	int targetY = s_calcTargetY(s_activeSnackbars.size() - 1);

	for (int i = 0; i < s_activeSnackbars.size() - 1; ++i)
	{
		ElaSnackbar *snackbar = s_activeSnackbars[i];
		int newY = s_calcTargetY(i);
		if (snackbar->y() != newY)
		{
			QPropertyAnimation *moveAnim = new QPropertyAnimation(snackbar, "pos", snackbar);
			moveAnim->setDuration(200);
			moveAnim->setStartValue(snackbar->pos());
			moveAnim->setEndValue(QPoint(s_centerX, newY));
			moveAnim->setEasingCurve(QEasingCurve::OutCubic);
			moveAnim->start(QAbstractAnimation::DeleteWhenStopped);
		}
	}

	move(s_centerX, targetY + 20);
	show();

	QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
	opacityEffect->setOpacity(0);
	setGraphicsEffect(opacityEffect);

	QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity", this);
	fadeIn->setDuration(250);
	fadeIn->setStartValue(0.0);
	fadeIn->setEndValue(1.0);
	fadeIn->setEasingCurve(QEasingCurve::OutCubic);

	QPropertyAnimation *slideIn = new QPropertyAnimation(this, "pos", this);
	slideIn->setDuration(250);
	slideIn->setStartValue(QPoint(s_centerX, targetY + 20));
	slideIn->setEndValue(QPoint(s_centerX, targetY));
	slideIn->setEasingCurve(QEasingCurve::OutCubic);

	d->_doClose = [=]()
	{
		if (d->_isClosing)
		{
			return;
		}
		d->_isClosing = true;
		s_activeSnackbars.removeOne(this);
		_reflowAll();

		QPropertyAnimation *fadeOut = new QPropertyAnimation(opacityEffect, "opacity", this);
		fadeOut->setDuration(300);
		fadeOut->setStartValue(opacityEffect->opacity());
		fadeOut->setEndValue(0.0);
		fadeOut->setEasingCurve(QEasingCurve::InCubic);
		connect(fadeOut, &QPropertyAnimation::finished, this, [=]()
		{
			Q_EMIT closed();
			close();
			deleteLater();
		});
		fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
	};

	connect(fadeIn, &QPropertyAnimation::finished, this, [=]()
	{
		QTimer::singleShot(displayMsec, this, d->_doClose);
	});

	fadeIn->start();
	slideIn->start();
}

ElaSnackbar::~ElaSnackbar()
{
}

void ElaSnackbar::setMaxCount(int count)
{
	s_maxSnackbars = qBound(1, count, 12);
}

int ElaSnackbar::getMaxCount()
{
	return s_maxSnackbars;
}

void ElaSnackbar::dismiss()
{
	Q_D(ElaSnackbar);
	if (d->_doClose)
	{
		d->_doClose();
	}
}

ElaSnackbar *ElaSnackbar::success(const QString &text, const QString &actionText, int displayMsec, QWidget *parent)
{
	return new ElaSnackbar(Success, text, actionText, displayMsec, parent);
}

ElaSnackbar *ElaSnackbar::info(const QString &text, const QString &actionText, int displayMsec, QWidget *parent)
{
	return new ElaSnackbar(Info, text, actionText, displayMsec, parent);
}

ElaSnackbar *ElaSnackbar::warning(const QString &text, const QString &actionText, int displayMsec, QWidget *parent)
{
	return new ElaSnackbar(Warning, text, actionText, displayMsec, parent);
}

ElaSnackbar *ElaSnackbar::error(const QString &text, const QString &actionText, int displayMsec, QWidget *parent)
{
	return new ElaSnackbar(Error, text, actionText, displayMsec, parent);
}

void ElaSnackbar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaSnackbar);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

	QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth,
	                     width() - 2 * d->_shadowBorderWidth, height() - 2 * d->_shadowBorderWidth);

	painter.save();
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
	painter.restore();

	QColor indicatorColor;
	ElaIconType::IconName iconEnum;
	QColor iconColor;
	switch (d->_snackbarType)
	{
		case Success:
			indicatorColor = QColor(0x0F, 0x7B, 0x0F);
			iconEnum = ElaIconType::Check;
			iconColor = QColor(0x0F, 0x7B, 0x0F);
			break;
		case Info:
			indicatorColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
			iconEnum = ElaIconType::CircleInfo;
			iconColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
			break;
		case Warning:
			indicatorColor = QColor(0xF7, 0x93, 0x0E);
			iconEnum = ElaIconType::CircleExclamation;
			iconColor = QColor(0xF7, 0x93, 0x0E);
			break;
		case Error:
			indicatorColor = ElaThemeColor(d->_themeMode, StatusDanger);
			iconEnum = ElaIconType::CircleXmark;
			iconColor = ElaThemeColor(d->_themeMode, StatusDanger);
			break;
	}

	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setBrush(indicatorColor);
	QPainterPath clipPath;
	clipPath.addRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
	painter.setClipPath(clipPath);
	painter.drawRect(QRect(foregroundRect.x(), foregroundRect.y(), 4, foregroundRect.height()));
	painter.restore();

	painter.save();
	painter.setPen(iconColor);
	QFont iconFont = QFont("ElaAwesome");
	iconFont.setPixelSize(16);
	painter.setFont(iconFont);
	QRect iconRect(foregroundRect.x() + 14, foregroundRect.y(), 20, foregroundRect.height());
	painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) iconEnum));
	painter.restore();

	int textLeft = foregroundRect.x() + 42;
	int textRight = foregroundRect.right() - 10;

	if (!d->_actionText.isEmpty())
	{
		QFont actionFont = font();
		actionFont.setPixelSize(14);
		actionFont.setBold(true);
		QFontMetrics afm(actionFont);
		int actionWidth = afm.horizontalAdvance(d->_actionText) + 16;
		int actionHeight = 28;
		int actionX = foregroundRect.right() - actionWidth - 8;
		int actionY = foregroundRect.y() + (foregroundRect.height() - actionHeight) / 2;
		d->_actionRect = QRect(actionX, actionY, actionWidth, actionHeight);
		textRight = actionX - 8;

		if (d->_isActionPressed)
		{
			painter.save();
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicPress));
			painter.drawRoundedRect(d->_actionRect, 4, 4);
			painter.restore();
		}
		else if (d->_isActionHover)
		{
			painter.save();
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
			painter.drawRoundedRect(d->_actionRect, 4, 4);
			painter.restore();
		}

		painter.save();
		painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
		painter.setFont(actionFont);
		painter.drawText(d->_actionRect, Qt::AlignCenter, d->_actionText);
		painter.restore();
	}

	painter.save();
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	QFont textFont = font();
	textFont.setPixelSize(14);
	painter.setFont(textFont);
	QRect textRect(textLeft, foregroundRect.y(), textRight - textLeft, foregroundRect.height());
	painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, painter.fontMetrics().elidedText(d->_text, Qt::ElideRight, textRect.width()));
	painter.restore();
}

void ElaSnackbar::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaSnackbar);
	if (!d->_actionText.isEmpty() && d->_actionRect.contains(event->pos()))
	{
		d->_isActionPressed = true;
		update();
	}
	QWidget::mousePressEvent(event);
}

void ElaSnackbar::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaSnackbar);
	if (d->_isActionPressed && d->_actionRect.contains(event->pos()))
	{
		Q_EMIT actionClicked();
	}
	d->_isActionPressed = false;
	update();
	QWidget::mouseReleaseEvent(event);
}

void ElaSnackbar::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaSnackbar);
	bool hover = !d->_actionText.isEmpty() && d->_actionRect.contains(event->pos());
	if (hover != d->_isActionHover)
	{
		d->_isActionHover = hover;
		setCursor(hover ? Qt::PointingHandCursor : Qt::ArrowCursor);
		update();
	}
	QWidget::mouseMoveEvent(event);
}

ElaSnackbarPrivate::ElaSnackbarPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaSnackbarPrivate::~ElaSnackbarPrivate()
{
}