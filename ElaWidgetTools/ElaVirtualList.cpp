#include "ElaVirtualList.h"

#include <QPainter>
#include <QPalette>
#include <QStandardItemModel>

#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "private/ElaVirtualListPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaVirtualList, int, ItemHeight)
Q_PROPERTY_CREATE_Q_CPP(ElaVirtualList, bool, IsTransparent)
Q_PROPERTY_CREATE_Q_CPP(ElaVirtualList, bool, IsAlternatingRowColors)

ElaVirtualList::ElaVirtualList(QWidget *parent) : QListView(parent), d_ptr(new ElaVirtualListPrivate())
{
	Q_D(ElaVirtualList);
	d->q_ptr = this;
	d->_pItemHeight = 40;
	d->_pIsTransparent = true;
	d->_pIsAlternatingRowColors = false;
	d->_themeMode = eTheme->getThemeMode();
	setObjectName("ElaVirtualList");
	setStyleSheet("#ElaVirtualList{background-color:transparent;}");
	setMouseTracking(true);

	QPalette p = palette();
	p.setColor(QPalette::Text, ElaThemeColor(d->_themeMode, BasicText));
	setPalette(p);

	ElaScrollBar *vScrollBar = new ElaScrollBar(this);
	vScrollBar->setIsAnimation(true);
	setVerticalScrollBar(vScrollBar);
	ElaScrollBar *hScrollBar = new ElaScrollBar(this);
	setHorizontalScrollBar(hScrollBar);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	setUniformItemSizes(true);

	setLayoutMode(QListView::Batched);
	setBatchSize(100);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		QPalette p = palette();
		p.setColor(QPalette::Text, ElaThemeColor(themeMode, BasicText));
		setPalette(p);
		viewport()->update();
	});
}

ElaVirtualList::~ElaVirtualList()
{
}

void ElaVirtualList::setItemCount(int count)
{
	Q_D(ElaVirtualList);
	d->_itemCount = count;
}

int ElaVirtualList::getItemCount() const
{
	return d_ptr->_itemCount;
}

void ElaVirtualList::paintEvent(QPaintEvent *event)
{
	Q_D(ElaVirtualList);
	if (!d->_pIsTransparent)
	{
		QPainter painter(viewport());
		painter.save();
		painter.setRenderHints(QPainter::Antialiasing);
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
		painter.drawRect(rect());
		painter.restore();
	}
	QListView::paintEvent(event);
}