#include "ElaMessageDialogPrivate.h"

#include <QPainter>
#include <QMouseEvent>
#include "ElaTheme.h"

ElaMessageDialogButton::ElaMessageDialogButton(ButtonType type, QWidget *parent)
	: QWidget(parent), _buttonType(type)
{
	_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		_themeMode = themeMode;
		update();
	});
	setCursor(Qt::PointingHandCursor);
}

ElaMessageDialogButton::~ElaMessageDialogButton()
{
}

void ElaMessageDialogButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	if (_isPressed)
	{
		painter.fillRect(rect(), QColor(0, 0, 0, 20));
	}
	else if (_isHovered)
	{
		painter.fillRect(rect(), QColor(0, 0, 0, 10));
	}

	int centerX = width() / 2;
	int centerY = height() / 2;

	painter.setPen(QPen(ElaThemeColor(_themeMode, BasicText), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	if (_buttonType == Confirm)
	{
		QPoint p1(centerX - 6, centerY);
		QPoint p2(centerX - 2, centerY + 4);
		QPoint p3(centerX + 6, centerY - 4);

		painter.drawLine(p1, p2);
		painter.drawLine(p2, p3);
	}
	else
	{
		int offset = 5;
		painter.drawLine(centerX - offset, centerY - offset, centerX + offset, centerY + offset);
		painter.drawLine(centerX - offset, centerY + offset, centerX + offset, centerY - offset);
	}
}

void ElaMessageDialogButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		_isPressed = true;
		update();
	}
	QWidget::mousePressEvent(event);
}

void ElaMessageDialogButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && _isPressed)
	{
		_isPressed = false;
		update();
		if (rect().contains(event->pos()))
		{
			Q_EMIT clicked();
		}
	}
	QWidget::mouseReleaseEvent(event);
}

void ElaMessageDialogButton::enterEvent(QEnterEvent *event)
{
	_isHovered = true;
	update();
	QWidget::enterEvent(event);
}

void ElaMessageDialogButton::leaveEvent(QEvent *event)
{
	_isHovered = false;
	_isPressed = false;
	update();
	QWidget::leaveEvent(event);
}

ElaMessageDialogPrivate::ElaMessageDialogPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaMessageDialogPrivate::~ElaMessageDialogPrivate()
{
}