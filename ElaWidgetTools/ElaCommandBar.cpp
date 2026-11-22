#include "ElaCommandBar.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>

#include "ElaCommandBarPrivate.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

Q_PROPERTY_CREATE_Q_CPP(ElaCommandBar, int, ButtonSize)

void ElaCommandBar::setItemSpacing(int spacing)
{
	Q_D(ElaCommandBar);
	if (d->_pItemSpacing == spacing)
	{
		return;
	}
	d->_pItemSpacing = spacing;
	Q_EMIT pItemSpacingChanged();
	updateGeometry();
	update();
}

int ElaCommandBar::getItemSpacing() const
{
	return d_ptr->_pItemSpacing;
}

ElaCommandBar::ElaCommandBar(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaCommandBarPrivate())
{
	Q_D(ElaCommandBar);
	d->q_ptr = this;
	d->_pButtonSize = 36;
	d->_pItemSpacing = 0;
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

void ElaCommandBar::setItemCheckable(int index, bool checkable)
{
	Q_D(ElaCommandBar);
	if (index < 0 || index >= d->_items.count() || d->_items[index].isSeparator)
	{
		return;
	}
	if (d->_items[index].isCheckable == checkable)
	{
		return;
	}
	d->_items[index].isCheckable = checkable;
	update();
}

void ElaCommandBar::setItemChecked(int index, bool checked)
{
	Q_D(ElaCommandBar);
	if (index < 0 || index >= d->_items.count() || d->_items[index].isSeparator)
	{
		return;
	}
	if (d->_items[index].isChecked == checked)
	{
		return;
	}
	d->_items[index].isChecked = checked;
	Q_EMIT itemCheckedChanged(index, checked);
	update();
}

bool ElaCommandBar::isItemCheckable(int index) const
{
	Q_D(const ElaCommandBar);
	if (index < 0 || index >= d->_items.count())
	{
		return false;
	}
	return d->_items[index].isCheckable;
}

bool ElaCommandBar::isItemChecked(int index) const
{
	Q_D(const ElaCommandBar);
	if (index < 0 || index >= d->_items.count())
	{
		return false;
	}
	return d->_items[index].isChecked;
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
			const bool isCheckedItem = item.isCheckable && item.isChecked;
			// 背景优先级：按下 > 悬停 > 勾选
			QColor bgColor;
			bool hasBackground = false;
			if (d->_pressedIndex == i)
			{
				bgColor = ElaThemeColor(d->_themeMode, BasicPressAlpha);
				hasBackground = true;
			}
			else if (d->_hoveredIndex == i)
			{
				bgColor = ElaThemeColor(d->_themeMode, BasicHoverAlpha);
				hasBackground = true;
			}
			else if (isCheckedItem)
			{
				bgColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
				bgColor.setAlpha(30);
				hasBackground = true;
			}
			if (hasBackground)
			{
				painter.setPen(Qt::NoPen);
				painter.setBrush(bgColor);
				painter.drawRoundedRect(itemRect, 4, 4);
			}

			const QColor fgColor = isCheckedItem ? ElaThemeColor(d->_themeMode, PrimaryNormal) : ElaThemeColor(d->_themeMode, BasicText);

			int xOffset = itemRect.x();

			// Draw icon
			if (item.icon != ElaIconType::None)
			{
				QFont iconFont = QFont("ElaAwesome");
				iconFont.setPixelSize(16);
				painter.setFont(iconFont);
				painter.setPen(fgColor);
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
				painter.setPen(fgColor);
				int textX = (item.icon != ElaIconType::None) ? xOffset : itemRect.x() + 6;
				QRect textRect(textX, itemRect.y(), itemRect.x() + itemRect.width() - textX - 6, itemRect.height());
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
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicPressAlpha));
			painter.drawRoundedRect(overflowRect, 4, 4);
		}
		else if (d->_hoveredIndex == overflowIndex)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicHoverAlpha));
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
						// 勾选项在溢出菜单里用对勾图标代替原图标
						ElaIconType::IconName actionIcon = (item.isCheckable && item.isChecked) ? ElaIconType::Check : item.icon;
						QAction *action = nullptr;
						if (actionIcon != ElaIconType::None)
						{
							action = d->_overflowMenu->addElaIconAction(actionIcon, item.text);
						}
						else
						{
							action = d->_overflowMenu->addAction(item.text);
						}
						int index = i;
						connect(action, &QAction::triggered, this, [=]()
						{
							if (index >= 0 && index < d->_items.count() && d->_items[index].isCheckable)
							{
								d->_items[index].isChecked = !d->_items[index].isChecked;
								Q_EMIT itemCheckedChanged(index, d->_items[index].isChecked);
								update();
							}
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
				const int index = d->_pressedIndex;
				if (index >= 0 && index < d->_items.count() && d->_items[index].isCheckable)
				{
					d->_items[index].isChecked = !d->_items[index].isChecked;
					Q_EMIT itemCheckedChanged(index, d->_items[index].isChecked);
					update();
				}
				Q_EMIT itemClicked(index);
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
	for (int i = 0; i < d->_items.count(); ++i)
	{
		totalWidth += d->_getItemWidth(d->_items[i]);
		if (i != d->_items.count() - 1)
		{
			totalWidth += d->_pItemSpacing;
		}
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
	w += qMax(fm.horizontalAdvance(item.text), fm.boundingRect(item.text).width());
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
	for (int i = 0; i < _items.count(); ++i)
	{
		totalWidth += _getItemWidth(_items[i]);
		if (i != _items.count() - 1)
		{
			totalWidth += _pItemSpacing;
		}
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
		x += itemWidth + _pItemSpacing;
		visibleCount++;
	}

	const_cast<ElaCommandBarPrivate *>(this)->_visibleCount = visibleCount;

	if (needsOverflow)
	{
		rects.append(QRect(x, yOffset, overflowWidth, _pButtonSize));
	}

	return rects;
}