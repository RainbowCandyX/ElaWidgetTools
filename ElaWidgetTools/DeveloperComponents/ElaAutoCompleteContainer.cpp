#include "ElaAutoCompleteContainer.h"

#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaLineEdit.h"
#include "ElaTheme.h"

ElaAutoCompleteContainer::ElaAutoCompleteContainer(QWidget *parent) : QWidget(parent)
{
	setContentsMargins(4, 4, 4, 4);
	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		_themeMode = themeMode;
		update();
	});
}

ElaAutoCompleteContainer::~ElaAutoCompleteContainer()
{
}

void ElaAutoCompleteContainer::setForwardTarget(ElaLineEdit *target)
{
	_forwardTarget = target;
}

void ElaAutoCompleteContainer::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);

	QPainterPath backgroundPath;
	backgroundPath.addRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), 6, 6);
	painter.fillPath(backgroundPath, ElaThemeColor(_themeMode, DialogBase));

	painter.setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1));
	painter.drawPath(backgroundPath);
}

void ElaAutoCompleteContainer::keyPressEvent(QKeyEvent *event)
{
	if (_forwardTarget)
	{
		if (event->key() == Qt::Key_Escape)
		{
			hide();
			_forwardTarget->setFocus();
			return;
		}
		if (event->key() != Qt::Key_Up && event->key() != Qt::Key_Down &&
		    event->key() != Qt::Key_Enter && event->key() != Qt::Key_Return)
		{
			QApplication::sendEvent(_forwardTarget, event);
			return;
		}
	}
	QWidget::keyPressEvent(event);
}

void ElaAutoCompleteContainer::hideEvent(QHideEvent *event)
{
	QWidget::hideEvent(event);
	Q_EMIT popupClosed();
}
