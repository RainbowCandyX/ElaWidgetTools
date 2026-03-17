#include "ElaCommandBar.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>

#include "ElaCommandBarPrivate.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

Q_PROPERTY_CREATE_Q_CPP(ElaCommandBar, int, ButtonSize)

ElaCommandBar::ElaCommandBar(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaCommandBarPrivate())
{
	Q_D(ElaCommandBar);
	d->q_ptr = this;
	d->_pButtonSize = 36;
	setFixedHeight(40);
	setObjectName("ElaCommandBar");
	setMouseTracking(true);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaCommandBar::~ElaCommandBar()
{
}

void ElaCommandBar::addItem(const CommandItem &item)
{
	Q_D(ElaCommandBar);
	d->_items.append(item);
	update();
}

void ElaCommandBar::addSeparator()
{
	CommandItem sep;
	sep.isSeparator = true;
	Q_D(ElaCommandBar);
	d->_items.append(sep);
	update();
}

void ElaCommandBar::clearItems()
{
	Q_D(ElaCommandBar);
	d->_items.clear();
	d->_hoveredIndex = -1;
	d->_pressedIndex = -1;
	update();
}

void ElaCommandBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaCommandBar);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QList<QRect> rects = d->_getVisibleItemRects();
	bool hasOverflow = (d->_visibleCount < d->_items.count());
	int totalVisible = rects.count();
	if (hasOverflow)
	{
		totalVisible -= 1; // last rect is the overflow button
	}

	for (int i = 0; i < totalVisible; ++i)
	{
		const ElaCommandBar::CommandItem &item = d->_items[i];
		QRect itemRect = rects[i];

		if (item.isSeparator)
		{
			painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
			int x = itemRect.center().x();
			int y1 = itemRect.top() + 8;
			int y2 = itemRect.bottom() - 8;
			painter.drawLine(x, y1, x, y2);
		}
		else
		{
			// Hover / Press background
			if (d->_pressedIndex == i)
			{
				painter.setPen(Qt::NoPen);
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicPress));
				painter.drawRoundedRect(itemRect, 4, 4);
			}
			else if (d->_hoveredIndex == i)
			{
				painter.setPen(Qt::NoPen);
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
				painter.drawRoundedRect(itemRect, 4, 4);
			}

			int xOffset = itemRect.x();

			// Draw icon
			if (item.icon != ElaIconType::None)
			{
				QFont iconFont = QFont("ElaAwesome");
				iconFont.setPixelSize(16);
				painter.setFont(iconFont);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
				if (item.text.isEmpty())
				{
					painter.drawText(itemRect, Qt::AlignCenter, QChar((unsigned short) item.icon));
				}
				else
				{
					QRect iconRect(xOffset + 6, itemRect.y(), 20, itemRect.height());
					painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) item.icon));
					xOffset += 26;
				}
			}

			// Draw text
			if (!item.text.isEmpty())
			{
				QFont textFont = this->font();
				textFont.setPixelSize(12);
				painter.setFont(textFont);
				painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
				int textX = (item.icon != ElaIconType::None) ? xOffset : itemRect.x() + 6;
				QRect textRect(textX, itemRect.y(), itemRect.right() - textX - 6, itemRect.height());
				painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, item.text);
			}
		}
	}

	// Draw overflow button
	if (hasOverflow && !rects.isEmpty())
	{
		QRect overflowRect = rects.last();
		int overflowIndex = -2; // special index for overflow
		if (d->_pressedIndex == overflowIndex)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicPress));
			painter.drawRoundedRect(overflowRect, 4, 4);
		}
		else if (d->_hoveredIndex == overflowIndex)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
			painter.drawRoundedRect(overflowRect, 4, 4);
		}
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(16);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		painter.drawText(overflowRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::Ellipsis));
	}

	painter.restore();
}

void ElaCommandBar::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaCommandBar);
	QList<QRect> rects = d->_getVisibleItemRects();
	bool hasOverflow = (d->_visibleCount < d->_items.count());
	int totalVisible = rects.count();

	int oldHovered = d->_hoveredIndex;
	d->_hoveredIndex = -1;

	if (hasOverflow && !rects.isEmpty())
	{
		QRect overflowRect = rects.last();
		if (overflowRect.contains(event->pos()))
		{
			d->_hoveredIndex = -2;
		}
		totalVisible -= 1;
	}

	if (d->_hoveredIndex == -1)
	{
		for (int i = 0; i < totalVisible; ++i)
		{
			if (!d->_items[i].isSeparator && rects[i].contains(event->pos()))
			{
				d->_hoveredIndex = i;
				break;
			}
		}
	}

	if (oldHovered != d->_hoveredIndex)
	{
		update();
	}
	QWidget::mouseMoveEvent(event);
}

