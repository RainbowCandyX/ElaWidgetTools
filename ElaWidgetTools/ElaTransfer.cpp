#include "ElaTransfer.h"

#include <QPainter>
#include <QPainterPath>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "ElaBaseListView.h"
#include "ElaCheckBox.h"
#include "ElaIconButton.h"
#include "ElaLineEdit.h"
#include "ElaScrollBar.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "private/ElaTransferPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaTransfer, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaTransfer, int, ItemHeight)
Q_PROPERTY_CREATE_Q_CPP(ElaTransfer, QString, SourceTitle)
Q_PROPERTY_CREATE_Q_CPP(ElaTransfer, QString, TargetTitle)
Q_PROPERTY_CREATE_Q_CPP(ElaTransfer, bool, IsSearchVisible)

ElaTransfer::ElaTransfer(QWidget *parent) : QWidget(parent), d_ptr(new ElaTransferPrivate())
{
	Q_D(ElaTransfer);
	d->q_ptr = this;
	d->_pBorderRadius = 6;
	d->_pItemHeight = 35;
	d->_pSourceTitle = "源列表";
	d->_pTargetTitle = "目标列表";
	d->_pIsSearchVisible = true;
	d->_themeMode = eTheme->getThemeMode();

	setMinimumSize(500, 300);

	QWidget *sourcePanel = new QWidget(this);
	QVBoxLayout *sourceLayout = new QVBoxLayout(sourcePanel);
	sourceLayout->setContentsMargins(0, 0, 0, 0);
	sourceLayout->setSpacing(4);

	QHBoxLayout *sourceHeaderLayout = new QHBoxLayout();
	sourceHeaderLayout->setContentsMargins(8, 8, 8, 4);
	sourceHeaderLayout->setSpacing(2);
	d->_sourceSelectAll = new ElaCheckBox(this);
	d->_sourceSelectAll->setFixedWidth(30);
	d->_sourceTitleText = new ElaText(d->_pSourceTitle, this);
	d->_sourceTitleText->setTextStyle(ElaTextType::BodyStrong);
	d->_sourceCountText = new ElaText("0/0", this);
	d->_sourceCountText->setTextStyle(ElaTextType::Caption);
	sourceHeaderLayout->addWidget(d->_sourceSelectAll, 0, Qt::AlignVCenter);
	sourceHeaderLayout->addWidget(d->_sourceTitleText);
	sourceHeaderLayout->addStretch();
	sourceHeaderLayout->addWidget(d->_sourceCountText);
	sourceLayout->addLayout(sourceHeaderLayout);

	d->_sourceSearchEdit = new ElaLineEdit(this);
	d->_sourceSearchEdit->setPlaceholderText("搜索");
	d->_sourceSearchEdit->setFixedHeight(30);
	d->_sourceSearchEdit->setClearButtonEnabled(true);
	QHBoxLayout *sourceSearchLayout = new QHBoxLayout();
	sourceSearchLayout->setContentsMargins(8, 0, 8, 0);
	sourceSearchLayout->addWidget(d->_sourceSearchEdit);
	sourceLayout->addLayout(sourceSearchLayout);

	d->_sourceModel = new QStandardItemModel(this);
	d->_sourceListView = new ElaBaseListView(this);
	d->_sourceListView->setModel(d->_sourceModel);
	d->_sourceListView->setSelectionMode(QAbstractItemView::MultiSelection);
	d->_sourceListView->setObjectName("ElaTransferSourceView");
	d->_sourceListView->setStyleSheet("#ElaTransferSourceView{background-color:transparent;}");
	ElaScrollBar *sourceScrollBar = new ElaScrollBar(d->_sourceListView->verticalScrollBar(), d->_sourceListView);
	sourceScrollBar->setIsAnimation(true);
	sourceLayout->addWidget(d->_sourceListView);

	QWidget *buttonPanel = new QWidget(this);
	QVBoxLayout *buttonLayout = new QVBoxLayout(buttonPanel);
	buttonLayout->setContentsMargins(8, 0, 8, 0);
	buttonLayout->setSpacing(8);
	buttonLayout->addStretch();
	d->_moveToTargetButton = new ElaIconButton(ElaIconType::AngleRight, 14, 36, 30, this);
	d->_moveToTargetButton->setBorderRadius(4);
	d->_moveToSourceButton = new ElaIconButton(ElaIconType::AngleLeft, 14, 36, 30, this);
	d->_moveToSourceButton->setBorderRadius(4);
	buttonLayout->addWidget(d->_moveToTargetButton);
	buttonLayout->addWidget(d->_moveToSourceButton);
	buttonLayout->addStretch();

	QWidget *targetPanel = new QWidget(this);
	QVBoxLayout *targetLayout = new QVBoxLayout(targetPanel);
	targetLayout->setContentsMargins(0, 0, 0, 0);
	targetLayout->setSpacing(4);

	QHBoxLayout *targetHeaderLayout = new QHBoxLayout();
	targetHeaderLayout->setContentsMargins(8, 8, 8, 4);
	targetHeaderLayout->setSpacing(2);
	d->_targetSelectAll = new ElaCheckBox(this);
	d->_targetSelectAll->setFixedWidth(30);
	d->_targetTitleText = new ElaText(d->_pTargetTitle, this);
	d->_targetTitleText->setTextStyle(ElaTextType::BodyStrong);
	d->_targetCountText = new ElaText("0/0", this);
	d->_targetCountText->setTextStyle(ElaTextType::Caption);
	targetHeaderLayout->addWidget(d->_targetSelectAll, 0, Qt::AlignVCenter);
	targetHeaderLayout->addWidget(d->_targetTitleText);
	targetHeaderLayout->addStretch();
	targetHeaderLayout->addWidget(d->_targetCountText);
	targetLayout->addLayout(targetHeaderLayout);

	d->_targetSearchEdit = new ElaLineEdit(this);
	d->_targetSearchEdit->setPlaceholderText("搜索");
	d->_targetSearchEdit->setFixedHeight(30);
	d->_targetSearchEdit->setClearButtonEnabled(true);
	QHBoxLayout *targetSearchLayout = new QHBoxLayout();
	targetSearchLayout->setContentsMargins(8, 0, 8, 0);
	targetSearchLayout->addWidget(d->_targetSearchEdit);
	targetLayout->addLayout(targetSearchLayout);

	d->_targetModel = new QStandardItemModel(this);
	d->_targetListView = new ElaBaseListView(this);
	d->_targetListView->setModel(d->_targetModel);
	d->_targetListView->setSelectionMode(QAbstractItemView::MultiSelection);
	d->_targetListView->setObjectName("ElaTransferTargetView");
	d->_targetListView->setStyleSheet("#ElaTransferTargetView{background-color:transparent;}");
	ElaScrollBar *targetScrollBar = new ElaScrollBar(d->_targetListView->verticalScrollBar(), d->_targetListView);
	targetScrollBar->setIsAnimation(true);
	targetLayout->addWidget(d->_targetListView);

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(sourcePanel, 1);
	mainLayout->addWidget(buttonPanel, 0);
	mainLayout->addWidget(targetPanel, 1);

	connect(d->_moveToTargetButton, &ElaIconButton::clicked, d, &ElaTransferPrivate::onMoveToTarget);
	connect(d->_moveToSourceButton, &ElaIconButton::clicked, d, &ElaTransferPrivate::onMoveToSource);
	connect(d->_sourceSearchEdit, &ElaLineEdit::textChanged, d, &ElaTransferPrivate::onSourceSearchTextChanged);
	connect(d->_targetSearchEdit, &ElaLineEdit::textChanged, d, &ElaTransferPrivate::onTargetSearchTextChanged);
	connect(d->_sourceSelectAll, &ElaCheckBox::stateChanged, d, &ElaTransferPrivate::onSourceSelectAllChanged);
	connect(d->_targetSelectAll, &ElaCheckBox::stateChanged, d, &ElaTransferPrivate::onTargetSelectAllChanged);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		QColor textColor = ElaThemeColor(themeMode, BasicText);
		for (int i = 0; i < d->_sourceModel->rowCount(); ++i)
		{
			d->_sourceModel->item(i)->setForeground(textColor);
		}
		for (int i = 0; i < d->_targetModel->rowCount(); ++i)
		{
			d->_targetModel->item(i)->setForeground(textColor);
		}
		update();
	});
}

