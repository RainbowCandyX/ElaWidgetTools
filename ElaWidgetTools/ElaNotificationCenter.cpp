#include "ElaNotificationCenter.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QWheelEvent>

#include "ElaTheme.h"
#include "private/ElaNotificationCenterPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaNotificationCenter, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaNotificationCenter, int, PanelWidth)

ElaNotificationCenter::ElaNotificationCenter(QWidget *parent)
	: QWidget{nullptr}, d_ptr(new ElaNotificationCenterPrivate())
{
	Q_D(ElaNotificationCenter);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pPanelWidth = 360;
	setObjectName("ElaNotificationCenter");
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedWidth(d->_pPanelWidth);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	hide();
}

ElaNotificationCenter::~ElaNotificationCenter()
{
}

void ElaNotificationCenter::addNotification(const NotificationItem &item)
{
	Q_D(ElaNotificationCenter);
	d->_items.prepend(item);
	if (d->_isVisible)
	{
		update();
	}
}

void ElaNotificationCenter::clearAll()
{
	Q_D(ElaNotificationCenter);
	d->_items.clear();
	d->_scrollOffset = 0;
	if (d->_isVisible)
	{
		update();
	}
}

int ElaNotificationCenter::getNotificationCount() const
{
	return d_ptr->_items.count();
}

void ElaNotificationCenter::showPanel(QWidget *anchor)
{
	Q_D(ElaNotificationCenter);
	if (!anchor || d->_isVisible)
	{
		return;
	}
	d->_anchorWidget = anchor;
	d->_scrollOffset = 0;
	setFixedWidth(d->_pPanelWidth);

	QPoint anchorGlobal = anchor->mapToGlobal(QPoint(0, 0));
	int anchorRight = anchorGlobal.x() + anchor->width();
	int anchorHeight = anchor->height();
	setFixedHeight(anchorHeight);

	// Start position: off-screen to the right
	int startX = anchorRight;
	int endX = anchorRight - d->_pPanelWidth;
	int posY = anchorGlobal.y();

	move(startX, posY);
	show();
	raise();

	if (d->_slideAnimation)
	{
		d->_slideAnimation->stop();
		delete d->_slideAnimation;
	}
	d->_slideAnimation = new QPropertyAnimation(this, "pos");
	d->_slideAnimation->setDuration(250);
	d->_slideAnimation->setEasingCurve(QEasingCurve::OutCubic);
	d->_slideAnimation->setStartValue(QPoint(startX, posY));
	d->_slideAnimation->setEndValue(QPoint(endX, posY));
	d->_slideAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	connect(d->_slideAnimation, &QPropertyAnimation::finished, this, [d]()
	{
		d->_slideAnimation = nullptr;
	});

	d->_isVisible = true;
	qApp->installEventFilter(d);
	Q_EMIT panelVisibilityChanged(true);
}

