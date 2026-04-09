#include "ElaTreeSelect.h"

#include <QApplication>
#include <QMouseEvent>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaTheme.h"
#include "ElaTreeView.h"
#include "private/ElaTreeSelectPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaTreeSelect, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaTreeSelect, int, MaxVisibleItems)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaTreeSelect, QString, PlaceholderText)
Q_PROPERTY_CREATE_Q_CPP(ElaTreeSelect, bool, IsSearchVisible)
Q_PROPERTY_CREATE_Q_CPP(ElaTreeSelect, bool, IsEditable)

ElaTreeSelect::ElaTreeSelect(QWidget *parent) : QWidget(parent), d_ptr(new ElaTreeSelectPrivate())
{
	Q_D(ElaTreeSelect);
	d->q_ptr = this;
	d->_pBorderRadius = 3;
	d->_pItemHeight = 35;
	d->_pMaxVisibleItems = 8;
	d->_pPlaceholderText = "";
	d->_pIsSearchVisible = true;
	d->_pIsEditable = false;
	d->_themeMode = eTheme->getThemeMode();
	setObjectName("ElaTreeSelect");
	setFixedHeight(35);
	setMinimumWidth(180);
	setMouseTracking(true);
	setCursor(Qt::PointingHandCursor);

	d->_popupContainer = new ElaTreeSelectPopupWidget(nullptr);
	d->_popupContainer->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	d->_popupContainer->setAttribute(Qt::WA_TranslucentBackground);
	d->_popupContainer->_themeMode = d->_themeMode;
	d->_popupContainer->setObjectName("ElaTreeSelectPopup");
	d->_popupContainer->hide();

	QVBoxLayout *popupLayout = new QVBoxLayout(d->_popupContainer);
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
	popupLayout->setContentsMargins(4, 4, 4, 4);
#else
	popupLayout->setContentsMargins(6, 6, 6, 6);
#endif
	popupLayout->setSpacing(4);

	d->_searchEdit = new ElaLineEdit(d->_popupContainer);
	d->_searchEdit->setPlaceholderText("搜索...");
	d->_searchEdit->setFixedHeight(30);
	d->_searchEdit->setIsClearButtonEnable(true);
	popupLayout->addWidget(d->_searchEdit);

	d->_treeView = new ElaTreeView(d->_popupContainer);
	d->_treeView->setHeaderHidden(true);
	d->_treeView->setRootIsDecorated(true);
	d->_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
	d->_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	d->_treeView->setObjectName("ElaTreeSelectView");
	d->_treeView->setStyleSheet("#ElaTreeSelectView{background-color:transparent;}");
	popupLayout->addWidget(d->_treeView);

	d->_proxyModel = new QSortFilterProxyModel(this);
	d->_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	d->_proxyModel->setRecursiveFilteringEnabled(true);

	connect(d->_treeView, &QTreeView::clicked, d, &ElaTreeSelectPrivate::onItemClicked);
	connect(d->_treeView, &QTreeView::expanded, d, &ElaTreeSelectPrivate::onTreeExpanded);
	connect(d->_treeView, &QTreeView::collapsed, d, &ElaTreeSelectPrivate::onTreeCollapsed);
	connect(d->_searchEdit, &ElaLineEdit::textChanged, d, &ElaTreeSelectPrivate::onSearchTextChanged);
	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaTreeSelectPrivate::onThemeChanged);
}

ElaTreeSelect::~ElaTreeSelect()
{
	d_ptr->_popupContainer->deleteLater();
}

void ElaTreeSelect::setItemHeight(int itemHeight)
{
	Q_D(ElaTreeSelect);
	if (itemHeight > 0)
	{
		d->_pItemHeight = itemHeight;
		d->_treeView->setItemHeight(itemHeight);
	}
}

int ElaTreeSelect::getItemHeight() const
{
	return d_ptr->_pItemHeight;
}

void ElaTreeSelect::setModel(QStandardItemModel *model)
{
	Q_D(ElaTreeSelect);
	d->_model = model;
	if (model)
	{
		d->_proxyModel->setSourceModel(model);
		d->_treeView->setModel(d->_proxyModel);
	}
}

QStandardItemModel *ElaTreeSelect::model() const
{
	return d_ptr->_model;
}

void ElaTreeSelect::setCurrentIndex(const QModelIndex &index)
{
	Q_D(ElaTreeSelect);
	d->_currentIndex = index;
	d->_updateDisplayText();
	if (d->_proxyModel && index.isValid())
	{
		QModelIndex proxyIndex = d->_proxyModel->mapFromSource(index);
		d->_treeView->setCurrentIndex(proxyIndex);
	}
	update();
}

QModelIndex ElaTreeSelect::currentIndex() const
{
	return d_ptr->_currentIndex;
}

QString ElaTreeSelect::currentText() const
{
	return d_ptr->_currentText;
}

void ElaTreeSelect::expandAll()
{
	Q_D(ElaTreeSelect);
	d->_treeView->expandAll();
}

void ElaTreeSelect::collapseAll()
{
	Q_D(ElaTreeSelect);
	d->_treeView->collapseAll();
}

void ElaTreeSelect::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaTreeSelect);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QRectF bgRect(0, 0, width(), height());
	QPainterPath bgPath;
	bgPath.addRoundedRect(bgRect, d->_pBorderRadius, d->_pBorderRadius);

	if (d->_isPressed)
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicBorderDeep));
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicPress));
	}
	else if (d->_isHover)
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicBorderHover));
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
	}
	else
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
	}
	painter.drawPath(bgPath);

	if (d->_isPopupVisible)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
		QRectF indicatorRect(d->_pBorderRadius + 6, height() - 3,
		                     width() - 2 * (d->_pBorderRadius + 6), 3);
		painter.drawRoundedRect(indicatorRect, 1.5, 1.5);
	}

	QRectF textRect(10, 0, width() - 35, height());
	if (d->_currentText.isEmpty())
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, d->_pPlaceholderText);
	}
	else
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QString elidedText = painter.fontMetrics().elidedText(d->_currentText, Qt::ElideRight, textRect.width());
		painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, elidedText);
	}

	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	QFont iconFont = QFont("ElaAwesome");
	iconFont.setPixelSize(13);
	painter.setFont(iconFont);
	QRectF iconRect(width() - 25, 0, 20, height());
	painter.translate(iconRect.center());
	painter.rotate(d->_expandIconRotate);
	painter.translate(-iconRect.center());
	painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::ChevronDown));

	painter.restore();
}

void ElaTreeSelect::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaTreeSelect);
	d->_isPressed = true;
	update();
	QWidget::mousePressEvent(event);
}

void ElaTreeSelect::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaTreeSelect);
	d->_isPressed = false;
	if (rect().contains(event->pos()))
	{
		if (d->_isPopupVisible)
		{
			d->_hidePopup();
		}
		else
		{
			d->_showPopup();
		}
	}
	update();
	QWidget::mouseReleaseEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void ElaTreeSelect::enterEvent(QEnterEvent *event)
#else
void ElaTreeSelect::enterEvent(QEvent *event)
#endif
{
	Q_D(ElaTreeSelect);
	d->_isHover = true;
	update();
	QWidget::enterEvent(event);
}

void ElaTreeSelect::leaveEvent(QEvent *event)
{
	Q_D(ElaTreeSelect);
	d->_isHover = false;
	update();
	QWidget::leaveEvent(event);
}