void ElaCommandBar::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaCommandBar);
	if (event->button() == Qt::LeftButton)
	{
		d->_pressedIndex = d->_hoveredIndex;
		update();
	}
	QWidget::mousePressEvent(event);
}

void ElaCommandBar::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaCommandBar);
	if (event->button() == Qt::LeftButton)
	{
		if (d->_pressedIndex != -1 && d->_pressedIndex == d->_hoveredIndex)
		{
			if (d->_pressedIndex == -2)
			{
				// Overflow menu
				if (!d->_overflowMenu)
				{
					d->_overflowMenu = new ElaMenu(this);
				}
				d->_overflowMenu->clear();
				for (int i = d->_visibleCount; i < d->_items.count(); ++i)
				{
					const CommandItem &item = d->_items[i];
					if (item.isSeparator)
					{
						d->_overflowMenu->addSeparator();
					}
					else
					{
						QAction *action = nullptr;
						if (item.icon != ElaIconType::None)
						{
							action = d->_overflowMenu->addElaIconAction(item.icon, item.text);
						}
						else
						{
							action = d->_overflowMenu->addAction(item.text);
						}
						int index = i;
						connect(action, &QAction::triggered, this, [=]()
						{
							Q_EMIT itemClicked(index);
						});
					}
				}
				QList<QRect> rects = d->_getVisibleItemRects();
				if (!rects.isEmpty())
				{
					QRect overflowRect = rects.last();
					d->_overflowMenu->popup(mapToGlobal(overflowRect.bottomLeft()));
				}
			}
			else
			{
				Q_EMIT itemClicked(d->_pressedIndex);
			}
		}
		d->_pressedIndex = -1;
		update();
	}
	QWidget::mouseReleaseEvent(event);
}

void ElaCommandBar::leaveEvent(QEvent *event)
{
	Q_D(ElaCommandBar);
	d->_hoveredIndex = -1;
	update();
	QWidget::leaveEvent(event);
}

QSize ElaCommandBar::sizeHint() const
{
	Q_D(const ElaCommandBar);
	int totalWidth = 0;
	for (const auto &item: d->_items)
	{
		totalWidth += d->_getItemWidth(item);
	}
	return QSize(qMax(totalWidth, 50), 40);
}

ElaCommandBarPrivate::ElaCommandBarPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaCommandBarPrivate::~ElaCommandBarPrivate()
{
}

int ElaCommandBarPrivate::_getItemWidth(const ElaCommandBar::CommandItem &item) const
{
	if (item.isSeparator)
	{
		return 8;
	}
	if (item.icon != ElaIconType::None && item.text.isEmpty())
	{
		return _pButtonSize;
	}
	int w = 12; // padding
	if (item.icon != ElaIconType::None)
	{
		w += 20; // icon space
	}
	QFont textFont = q_ptr->font();
	textFont.setPixelSize(12);
	QFontMetrics fm(textFont);
	w += fm.horizontalAdvance(item.text);
	if (w < _pButtonSize)
	{
		w = _pButtonSize;
	}
	return w;
}

QList<QRect> ElaCommandBarPrivate::_getVisibleItemRects() const
{
	QList<QRect> rects;
	int availableWidth = q_ptr->width();
	int overflowWidth = _pButtonSize; // width for the "..." button
	int yOffset = (q_ptr->height() - _pButtonSize) / 2;
	int x = 0;

	// First pass: check if all items fit
	int totalWidth = 0;
	for (const auto &item: _items)
	{
		totalWidth += _getItemWidth(item);
	}

	bool needsOverflow = (totalWidth > availableWidth);
	int maxWidth = needsOverflow ? (availableWidth - overflowWidth) : availableWidth;

	int visibleCount = 0;
	for (int i = 0; i < _items.count(); ++i)
	{
		int itemWidth = _getItemWidth(_items[i]);
		if (x + itemWidth > maxWidth)
		{
			break;
		}
		int itemHeight = _items[i].isSeparator ? q_ptr->height() - 16 : _pButtonSize;
		int itemY = _items[i].isSeparator ? 8 : yOffset;
		rects.append(QRect(x, itemY, itemWidth, itemHeight));
		x += itemWidth;
		visibleCount++;
	}

	const_cast<ElaCommandBarPrivate *>(this)->_visibleCount = visibleCount;

	if (needsOverflow)
	{
		rects.append(QRect(x, yOffset, overflowWidth, _pButtonSize));
	}

	return rects;
}