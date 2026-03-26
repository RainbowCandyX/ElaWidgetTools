#include "ElaAutoCompletePrivate.h"

#include <QApplication>
#include <QLayout>
#include <QPropertyAnimation>
#include <QRegularExpression>
#include <QScreen>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#include <QRegExp>
#endif

#include "ElaAutoComplete.h"
#include "ElaAutoCompleteContainer.h"
#include "ElaAutoCompleteDelegate.h"
#include "ElaBaseListView.h"
#include "ElaLineEdit.h"
#include "ElaTheme.h"

ElaAutoCompletePrivate::ElaAutoCompletePrivate(QObject *parent)
	: QObject(parent)
{
}

ElaAutoCompletePrivate::~ElaAutoCompletePrivate()
{
}

void ElaAutoCompletePrivate::onThemeModeChanged(ElaThemeType::ThemeMode themeMode)
{
	_themeMode = themeMode;
}

void ElaAutoCompletePrivate::onTextEdited(const QString &text)
{
	Q_Q(ElaAutoComplete);
	Q_EMIT q->textEdited(text);

	if (text.isEmpty())
	{
		_hidePopup();
		return;
	}

	_updateFilter(text);

	if (_filterModel->rowCount() > 0)
	{
		_showPopup();
	}
	else
	{
		_hidePopup();
	}
}

void ElaAutoCompletePrivate::onItemClicked(const QModelIndex &index)
{
	Q_Q(ElaAutoComplete);
	if (!index.isValid())
	{
		return;
	}
	QString text = _filterModel->data(index, Qt::DisplayRole).toString();
	_lineEdit->setText(text);
	_hidePopup();
	Q_EMIT q->completionSelected(text);
}