void ElaNotificationCenter::hidePanel()
{
	Q_D(ElaNotificationCenter);
	if (!d->_isVisible)
	{
		return;
	}
	qApp->removeEventFilter(d);
	d->_isVisible = false;

	if (d->_anchorWidget)
	{
		QPoint anchorGlobal = d->_anchorWidget->mapToGlobal(QPoint(0, 0));
		int anchorRight = anchorGlobal.x() + d->_anchorWidget->width();
		int posY = anchorGlobal.y();

		if (d->_slideAnimation)
		{
			d->_slideAnimation->stop();
			delete d->_slideAnimation;
		}
		d->_slideAnimation = new QPropertyAnimation(this, "pos");
		d->_slideAnimation->setDuration(250);
		d->_slideAnimation->setEasingCurve(QEasingCurve::OutCubic);
		d->_slideAnimation->setStartValue(pos());
		d->_slideAnimation->setEndValue(QPoint(anchorRight, posY));
		connect(d->_slideAnimation, &QPropertyAnimation::finished, this, [this, d]()
		{
			hide();
			d->_slideAnimation = nullptr;
		});
		d->_slideAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	else
	{
		hide();
	}
	Q_EMIT panelVisibilityChanged(false);
}

bool ElaNotificationCenter::isPanelVisible() const
{
	return d_ptr->_isVisible;
}

void ElaNotificationCenter::paintEvent(QPaintEvent *event)
{
	Q_D(ElaNotificationCenter);
	Q_UNUSED(event);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	// Shadow on left edge
	eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

	// Background
	QRect foregroundRect = rect();
	foregroundRect.adjust(d->_shadowBorderWidth, 0, 0, 0);
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	int contentLeft = foregroundRect.x() + 12;
	int contentRight = foregroundRect.right() - 12;
	int contentWidth = contentRight - contentLeft;

	// Header
	int headerY = foregroundRect.y() + 12;
	QFont headerFont = painter.font();
	headerFont.setPixelSize(16);
	headerFont.setBold(true);
	painter.setFont(headerFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	QRect headerTitleRect(contentLeft, headerY, contentWidth / 2, 24);
	painter.drawText(headerTitleRect, Qt::AlignLeft | Qt::AlignVCenter, QString::fromUtf8("\xe9\x80\x9a\xe7\x9f\xa5\xe4\xb8\xad\xe5\xbf\x83"));

	// "清除全部" button
	QFont clearFont = painter.font();
	clearFont.setPixelSize(13);
	clearFont.setBold(false);
	painter.setFont(clearFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
	QRect clearButtonRect(contentRight - 80, headerY, 80, 24);
	painter.drawText(clearButtonRect, Qt::AlignRight | Qt::AlignVCenter, QString::fromUtf8("\xe6\xb8\x85\xe9\x99\xa4\xe5\x85\xa8\xe9\x83\xa8"));

	// Separator line
	int separatorY = headerY + 30;
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.drawLine(contentLeft, separatorY, contentRight, separatorY);

	int cardAreaTop = separatorY + 8;
	int cardAreaBottom = foregroundRect.bottom() - 8;

	if (d->_items.isEmpty())
	{
		// No notifications
		QFont emptyFont = painter.font();
		emptyFont.setPixelSize(14);
		emptyFont.setBold(false);
		painter.setFont(emptyFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		QRect emptyRect(contentLeft, cardAreaTop, contentWidth, cardAreaBottom - cardAreaTop);
		painter.drawText(emptyRect, Qt::AlignCenter, QString::fromUtf8("\xe6\x9a\x82\xe6\x97\xa0\xe9\x80\x9a\xe7\x9f\xa5"));
	}
	else
	{
		// Clip to card area
		painter.setClipRect(QRect(foregroundRect.x(), cardAreaTop, foregroundRect.width(), cardAreaBottom - cardAreaTop));

		int cardHeight = 70;
		int cardMargin = 8;
		int currentY = cardAreaTop - d->_scrollOffset;

		for (int i = 0; i < d->_items.count(); ++i)
		{
			const NotificationItem &item = d->_items[i];
			QRect cardRect(contentLeft, currentY, contentWidth, cardHeight);

			if (cardRect.bottom() >= cardAreaTop && cardRect.top() <= cardAreaBottom)
			{
				// Card background
				painter.setPen(Qt::NoPen);
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
				QPainterPath cardPath;
				cardPath.addRoundedRect(cardRect, d->_pBorderRadius, d->_pBorderRadius);
				painter.drawPath(cardPath);

				int iconAreaWidth = 30;
				int textLeft = cardRect.x() + 8;

				// Icon
				if (item.icon != ElaIconType::None)
				{
					QFont iconFont = QFont("ElaAwesome");
					iconFont.setPixelSize(18);
					painter.setFont(iconFont);
					painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
					QRect iconRect(textLeft, cardRect.y() + 8, iconAreaWidth, 22);
					painter.drawText(iconRect, Qt::AlignVCenter | Qt::AlignLeft, QChar((unsigned short) item.icon));
					textLeft += iconAreaWidth + 4;
				}
				else
				{
					textLeft += 4;
				}

				int textWidth = cardRect.right() - textLeft - 8;

				// Title
				QFont titleFont = this->font();
				titleFont.setPixelSize(14);
				titleFont.setBold(true);
				painter.setFont(titleFont);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
				QRect titleRect(textLeft, cardRect.y() + 6, textWidth, 20);
				painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter,
				                 painter.fontMetrics().elidedText(item.title, Qt::ElideRight, textWidth));

				// Content
				QFont contentFont = this->font();
				contentFont.setPixelSize(13);
				painter.setFont(contentFont);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
				QRect contentRect(textLeft, cardRect.y() + 26, textWidth, 20);
				painter.drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter,
				                 painter.fontMetrics().elidedText(item.content, Qt::ElideRight, textWidth));

				// Timestamp
				QFont tsFont = this->font();
				tsFont.setPixelSize(12);
				painter.setFont(tsFont);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
				QRect tsRect(textLeft, cardRect.y() + 48, textWidth, 16);
				painter.drawText(tsRect, Qt::AlignRight | Qt::AlignVCenter, item.timestamp);
			}
			currentY += cardHeight + cardMargin;
		}
		painter.setClipping(false);
	}
	painter.restore();
}

void ElaNotificationCenter::wheelEvent(QWheelEvent *event)
{
	Q_D(ElaNotificationCenter);
	if (d->_items.isEmpty())
	{
		return;
	}
	int cardHeight = 70;
	int cardMargin = 8;
	int totalHeight = d->_items.count() * (cardHeight + cardMargin) - cardMargin;

	// Header area height
	int headerHeight = 50;
	int visibleHeight = height() - headerHeight - 16;

	int maxScroll = qMax(0, totalHeight - visibleHeight);
	d->_scrollOffset -= event->angleDelta().y() / 3;
	d->_scrollOffset = qBound(0, d->_scrollOffset, maxScroll);
	update();
	event->accept();
}

void ElaNotificationCenter::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaNotificationCenter);
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mousePressEvent(event);
		return;
	}

	QPoint pos = event->pos();

	// Check clear all button
	if (d->_hitTestClearAll(pos))
	{
		clearAll();
		return;
	}

	// Check notification cards
	int index = d->_hitTestCard(pos);
	if (index >= 0)
	{
		Q_EMIT notificationClicked(index);
		return;
	}
	QWidget::mousePressEvent(event);
}

