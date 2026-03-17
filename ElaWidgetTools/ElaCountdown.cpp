#include "ElaCountdown.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaTheme.h"
#include "private/ElaCountdownPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, bool, IsShowDays)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, bool, IsShowHours)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, bool, IsShowMinutes)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, bool, IsShowSeconds)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, int, DigitWidth)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, int, DigitHeight)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, int, DigitSpacing)
Q_PROPERTY_CREATE_Q_CPP(ElaCountdown, int, FontPixelSize)

ElaCountdown::ElaCountdown(QWidget *parent) : QWidget(parent), d_ptr(new ElaCountdownPrivate())
{
	Q_D(ElaCountdown);
	d->q_ptr = this;
	d->_pBorderRadius = 6;
	d->_pIsShowDays = true;
	d->_pIsShowHours = true;
	d->_pIsShowMinutes = true;
	d->_pIsShowSeconds = true;
	d->_pDigitWidth = 44;
	d->_pDigitHeight = 56;
	d->_pDigitSpacing = 6;
	d->_pFontPixelSize = 24;
	d->_pFlipAngle = 0;
	d->_themeMode = eTheme->getThemeMode();

	d->_timer = new QTimer(this);
	d->_timer->setInterval(1000);
	connect(d->_timer, &QTimer::timeout, d, &ElaCountdownPrivate::onTimerTick);

	_updateSize();

	connect(this, &ElaCountdown::pIsShowDaysChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pIsShowHoursChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pIsShowMinutesChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pIsShowSecondsChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pDigitWidthChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pDigitHeightChanged, this, &ElaCountdown::_updateSize);
	connect(this, &ElaCountdown::pDigitSpacingChanged, this, &ElaCountdown::_updateSize);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaCountdown::~ElaCountdown()
{
}

void ElaCountdown::setTargetDateTime(const QDateTime &dateTime)
{
	Q_D(ElaCountdown);
	d->_targetDateTime = dateTime;
	d->_useTargetMode = true;
	qint64 secs = QDateTime::currentDateTime().secsTo(dateTime);
	d->_remainingSeconds = qMax(static_cast<qint64>(0), secs);
	update();
}

QDateTime ElaCountdown::getTargetDateTime() const
{
	return d_ptr->_targetDateTime;
}

void ElaCountdown::setRemainingSeconds(qint64 seconds)
{
	Q_D(ElaCountdown);
	d->_remainingSeconds = qMax(static_cast<qint64>(0), seconds);
	d->_useTargetMode = false;
	update();
}

qint64 ElaCountdown::getRemainingSeconds() const
{
	return d_ptr->_remainingSeconds;
}

void ElaCountdown::start()
{
	Q_D(ElaCountdown);
	if (d->_isRunning)
	{
		return;
	}
	d->_isRunning = true;
	if (d->_useTargetMode)
	{
		qint64 secs = QDateTime::currentDateTime().secsTo(d->_targetDateTime);
		d->_remainingSeconds = qMax(static_cast<qint64>(0), secs);
	}
	d->_prevSeconds = -1;
	d->_prevMinutes = -1;
	d->_prevHours = -1;
	d->_prevDays = -1;
	d->_timer->start();
	update();
}

void ElaCountdown::pause()
{
	Q_D(ElaCountdown);
	d->_isRunning = false;
	d->_timer->stop();
}

void ElaCountdown::resume()
{
	Q_D(ElaCountdown);
	if (!d->_isRunning)
	{
		d->_isRunning = true;
		d->_timer->start();
	}
}

void ElaCountdown::stop()
{
	Q_D(ElaCountdown);
	d->_isRunning = false;
	d->_timer->stop();
	d->_remainingSeconds = 0;
	update();
}

bool ElaCountdown::isRunning() const
{
	return d_ptr->_isRunning;
}

void ElaCountdown::paintEvent(QPaintEvent *event)
{
	Q_D(ElaCountdown);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	qint64 total = d->_remainingSeconds;
	int days = static_cast<int>(total / 86400);
	int hours = static_cast<int>((total % 86400) / 3600);
	int minutes = static_cast<int>((total % 3600) / 60);
	int seconds = static_cast<int>(total % 60);

	QFont digitFont = font();
	digitFont.setPixelSize(d->_pFontPixelSize);
	digitFont.setBold(true);

	QFont separatorFont = font();
	separatorFont.setPixelSize(d->_pFontPixelSize);
	separatorFont.setBold(true);

	QColor bgColor = ElaThemeColor(d->_themeMode, BasicBaseDeep);
	QColor textColor = ElaThemeColor(d->_themeMode, BasicText);
	QColor separatorColor = ElaThemeColor(d->_themeMode, BasicTextNoFocus);
	QColor lineColor = ElaThemeColor(d->_themeMode, BasicBorder);

	int dw = d->_pDigitWidth;
	int dh = d->_pDigitHeight;
	int spacing = d->_pDigitSpacing;
	int br = d->_pBorderRadius;

	auto drawDigitPair = [&](int x, int value)
	{
		QString text = QString("%1").arg(value, 2, 10, QChar('0'));

		QRectF topRect(x, 0, dw, dh / 2);
		QRectF bottomRect(x, dh / 2, dw, dh / 2);

		QPainterPath topPath;
		topPath.moveTo(topRect.bottomLeft());
		topPath.lineTo(topRect.x(), topRect.y() + br);
		topPath.quadTo(topRect.topLeft(), QPointF(topRect.x() + br, topRect.y()));
		topPath.lineTo(topRect.right() - br, topRect.y());
		topPath.quadTo(topRect.topRight(), QPointF(topRect.right(), topRect.y() + br));
		topPath.lineTo(topRect.bottomRight());
		topPath.closeSubpath();

		QPainterPath bottomPath;
		bottomPath.moveTo(bottomRect.topLeft());
		bottomPath.lineTo(bottomRect.x(), bottomRect.bottom() - br);
		bottomPath.quadTo(bottomRect.bottomLeft(), QPointF(bottomRect.x() + br, bottomRect.bottom()));
		bottomPath.lineTo(bottomRect.right() - br, bottomRect.bottom());
		bottomPath.quadTo(bottomRect.bottomRight(), QPointF(bottomRect.right(), bottomRect.bottom() - br));
		bottomPath.lineTo(bottomRect.topRight());
		bottomPath.closeSubpath();

		painter.setPen(Qt::NoPen);
		painter.setBrush(bgColor);
		painter.drawPath(topPath);
		painter.drawPath(bottomPath);

		painter.setPen(lineColor);
		painter.drawLine(QPointF(x + 1, dh / 2.0), QPointF(x + dw - 1, dh / 2.0));

		painter.setFont(digitFont);
		painter.setPen(textColor);
		QRectF fullRect(x, 0, dw, dh);
		painter.drawText(fullRect, Qt::AlignCenter, text);
	};

	int sepW = 20;
	auto drawSeparator = [&](int x)
	{
		painter.setFont(separatorFont);
		painter.setPen(separatorColor);
		painter.drawText(QRectF(x, 0, sepW, dh), Qt::AlignCenter, ":");
	};

	struct Section
	{
		int value;
		bool show;
	};
	QList<Section> sections;
	sections.append({days, d->_pIsShowDays});
	sections.append({hours, d->_pIsShowHours});
	sections.append({minutes, d->_pIsShowMinutes});
	sections.append({seconds, d->_pIsShowSeconds});

	int xPos = 0;
	bool first = true;
	for (const auto &sec: sections)
	{
		if (!sec.show) continue;
		if (!first)
		{
			drawSeparator(xPos);
			xPos += sepW;
		}
		drawDigitPair(xPos, sec.value);
		xPos += dw;
		first = false;
	}
}

void ElaCountdown::_updateSize()
{
	Q_D(ElaCountdown);
	int dw = d->_pDigitWidth;
	int dh = d->_pDigitHeight;
	int sepW = 20;
	int sectionCount = 0;
	if (d->_pIsShowDays) { sectionCount++; }
	if (d->_pIsShowHours) { sectionCount++; }
	if (d->_pIsShowMinutes) { sectionCount++; }
	if (d->_pIsShowSeconds) { sectionCount++; }
	int totalWidth = sectionCount * dw + qMax(0, sectionCount - 1) * sepW;
	setFixedSize(totalWidth, dh);
}