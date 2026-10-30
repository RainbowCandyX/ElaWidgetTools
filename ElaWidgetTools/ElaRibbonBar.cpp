#include "ElaRibbonBar.h"

#include <QAction>
#include <QCursor>
#include <QEasingCurve>
#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QVariantAnimation>

#include "ElaMenu.h"
#include "ElaRibbonGroup.h"
#include "ElaRibbonTabBar.h"
#include "ElaTheme.h"
#include "private/ElaRibbonBarPrivate.h"

ElaRibbonBar::ElaRibbonBar(QWidget *parent) : QWidget{parent}, d_ptr(new ElaRibbonBarPrivate())
{
	Q_D(ElaRibbonBar);
	d->q_ptr = this;
	d->_pCurrentIndex = 0;
	d->_pBackgroundTransparent = false;

	setObjectName("ElaRibbonBar");
	setAttribute(Qt::WA_StyledBackground, false);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	d->_internalTabBar = new ElaRibbonTabBar(this);
	mainLayout->addWidget(d->_internalTabBar);

	d->_stack = new QStackedWidget(this);
	d->_stack->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(d->_stack, 1);

	connect(d->_internalTabBar, &ElaRibbonTabBar::pCurrentIndexChanged, this, [=]()
	{
		if (d->_externalTabBar)
		{
			return;
		}
		int idx = d->_internalTabBar->getCurrentIndex();
		if (idx != d->_pCurrentIndex)
		{
			d->_pCurrentIndex = idx;
			d->_stack->setCurrentIndex(idx);
			if (d->_isCollapsed)
			{
				setCollapsed(false);
			}
			Q_EMIT pCurrentIndexChanged();
		}
	});
	connect(d->_internalTabBar, &ElaRibbonTabBar::tabClicked, this, [=](int idx)
	{
		if (!d->_externalTabBar)
		{
			Q_EMIT tabClicked(idx);
		}
	});
	connect(d->_internalTabBar, &ElaRibbonTabBar::tabReclicked, this, [=](int)
	{
		if (!d->_externalTabBar)
		{
			setCollapsed(!d->_isCollapsed);
		}
	});

	setFixedHeight(d->_internalTabBar->height() + 88);

	d->_autoHideTimer = new QTimer(this);
	d->_autoHideTimer->setSingleShot(true);
	d->_autoHideTimer->setInterval(150);
	connect(d->_autoHideTimer, &QTimer::timeout, this, [=]()
	{
		if (d->_isPinned || d->_isCollapsed)
		{
			return;
		}
		const QPoint cursor = QCursor::pos();
		bool over = this->rect().contains(this->mapFromGlobal(cursor));
		if (!over && d->_internalTabBar && d->_internalTabBar->isVisible())
		{
			over = d->_internalTabBar->rect().contains(d->_internalTabBar->mapFromGlobal(cursor));
		}
		if (!over && d->_externalTabBar)
		{
			over = d->_externalTabBar->rect().contains(d->_externalTabBar->mapFromGlobal(cursor));
		}
		if (!over)
		{
			setCollapsed(true);
		}
	});

	this->installEventFilter(this);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, [=](const QPoint &pos)
	{
		showPinContextMenu(this->mapToGlobal(pos));
	});
	d->_internalTabBar->installEventFilter(this);
	d->_internalTabBar->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(d->_internalTabBar, &QWidget::customContextMenuRequested, this, [=](const QPoint &pos)
	{
		if (!d->_externalTabBar)
		{
			showPinContextMenu(d->_internalTabBar->mapToGlobal(pos));
		}
	});

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaRibbonBar::~ElaRibbonBar()
{
}