void ElaAutoCompletePrivate::_showPopup()
{
	if (!_popup)
	{
		return;
	}

	int rowCount = _filterModel->rowCount();
	int visibleRows = qMin(rowCount, _pMaxVisibleItems);
	int popupHeight = visibleRows * 36 + 8;
	int popupWidth = _lineEdit->width();

	QPoint lineEditGlobal = _lineEdit->mapToGlobal(QPoint(0, 0));
	QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();

	bool showAbove = (lineEditGlobal.y() + _lineEdit->height() + popupHeight > screenGeometry.bottom()) &&
	                 (lineEditGlobal.y() - popupHeight >= screenGeometry.top());

	QPoint popupPos;
	if (showAbove)
	{
		popupPos = QPoint(lineEditGlobal.x(), lineEditGlobal.y() - popupHeight - 3);
	}
	else
	{
		popupPos = QPoint(lineEditGlobal.x(), lineEditGlobal.y() + _lineEdit->height() + 3);
	}

	if (_isPopupVisible)
	{
		_popup->setFixedSize(popupWidth, popupHeight);
		_popup->move(popupPos);
		_popup->update();
		return;
	}
	_isPopupVisible = true;

	_popup->setFixedSize(popupWidth, popupHeight);
	_popup->show();
	_popup->move(popupPos);

	_listView->resize(popupWidth - 8, popupHeight - 8);

	QLayout *layout = _popup->layout();
	while (layout->count())
	{
		layout->takeAt(0);
	}

	QPropertyAnimation *heightAnimation = new QPropertyAnimation(_popup, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
	{
		_popup->setFixedHeight(value.toUInt());
	});
	heightAnimation->setStartValue(1);
	heightAnimation->setEndValue(popupHeight);
	heightAnimation->setEasingCurve(QEasingCurve::OutCubic);
	heightAnimation->setDuration(400);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	QPropertyAnimation *viewAnimation = new QPropertyAnimation(_listView, "pos");
	connect(viewAnimation, &QPropertyAnimation::finished, this, [=]()
	{
		_isAllowHidePopup = true;
		layout->addWidget(_listView);
	});
	QPoint viewPos = _listView->pos();
	viewAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - _listView->height()));
	viewAnimation->setEndValue(viewPos);
	viewAnimation->setEasingCurve(QEasingCurve::OutCubic);
	viewAnimation->setDuration(400);
	viewAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaAutoCompletePrivate::_hidePopup()
{
	if (!_popup || !_isPopupVisible)
	{
		return;
	}
	if (!_isAllowHidePopup)
	{
		return;
	}

	_isAllowHidePopup = false;
	int containerHeight = _popup->height();

	QLayout *layout = _popup->layout();
	while (layout->count())
	{
		layout->takeAt(0);
	}

	QPropertyAnimation *viewAnimation = new QPropertyAnimation(_listView, "pos");
	QPoint viewPos = _listView->pos();
	connect(viewAnimation, &QPropertyAnimation::finished, this, [=]()
	{
		layout->addWidget(_listView);
		_popup->hide();
		_popup->setFixedHeight(containerHeight);
		_listView->move(viewPos);
		_isPopupVisible = false;
	});
	viewAnimation->setStartValue(viewPos);
	viewAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - _listView->height()));
	viewAnimation->setEasingCurve(QEasingCurve::InCubic);
	viewAnimation->setDuration(400);
	viewAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	QPropertyAnimation *heightAnimation = new QPropertyAnimation(_popup, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
	{
		_popup->setFixedHeight(value.toInt());
	});
	heightAnimation->setStartValue(_popup->height());
	heightAnimation->setEndValue(1);
	heightAnimation->setEasingCurve(QEasingCurve::InCubic);
	heightAnimation->setDuration(400);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaAutoCompletePrivate::_updateFilter(const QString &text)
{
	Qt::CaseSensitivity cs = _pCaseSensitivity;
	switch (_matchMode)
	{
		case ElaAutoComplete::StartsWith:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
			QRegularExpression::PatternOption caseOption = cs == Qt::CaseInsensitive
				                                               ? QRegularExpression::CaseInsensitiveOption
				                                               : QRegularExpression::NoPatternOption;
			_filterModel->setFilterRegularExpression(
				QRegularExpression("^" + QRegularExpression::escape(text), caseOption));
#else
			_filterModel->setFilterCaseSensitivity(cs);
			_filterModel->setFilterRegExp(QRegExp("^" + QRegExp::escape(text), cs));
#endif
			break;
		}
		case ElaAutoComplete::EndsWith:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
			QRegularExpression::PatternOption caseOption = cs == Qt::CaseInsensitive
				                                               ? QRegularExpression::CaseInsensitiveOption
				                                               : QRegularExpression::NoPatternOption;
			_filterModel->setFilterRegularExpression(
				QRegularExpression(QRegularExpression::escape(text) + "$", caseOption));
#else
			_filterModel->setFilterCaseSensitivity(cs);
			_filterModel->setFilterRegExp(QRegExp(QRegExp::escape(text) + "$", cs));
#endif
			break;
		}
		case ElaAutoComplete::RegExp:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
			QRegularExpression::PatternOption caseOption = cs == Qt::CaseInsensitive
				                                               ? QRegularExpression::CaseInsensitiveOption
				                                               : QRegularExpression::NoPatternOption;
			QRegularExpression re(text, caseOption);
			if (re.isValid())
			{
				_filterModel->setFilterRegularExpression(re);
			}
#else
			QRegExp re(text, cs);
			if (re.isValid())
			{
				_filterModel->setFilterRegExp(re);
			}
#endif
			break;
		}
		case ElaAutoComplete::Contains:
		default:
			_filterModel->setFilterCaseSensitivity(cs);
			_filterModel->setFilterFixedString(text);
			break;
	}

	ElaAutoCompleteDelegate *delegate = qobject_cast<ElaAutoCompleteDelegate *>(_listView->itemDelegate());
	if (delegate)
	{
		delegate->setHighlightText(text);
		delegate->setMatchMode(_matchMode);
	}
}