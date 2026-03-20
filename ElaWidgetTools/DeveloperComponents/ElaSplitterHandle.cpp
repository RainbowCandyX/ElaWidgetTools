#include "ElaSplitterHandle.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"

ElaSplitterHandle::ElaSplitterHandle(Qt::Orientation orientation, QSplitter* parent)
	: QSplitterHandle(orientation, parent)
{
	setMouseTracking(true);
	_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		_themeMode = themeMode;
		update();
	});
}

ElaSplitterHandle::~ElaSplitterHandle()
{
}

void ElaSplitterHandle::setGripLength(int length)
{
	_gripLength = length;
	update();
}

int ElaSplitterHandle::getGripLength() const
{
	return _gripLength;
}

void ElaSplitterHandle::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	QColor lineColor = ElaThemeColor(_themeMode, BasicBorder);
	if (orientation() == Qt::Horizontal)
	{
		int centerX = width() / 2;
		painter.setPen(QPen(lineColor, 1));
		painter.drawLine(centerX, 0, centerX, height());

		int gripWidth = 4;
		int gripY = (height() - _gripLength) / 2;
		QRectF gripRect(centerX - gripWidth / 2.0, gripY, gripWidth, _gripLength);

		QColor gripColor;
		if (_isPressed)
		{
			gripColor = ElaThemeColor(_themeMode, PrimaryNormal);
		}
		else if (_isHover)
		{
			gripColor = ElaThemeColor(_themeMode, BasicTextPress);
		}
		else
		{
			gripColor = ElaThemeColor(_themeMode, BasicBorderDeep);
		}
		painter.setPen(Qt::NoPen);
		painter.setBrush(gripColor);
		painter.drawRoundedRect(gripRect, gripWidth / 2.0, gripWidth / 2.0);
	}
	else
	{
		int centerY = height() / 2;
		painter.setPen(QPen(lineColor, 1));
		painter.drawLine(0, centerY, width(), centerY);

		int gripHeight = 4;
		int gripX = (width() - _gripLength) / 2;
		QRectF gripRect(gripX, centerY - gripHeight / 2.0, _gripLength, gripHeight);

		QColor gripColor;
		if (_isPressed)
		{
			gripColor = ElaThemeColor(_themeMode, PrimaryNormal);
		}
		else if (_isHover)
		{
			gripColor = ElaThemeColor(_themeMode, BasicTextPress);
		}
		else
		{
			gripColor = ElaThemeColor(_themeMode, BasicBorderDeep);
		}
		painter.setPen(Qt::NoPen);
		painter.setBrush(gripColor);
		painter.drawRoundedRect(gripRect, gripHeight / 2.0, gripHeight / 2.0);
	}
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void ElaMessageDialogButton::enterEvent(QEnterEvent* event)
#else
void ElaSplitterHandle::enterEvent(QEvent* event)
#endif
{
	Q_UNUSED(event);
	_isHover = true;
	update();
}

void ElaSplitterHandle::leaveEvent(QEvent* event)
{
	Q_UNUSED(event);
	_isHover = false;
	update();
}

void ElaSplitterHandle::mousePressEvent(QMouseEvent* event)
{
	_isPressed = true;
	update();
	QSplitterHandle::mousePressEvent(event);
}

void ElaSplitterHandle::mouseReleaseEvent(QMouseEvent* event)
{
	_isPressed = false;
	update();
	QSplitterHandle::mouseReleaseEvent(event);
}