void ElaRibbonBar::bindTabBar(ElaRibbonTabBar *tabBar)
{
	Q_D(ElaRibbonBar);
	if (d->_externalTabBar.data() == tabBar)
	{
		return;
	}

	if (d->_externalTabBar)
	{
		disconnect(d->_externalTabBar.data(), nullptr, this, nullptr);
	}

	d->_externalTabBar = tabBar;

	if (tabBar)
	{
		d->_internalTabBar->hide();
		setFixedHeight(88);

		tabBar->clear();
		for (int i = 0; i < d->_internalTabBar->tabCount(); ++i)
		{
			tabBar->appendTab(d->_internalTabBar->tabText(i));
		}
		if (d->_pCurrentIndex >= 0 && d->_pCurrentIndex < tabBar->tabCount())
		{
			tabBar->setCurrentIndex(d->_pCurrentIndex);
		}

		connect(tabBar, &ElaRibbonTabBar::pCurrentIndexChanged, this, [=]()
		{
			if (!d->_externalTabBar)
			{
				return;
			}
			int idx = d->_externalTabBar->getCurrentIndex();
			if (idx != d->_pCurrentIndex)
			{
				d->_pCurrentIndex = idx;
				d->_stack->setCurrentIndex(idx);
				if (d->_isCollapsed)
				{
					setCollapsed(false);
				}
				Q_EMIT pCurrentIndexChanged();
			}
		});
		connect(tabBar, &ElaRibbonTabBar::tabClicked, this, [=](int idx)
		{
			Q_EMIT tabClicked(idx);
		});
		connect(tabBar, &ElaRibbonTabBar::tabReclicked, this, [=](int)
		{
			setCollapsed(!d->_isCollapsed);
		});

		tabBar->installEventFilter(this);
		tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(tabBar, &QWidget::customContextMenuRequested, this, [=](const QPoint &pos)
		{
			if (d->_externalTabBar)
			{
				showPinContextMenu(d->_externalTabBar->mapToGlobal(pos));
			}
		});
	}
	else
	{
		d->_internalTabBar->show();
		setFixedHeight(d->_internalTabBar->height() + 88);
	}
}

ElaRibbonTabBar *ElaRibbonBar::tabBar() const
{
	Q_D(const ElaRibbonBar);
	return d->activeTabBar();
}

QWidget *ElaRibbonBar::addTab(const QString &title)
{
	Q_D(ElaRibbonBar);
	QWidget *page = new QWidget(this);
	page->setAttribute(Qt::WA_TranslucentBackground);
	QHBoxLayout *pageLayout = new QHBoxLayout(page);
	pageLayout->setContentsMargins(6, 4, 6, 6);
	pageLayout->setSpacing(0);
	pageLayout->addStretch();
	d->_stack->addWidget(page);

	ElaRibbonTabBar *active = d->activeTabBar();
	if (active)
	{
		active->appendTab(title);
		if (d->_stack->count() == 1)
		{
			d->_pCurrentIndex = 0;
			d->_stack->setCurrentIndex(0);
			active->setCurrentIndex(0);
		}
	}
	return page;
}

ElaRibbonGroup *ElaRibbonBar::addGroup(QWidget *page, const QString &title)
{
	if (!page)
	{
		return nullptr;
	}
	QHBoxLayout *layout = qobject_cast<QHBoxLayout *>(page->layout());
	if (!layout)
	{
		return nullptr;
	}
	ElaRibbonGroup *group = new ElaRibbonGroup(title, page);
	layout->insertWidget(layout->count() - 1, group);
	return group;
}

int ElaRibbonBar::tabCount() const
{
	Q_D(const ElaRibbonBar);
	return d->_stack->count();
}

QString ElaRibbonBar::tabText(int index) const
{
	Q_D(const ElaRibbonBar);
	ElaRibbonTabBar *active = d->activeTabBar();
	return active ? active->tabText(index) : QString();
}

void ElaRibbonBar::setCurrentIndex(int currentIndex)
{
	Q_D(ElaRibbonBar);
	if (currentIndex < 0 || currentIndex >= d->_stack->count())
	{
		return;
	}
	if (d->_pCurrentIndex == currentIndex)
	{
		return;
	}
	d->_pCurrentIndex = currentIndex;
	d->_stack->setCurrentIndex(currentIndex);
	if (ElaRibbonTabBar *active = d->activeTabBar())
	{
		active->setCurrentIndex(currentIndex);
	}
	Q_EMIT pCurrentIndexChanged();
}

int ElaRibbonBar::getCurrentIndex() const
{
	Q_D(const ElaRibbonBar);
	return d->_pCurrentIndex;
}

void ElaRibbonBar::setCollapsed(bool collapsed)
{
	Q_D(ElaRibbonBar);
	if (d->_isCollapsed == collapsed)
	{
		return;
	}
	d->_isCollapsed = collapsed;

	const int normalH = d->_externalTabBar ? 88 : ((d->_internalTabBar ? d->_internalTabBar->height() : 0) + 88);
	const int collapsedH = d->_externalTabBar ? 0 : (d->_internalTabBar ? d->_internalTabBar->height() : 0);
	const int targetH = collapsed ? collapsedH : normalH;

	if (!d->_heightAnim)
	{
		d->_heightAnim = new QVariantAnimation(this);
		d->_heightAnim->setEasingCurve(QEasingCurve::OutCubic);
		connect(d->_heightAnim, &QVariantAnimation::valueChanged, this, [this](const QVariant &v)
		{
			const int h = v.toInt();
			this->setMinimumHeight(h);
			this->setMaximumHeight(h);
		});
		connect(d->_heightAnim, &QVariantAnimation::finished, this, [d]()
		{
			d->_stack->setVisible(!d->_isCollapsed);
		});
	}
	d->_heightAnim->stop();

	if (!collapsed)
	{
		d->_stack->setVisible(true);
	}
	d->_heightAnim->setStartValue(this->height());
	d->_heightAnim->setEndValue(targetH);
	d->_heightAnim->setDuration(d->_animationDuration);
	d->_heightAnim->start();

	Q_EMIT collapsedChanged(collapsed);
}