ElaTransfer::~ElaTransfer()
{
}

void ElaTransfer::setSourceItems(const QStringList &items)
{
	Q_D(ElaTransfer);
	d->_sourceModel->clear();
	QColor textColor = ElaThemeColor(d->_themeMode, BasicText);
	for (const QString &text: items)
	{
		QStandardItem *item = new QStandardItem(text);
		item->setForeground(textColor);
		item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		d->_sourceModel->appendRow(item);
	}
	d->_refreshCountText();
}

void ElaTransfer::addSourceItem(const QString &text)
{
	Q_D(ElaTransfer);
	QStandardItem *item = new QStandardItem(text);
	item->setForeground(ElaThemeColor(d->_themeMode, BasicText));
	item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	d->_sourceModel->appendRow(item);
	d->_refreshCountText();
}

void ElaTransfer::addSourceItems(const QStringList &items)
{
	Q_D(ElaTransfer);
	QColor textColor = ElaThemeColor(d->_themeMode, BasicText);
	for (const QString &text: items)
	{
		QStandardItem *item = new QStandardItem(text);
		item->setForeground(textColor);
		item->setFlags(item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		d->_sourceModel->appendRow(item);
	}
	d->_refreshCountText();
}

QStringList ElaTransfer::getSourceItems() const
{
	QStringList items;
	for (int i = 0; i < d_ptr->_sourceModel->rowCount(); ++i)
	{
		items.append(d_ptr->_sourceModel->item(i)->text());
	}
	return items;
}

QStringList ElaTransfer::getTargetItems() const
{
	QStringList items;
	for (int i = 0; i < d_ptr->_targetModel->rowCount(); ++i)
	{
		items.append(d_ptr->_targetModel->item(i)->text());
	}
	return items;
}

void ElaTransfer::moveToTarget()
{
	Q_D(ElaTransfer);
	d->onMoveToTarget();
}

void ElaTransfer::moveToSource()
{
	Q_D(ElaTransfer);
	d->onMoveToSource();
}

void ElaTransfer::moveAllToTarget()
{
	Q_D(ElaTransfer);
	d->_sourceListView->selectAll();
	d->onMoveToTarget();
}

void ElaTransfer::moveAllToSource()
{
	Q_D(ElaTransfer);
	d->_targetListView->selectAll();
	d->onMoveToSource();
}

void ElaTransfer::paintEvent(QPaintEvent *event)
{
	Q_D(ElaTransfer);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicBorder));
	painter.setBrush(Qt::NoBrush);

	int panelWidth = (width() - 52) / 2;
	int buttonPanelX = panelWidth;

	QPainterPath sourcePath;
	QRectF sourceRect(0, 0, panelWidth, height());
	sourcePath.addRoundedRect(sourceRect, d->_pBorderRadius, d->_pBorderRadius);
	painter.drawPath(sourcePath);

	QPainterPath targetPath;
	QRectF targetRect(width() - panelWidth, 0, panelWidth, height());
	targetPath.addRoundedRect(targetRect, d->_pBorderRadius, d->_pBorderRadius);
	painter.drawPath(targetPath);

	painter.restore();
}