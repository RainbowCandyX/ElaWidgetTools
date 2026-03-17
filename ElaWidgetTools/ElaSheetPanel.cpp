#include "ElaSheetPanel.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "ElaMaskWidget.h"
#include "ElaTheme.h"
#include "private/ElaSheetPanelPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, ElaSheetPanelType::Direction, Direction)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, qreal, PeekRatio)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, qreal, HalfRatio)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, qreal, FullRatio)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, bool, DragHandleVisible)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, bool, CloseOnOverlayClick)
Q_PROPERTY_CREATE_Q_CPP(ElaSheetPanel, qreal, OverlayOpacity)

ElaSheetPanel::ElaSheetPanel(QWidget *parent) : QWidget(parent), d_ptr(new ElaSheetPanelPrivate())
{
	Q_D(ElaSheetPanel);
	d->q_ptr = this;
	d->_pBorderRadius = 12;
	d->_pDirection = ElaSheetPanelType::Bottom;
	d->_pPeekRatio = 0.15;
	d->_pHalfRatio = 0.50;
	d->_pFullRatio = 0.85;
	d->_pDragHandleVisible = true;
	d->_pCloseOnOverlayClick = true;
	d->_pOverlayOpacity = 0.4;
	d->_themeMode = eTheme->getThemeMode();

	setVisible(false);

	d->_maskWidget = new ElaMaskWidget(parent);
	d->_maskWidget->setVisible(false);

	d->_panelWidget = new QWidget(parent);
	d->_panelWidget->setVisible(false);
	d->_panelWidget->setMouseTracking(true);
	d->_panelWidget->installEventFilter(this);

	d->_dragHandle = new QWidget(d->_panelWidget);
	d->_dragHandle->setFixedSize(40, 4);
	d->_dragHandle->installEventFilter(this);
	d->_dragHandle->setCursor(Qt::SizeVerCursor);

	d->_panelLayout = new QVBoxLayout(d->_panelWidget);
	d->_panelLayout->setContentsMargins(0, 12, 0, 0);
	d->_panelLayout->setSpacing(0);
	d->_panelLayout->addWidget(d->_dragHandle, 0, Qt::AlignHCenter);
	d->_panelLayout->addSpacing(8);

	if (parent)
	{
		parent->installEventFilter(this);
		d->_maskWidget->setFixedSize(parent->size());
	}

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		d->_panelWidget->update();
	});
}

ElaSheetPanel::~ElaSheetPanel()
{
	Q_D(ElaSheetPanel);
	if (d->_maskWidget)
	{
		d->_maskWidget->deleteLater();
	}
	if (d->_panelWidget)
	{
		d->_panelWidget->deleteLater();
	}
}

void ElaSheetPanel::setCentralWidget(QWidget *widget)
{
	Q_D(ElaSheetPanel);
	if (d->_centralWidget)
	{
		d->_panelLayout->removeWidget(d->_centralWidget);
		d->_centralWidget->deleteLater();
	}
	d->_centralWidget = widget;
	if (widget)
	{
		d->_panelLayout->addWidget(widget);
	}
}

void ElaSheetPanel::open(ElaSheetPanelType::DetentLevel level)
{
	Q_D(ElaSheetPanel);
	if (d->_isOpened && d->_currentDetent == level)
	{
		return;
	}
	d->_isOpened = true;
	d->_currentDetent = level;

	if (parentWidget())
	{
		d->_maskWidget->setFixedSize(parentWidget()->size());
		d->_maskWidget->move(0, 0);
	}
	d->_maskWidget->setVisible(true);
	d->_maskWidget->raise();
	d->_maskWidget->doMaskAnimation(static_cast<int>(d->_pOverlayOpacity * 255));

	d->_panelWidget->setVisible(true);
	d->_panelWidget->raise();
	d->_doOpenAnimation(level);

	Q_EMIT opened();
	Q_EMIT detentChanged(level);
}

void ElaSheetPanel::close()
{
	Q_D(ElaSheetPanel);
	if (!d->_isOpened)
	{
		return;
	}
	d->_doCloseAnimation();
}

ElaSheetPanelType::DetentLevel ElaSheetPanel::currentDetent() const
{
	return d_ptr->_currentDetent;
}

bool ElaSheetPanel::isOpened() const
{
	return d_ptr->_isOpened;
}

bool ElaSheetPanel::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaSheetPanel);
	if (watched == parentWidget())
	{
		if (event->type() == QEvent::Resize)
		{
			if (d->_isOpened && parentWidget())
			{
				d->_maskWidget->setFixedSize(parentWidget()->size());
				int targetSize = d->_getTargetSize(d->_currentDetent);
				d->_updatePanelGeometry(targetSize, false);
			}
		}
	}
	else if (watched == d->_maskWidget)
	{
		if (event->type() == QEvent::MouseButtonPress && d->_pCloseOnOverlayClick)
		{
			close();
			return true;
		}
	}
	else if (watched == d->_dragHandle || watched == d->_panelWidget)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			if (mouseEvent->button() == Qt::LeftButton && watched == d->_dragHandle)
			{
				d->_isDragging = true;
				d->_dragStartPos = mouseEvent->globalPosition().toPoint();
				if (d->_pDirection == ElaSheetPanelType::Bottom)
				{
					d->_dragStartSize = d->_panelWidget->height();
				}
				else
				{
					d->_dragStartSize = d->_panelWidget->width();
				}
				return true;
			}
		}
		else if (event->type() == QEvent::MouseMove && d->_isDragging)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			QPoint delta = mouseEvent->globalPosition().toPoint() - d->_dragStartPos;
			int newSize = d->_dragStartSize;
			if (d->_pDirection == ElaSheetPanelType::Bottom)
			{
				newSize = d->_dragStartSize - delta.y();
			}
			else if (d->_pDirection == ElaSheetPanelType::Right)
			{
				newSize = d->_dragStartSize - delta.x();
			}
			else
			{
				newSize = d->_dragStartSize + delta.x();
			}
			newSize = qMax(20, newSize);
			d->_updatePanelGeometry(newSize, false);
			return true;
		}
		else if (event->type() == QEvent::MouseButtonRelease && d->_isDragging)
		{
			d->_isDragging = false;
			int currentSize;
			if (d->_pDirection == ElaSheetPanelType::Bottom)
			{
				currentSize = d->_panelWidget->height();
			}
			else
			{
				currentSize = d->_panelWidget->width();
			}
			int peekSize = d->_getTargetSize(ElaSheetPanelType::Peek);
			if (currentSize < peekSize / 2)
			{
				close();
			}
			else
			{
				ElaSheetPanelType::DetentLevel nearestLevel = d->_getNearestDetent(currentSize);
				d->_currentDetent = nearestLevel;
				d->_updatePanelGeometry(d->_getTargetSize(nearestLevel), true);
				Q_EMIT detentChanged(nearestLevel);
			}
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}