bool ElaRibbonBar::isCollapsed() const
{
	Q_D(const ElaRibbonBar);
	return d->_isCollapsed;
}

void ElaRibbonBar::setPinned(bool pinned)
{
	Q_D(ElaRibbonBar);
	if (d->_isPinned == pinned)
	{
		return;
	}
	d->_isPinned = pinned;
	if (pinned)
	{
		d->_autoHideTimer->stop();
	}
	Q_EMIT pinnedChanged(pinned);
}

bool ElaRibbonBar::isPinned() const
{
	Q_D(const ElaRibbonBar);
	return d->_isPinned;
}

void ElaRibbonBar::setAnimationDuration(int durationMs)
{
	Q_D(ElaRibbonBar);
	d->_animationDuration = qMax(0, durationMs);
}

int ElaRibbonBar::getAnimationDuration() const
{
	Q_D(const ElaRibbonBar);
	return d->_animationDuration;
}

void ElaRibbonBar::setBackgroundTransparent(bool backgroundTransparent)
{
	Q_D(ElaRibbonBar);
	if (d->_pBackgroundTransparent == backgroundTransparent)
	{
		return;
	}
	d->_pBackgroundTransparent = backgroundTransparent;
	setAttribute(Qt::WA_TranslucentBackground, backgroundTransparent);
	Q_EMIT pBackgroundTransparentChanged();
	update();
}

bool ElaRibbonBar::getBackgroundTransparent() const
{
	Q_D(const ElaRibbonBar);
	return d->_pBackgroundTransparent;
}

bool ElaRibbonBar::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaRibbonBar);
	if (watched == this || watched == d->_internalTabBar || watched == d->_externalTabBar.data())
	{
		if (event->type() == QEvent::Enter)
		{
			d->_autoHideTimer->stop();
		}
		else if (event->type() == QEvent::Leave)
		{
			if (!d->_isPinned && !d->_isCollapsed)
			{
				d->_autoHideTimer->start();
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}

void ElaRibbonBar::showPinContextMenu(const QPoint &globalPos)
{
	Q_D(ElaRibbonBar);
	ElaMenu menu(this);
	menu.setMenuItemHeight(27);
	QAction *pinAction = menu.addAction(d->_isPinned ? "取消固定" : "固定 Ribbon");
	pinAction->setCheckable(true);
	pinAction->setChecked(d->_isPinned);
	QAction *collapseAction = menu.addAction(d->_isCollapsed ? "展开 Ribbon" : "折叠 Ribbon");
	QAction *selected = menu.exec(globalPos);
	if (!selected)
	{
		return;
	}
	if (selected == pinAction)
	{
		setPinned(!d->_isPinned);
	}
	else if (selected == collapseAction)
	{
		setCollapsed(!d->_isCollapsed);
	}
}

void ElaRibbonBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaRibbonBar);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	if (!d->_pBackgroundTransparent)
	{
		painter.setBrush(ElaThemeColor(d->_themeMode, WindowBase));
		painter.drawRect(rect());
	}
	if (d->_externalTabBar)
	{
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
		painter.drawLine(0, 0, width(), 0);
	}
}

QSize ElaRibbonBar::sizeHint() const
{
	Q_D(const ElaRibbonBar);
	const int tabH = d->_internalTabBar ? d->_internalTabBar->height() : 0;
	int h;
	if (d->_isCollapsed)
	{
		h = d->_externalTabBar ? 0 : tabH;
	}
	else
	{
		h = d->_externalTabBar ? 88 : (tabH + 88);
	}
	return QSize(800, h);
}

ElaRibbonBarPrivate::ElaRibbonBarPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaRibbonBarPrivate::~ElaRibbonBarPrivate()
{
}

ElaRibbonTabBar *ElaRibbonBarPrivate::activeTabBar() const
{
	if (_externalTabBar)
	{
		return _externalTabBar.data();
	}
	return _internalTabBar;
}