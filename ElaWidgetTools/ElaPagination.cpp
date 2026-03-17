#include "ElaPagination.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QIntValidator>

#include "ElaLineEdit.h"
#include "ElaTheme.h"
#include "private/ElaPaginationPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaPagination, int, CurrentPage)
Q_PROPERTY_CREATE_Q_CPP(ElaPagination, int, TotalPages)
Q_PROPERTY_CREATE_Q_CPP(ElaPagination, int, ButtonSize)
Q_PROPERTY_CREATE_Q_CPP(ElaPagination, int, PagerCount)
Q_PROPERTY_CREATE_Q_CPP(ElaPagination, bool, JumperVisible)

ElaPaginationPrivate::ElaPaginationPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaPaginationPrivate::~ElaPaginationPrivate()
{
}

void ElaPaginationPrivate::_updateJumperPosition()
{
	Q_Q(ElaPagination);
	if (!_jumperEdit)
	{
		return;
	}
	_jumperEdit->setVisible(_pJumperVisible);
	if (!_pJumperVisible)
	{
		return;
	}
	QList<QPair<QRect, int>> buttonRects = _getButtonRects();
	int jumperX = 0;
	if (!buttonRects.isEmpty())
	{
		QRect lastRect = buttonRects.last().first;
		jumperX = lastRect.x() + lastRect.width() + 12;
	}
	int editWidth = 55;
	int editHeight = _pButtonSize;
	int y = (q->height() - editHeight) / 2;
	_jumperEdit->setGeometry(jumperX, y, editWidth, editHeight);
}

QList<int> ElaPaginationPrivate::_getVisiblePages() const
{
	QList<int> pages;
	int total = _pTotalPages;
	int current = _pCurrentPage;
	int pagerCount = _pPagerCount;

	if (total <= 0)
	{
		return pages;
	}

	if (total <= pagerCount)
	{
		for (int i = 1; i <= total; i++)
		{
			pages.append(i);
		}
		return pages;
	}

	int halfPager = (pagerCount - 2) / 2;
	bool showLeftEllipsis = current > halfPager + 2;
	bool showRightEllipsis = current < total - halfPager - 1;

	if (!showLeftEllipsis && showRightEllipsis)
	{
		for (int i = 1; i <= pagerCount - 2; i++)
		{
			pages.append(i);
		}
		pages.append(-1);
		pages.append(total);
	}
	else if (showLeftEllipsis && !showRightEllipsis)
	{
		pages.append(1);
		pages.append(-3);
		for (int i = total - (pagerCount - 3); i <= total; i++)
		{
			pages.append(i);
		}
	}
	else
	{
		pages.append(1);
		pages.append(-3);
		int sideCount = (pagerCount - 4) / 2;
		for (int i = current - sideCount; i <= current + sideCount; i++)
		{
			pages.append(i);
		}
		pages.append(-1);
		pages.append(total);
	}

	return pages;
}

QList<QPair<QRect, int> > ElaPaginationPrivate::_getButtonRects() const
{
	QList<QPair<QRect, int> > rects;
	int size = _pButtonSize;
	int spacing = 4;
	int x = 0;
	int y = 4;

	rects.append(qMakePair(QRect(x, y, size, size), 0));
	x += size + spacing;

	QList<int> pages = _getVisiblePages();
	for (int page: pages)
	{
		if (page == -1)
		{
			rects.append(qMakePair(QRect(x, y, size, size), -1));
		}
		else if (page == -3)
		{
			rects.append(qMakePair(QRect(x, y, size, size), -3));
		}
		else
		{
			rects.append(qMakePair(QRect(x, y, size, size), page));
		}
		x += size + spacing;
	}

	rects.append(qMakePair(QRect(x, y, size, size), -2));

	return rects;
}