// ElaNotificationCenterPrivate

ElaNotificationCenterPrivate::ElaNotificationCenterPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaNotificationCenterPrivate::~ElaNotificationCenterPrivate()
{
}

bool ElaNotificationCenterPrivate::eventFilter(QObject *watched, QEvent *event)
{
	Q_Q(ElaNotificationCenter);
	if (event->type() == QEvent::MouseButtonPress)
	{
		QWidget *clickedWidget = qobject_cast<QWidget *>(watched);
		if (clickedWidget && !q->isAncestorOf(clickedWidget) && clickedWidget != q)
		{
			q->hidePanel();
			return false;
		}
	}
	return QObject::eventFilter(watched, event);
}

int ElaNotificationCenterPrivate::_hitTestCard(const QPoint &pos) const
{
	Q_Q(const ElaNotificationCenter);
	int foregroundLeft = _shadowBorderWidth;
	int contentLeft = foregroundLeft + 12;
	int contentRight = q->width() - 12;
	int contentWidth = contentRight - contentLeft;

	int headerY = 12;
	int separatorY = headerY + 30;
	int cardAreaTop = separatorY + 8;

	int cardHeight = 70;
	int cardMargin = 8;

	for (int i = 0; i < _items.count(); ++i)
	{
		int currentY = cardAreaTop - _scrollOffset + i * (cardHeight + cardMargin);
		QRect cardRect(contentLeft, currentY, contentWidth, cardHeight);
		if (cardRect.contains(pos) && currentY + cardHeight > cardAreaTop)
		{
			return i;
		}
	}
	return -1;
}

QRect ElaNotificationCenterPrivate::_clearAllButtonRect() const
{
	Q_Q(const ElaNotificationCenter);
	int foregroundLeft = _shadowBorderWidth;
	int contentRight = q->width() - 12;
	int headerY = 12;
	return QRect(contentRight - 80, headerY, 80, 24);
}

bool ElaNotificationCenterPrivate::_hitTestClearAll(const QPoint &pos) const
{
	return _clearAllButtonRect().contains(pos);
}