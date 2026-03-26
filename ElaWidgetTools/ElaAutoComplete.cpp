#include "ElaAutoComplete.h"

#include <QHBoxLayout>
#include <QSortFilterProxyModel>
#include <QStringListModel>

#include "ElaAutoCompleteContainer.h"
#include "ElaAutoCompleteDelegate.h"
#include "ElaBaseListView.h"
#include "ElaLineEdit.h"
#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "private/ElaAutoCompletePrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaAutoComplete, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaAutoComplete, int, MaxVisibleItems)
Q_PROPERTY_CREATE_Q_CPP(ElaAutoComplete, Qt::CaseSensitivity, CaseSensitivity)

ElaAutoComplete::ElaAutoComplete(QWidget *parent) : QWidget(parent), d_ptr(new ElaAutoCompletePrivate())
{
	Q_D(ElaAutoComplete);
	d->q_ptr = this;
	d->_pBorderRadius = 6;
	d->_pMaxVisibleItems = 6;
	d->_pCaseSensitivity = Qt::CaseInsensitive;

	setObjectName("ElaAutoComplete");

	d->_lineEdit = new ElaLineEdit(this);
	d->_lineEdit->setFixedHeight(35);
	setFixedHeight(35);
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(d->_lineEdit);

	d->_sourceModel = new QStringListModel(this);
	d->_filterModel = new QSortFilterProxyModel(this);
	d->_filterModel->setSourceModel(d->_sourceModel);
	d->_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

	d->_popup = new ElaAutoCompleteContainer(nullptr);
	d->_popup->setForwardTarget(d->_lineEdit);
	d->_popup->hide();

	d->_popupLayout = new QVBoxLayout(d->_popup);
	d->_popupLayout->setContentsMargins(4, 4, 4, 4);

	d->_listView = new ElaBaseListView(d->_popup);
	d->_listView->setModel(d->_filterModel);
	d->_listView->setItemDelegate(new ElaAutoCompleteDelegate(this));
	d->_listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ElaScrollBar *scrollBar = new ElaScrollBar(d->_listView->verticalScrollBar(), d->_listView);
	scrollBar->setIsAnimation(true);
	d->_popupLayout->addWidget(d->_listView);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaAutoCompletePrivate::onThemeModeChanged);

	connect(d->_lineEdit, &ElaLineEdit::textEdited, d, &ElaAutoCompletePrivate::onTextEdited);
	connect(d->_lineEdit, &ElaLineEdit::textChanged, this, &ElaAutoComplete::textChanged);
	connect(d->_listView, &ElaBaseListView::clicked, d, &ElaAutoCompletePrivate::onItemClicked);
	connect(d->_lineEdit, &ElaLineEdit::returnPressed, this, [=]()
	{
		Q_EMIT returnPressed(d->_lineEdit->text());
	});
	connect(d->_popup, &ElaAutoCompleteContainer::popupClosed, this, [=]()
	{
		d->_isPopupVisible = false;
		d->_isAllowHidePopup = false;
	});
}

ElaAutoComplete::~ElaAutoComplete()
{
	if (d_ptr->_popup)
	{
		delete d_ptr->_popup;
		d_ptr->_popup = nullptr;
	}
}

void ElaAutoComplete::setPlaceholderText(const QString &placeholderText)
{
	Q_D(ElaAutoComplete);
	d->_lineEdit->setPlaceholderText(placeholderText);
}

QString ElaAutoComplete::placeholderText() const
{
	return d_ptr->_lineEdit->placeholderText();
}

void ElaAutoComplete::setText(const QString &text)
{
	Q_D(ElaAutoComplete);
	d->_lineEdit->setText(text);
}

QString ElaAutoComplete::text() const
{
	return d_ptr->_lineEdit->text();
}

void ElaAutoComplete::setCompletions(const QStringList &completions)
{
	Q_D(ElaAutoComplete);
	d->_completions = completions;
	d->_sourceModel->setStringList(completions);
}

QStringList ElaAutoComplete::completions() const
{
	return d_ptr->_completions;
}

void ElaAutoComplete::setMatchMode(MatchMode mode)
{
	Q_D(ElaAutoComplete);
	d->_matchMode = mode;
}

ElaAutoComplete::MatchMode ElaAutoComplete::matchMode() const
{
	return d_ptr->_matchMode;
}

void ElaAutoComplete::setFixedHeight(int h)
{
	QWidget::setFixedHeight(h);
	Q_D(ElaAutoComplete);
	d->_lineEdit->setFixedHeight(h);
}