ElaPagination::ElaPagination(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaPaginationPrivate())
{
	Q_D(ElaPagination);
	d->q_ptr = this;
	d->_pCurrentPage = 1;
	d->_pTotalPages = 1;
	d->_pButtonSize = 28;
	d->_pPagerCount = 11;
	d->_pJumperVisible = false;
	setObjectName("ElaPagination");
	setMouseTracking(true);
	setFixedHeight(d->_pButtonSize + 8);
	d->_themeMode = eTheme->getThemeMode();

	d->_jumperEdit = new ElaLineEdit(this);
	d->_jumperEdit->setFixedHeight(d->_pButtonSize);
	d->_jumperEdit->setPlaceholderText("页码");
	d->_jumperEdit->setAlignment(Qt::AlignCenter);
	d->_jumperEdit->setStyleSheet("#ElaLineEdit{background-color:transparent;padding-left:0px;padding-right:0px;}");
	d->_jumperEdit->setValidator(new QIntValidator(1, 9999, d->_jumperEdit));
	d->_jumperEdit->setVisible(false);
	connect(d->_jumperEdit, &ElaLineEdit::returnPressed, this, [=]()
	{
		QString text = d->_jumperEdit->text().trimmed();
		bool ok = false;
		int page = text.toInt(&ok);
		if (ok && page >= 1 && page <= d->_pTotalPages && page != d->_pCurrentPage)
		{
			d->_pCurrentPage = page;
			Q_EMIT pCurrentPageChanged();
			Q_EMIT currentPageChanged(page);
			update();
		}
		d->_jumperEdit->clear();
	});

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaPagination::~ElaPagination()
{
}

void ElaPagination::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaPagination);
	d->_updateJumperPosition();
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QList<QPair<QRect, int> > buttonRects = d->_getButtonRects();
	QFont textFont = this->font();
	textFont.setPixelSize(14);

	QFont iconFont("ElaAwesome");
	iconFont.setPixelSize(16);

	for (int i = 0; i < buttonRects.size(); i++)
	{
		QRect rect = buttonRects[i].first;
		int pageValue = buttonRects[i].second;

		bool isHovered = (i == d->_hoverIndex);
		bool isCurrentPage = (pageValue > 0 && pageValue == d->_pCurrentPage);
		bool isDisabled = false;

		if (pageValue == 0 && d->_pCurrentPage <= 1)
		{
			isDisabled = true;
		}
		if (pageValue == -2 && d->_pCurrentPage >= d->_pTotalPages)
		{
			isDisabled = true;
		}

		painter.setPen(Qt::NoPen);
		if (isCurrentPage)
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
		}
		else if (isDisabled)
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicDisable));
		}
		else if (isHovered)
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
		}
		else
		{
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
		}

		QPainterPath path;
		path.addRoundedRect(rect, 6, 6);
		painter.drawPath(path);

		if (isCurrentPage)
		{
			painter.setPen(Qt::white);
		}
		else if (isDisabled)
		{
			painter.setPen(ElaThemeColor(d->_themeMode, BasicTextDisable));
		}
		else
		{
			painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		}

		if (pageValue == 0)
		{
			painter.setFont(iconFont);
			painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xea84));
		}
		else if (pageValue == -2)
		{
			painter.setFont(iconFont);
			painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xea85));
		}
		else if (pageValue == -1 || pageValue == -3)
		{
			painter.setFont(iconFont);
			painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xec4d));
		}
		else
		{
			painter.setFont(textFont);
			painter.drawText(rect, Qt::AlignCenter, QString::number(pageValue));
		}
	}
}

void ElaPagination::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaPagination);
	QList<QPair<QRect, int> > buttonRects = d->_getButtonRects();
	int oldHover = d->_hoverIndex;
	d->_hoverIndex = -1;
	for (int i = 0; i < buttonRects.size(); i++)
	{
		if (buttonRects[i].first.contains(event->pos()))
		{
			d->_hoverIndex = i;
			break;
		}
	}
	if (oldHover != d->_hoverIndex)
	{
		update();
	}
	QWidget::mouseMoveEvent(event);
}

void ElaPagination::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaPagination);
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mousePressEvent(event);
		return;
	}

	QList<QPair<QRect, int> > buttonRects = d->_getButtonRects();
	for (int i = 0; i < buttonRects.size(); i++)
	{
		if (buttonRects[i].first.contains(event->pos()))
		{
			int pageValue = buttonRects[i].second;
			int newPage = d->_pCurrentPage;

			if (pageValue == 0)
			{
				if (d->_pCurrentPage > 1)
				{
					newPage = d->_pCurrentPage - 1;
				}
			}
			else if (pageValue == -2)
			{
				if (d->_pCurrentPage < d->_pTotalPages)
				{
					newPage = d->_pCurrentPage + 1;
				}
			}
			else if (pageValue == -1)
			{
				newPage = qMin(d->_pCurrentPage + d->_pPagerCount - 2, d->_pTotalPages);
			}
			else if (pageValue == -3)
			{
				newPage = qMax(d->_pCurrentPage - (d->_pPagerCount - 2), 1);
			}
			else if (pageValue > 0)
			{
				newPage = pageValue;
			}

			if (newPage != d->_pCurrentPage)
			{
				d->_pCurrentPage = newPage;
				Q_EMIT pCurrentPageChanged();
				Q_EMIT currentPageChanged(newPage);
				update();
			}
			break;
		}
	}
	QWidget::mousePressEvent(event);
}

QSize ElaPagination::sizeHint() const
{
	Q_D(const ElaPagination);
	QList<QPair<QRect, int> > buttonRects = d->_getButtonRects();
	int totalWidth = 0;
	if (!buttonRects.isEmpty())
	{
		QRect lastRect = buttonRects.last().first;
		totalWidth = lastRect.x() + lastRect.width();
	}
	if (d->_pJumperVisible)
	{
		totalWidth += 12 + 55;
	}
	return QSize(totalWidth, d->_pButtonSize + 8);
}
