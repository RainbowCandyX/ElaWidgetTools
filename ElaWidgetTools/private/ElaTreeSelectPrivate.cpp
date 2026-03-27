#include "ElaTreeSelectPrivate.h"

#include <QApplication>
#include <QLayout>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QScreen>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

#include "ElaLineEdit.h"
#include "ElaTheme.h"
#include "ElaTreeSelect.h"
#include "ElaTreeView.h"

ElaTreeSelectPopupWidget::ElaTreeSelectPopupWidget(QWidget *parent) : QWidget(parent)
{
}

void ElaTreeSelectPopupWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(ElaThemeColor(_themeMode, PopupBorder), 1));
	painter.setBrush(ElaThemeColor(_themeMode, PopupBase));
	QRectF bgRect(0, 0, width(), height());
	QPainterPath path;
	path.addRoundedRect(bgRect, _borderRadius, _borderRadius);
	painter.drawPath(path);
}

ElaTreeSelectPrivate::ElaTreeSelectPrivate(QObject *parent) : QObject{parent}
{
}

ElaTreeSelectPrivate::~ElaTreeSelectPrivate()
{
}

bool ElaTreeSelectPrivate::eventFilter(QObject *watched, QEvent *event)
{
	if (_isPopupVisible && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		QPoint globalPos = mouseEvent->globalPosition().toPoint();
		Q_Q(ElaTreeSelect);
		QRect popupRect(_popupContainer->mapToGlobal(QPoint(0, 0)), _popupContainer->size());
		QRect triggerRect(q->mapToGlobal(QPoint(0, 0)), q->size());
		if (!popupRect.contains(globalPos) && !triggerRect.contains(globalPos))
		{
			_hidePopup();
		}
	}
	return QObject::eventFilter(watched, event);
}

void ElaTreeSelectPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
	Q_Q(ElaTreeSelect);
	_themeMode = themeMode;
	_popupContainer->_themeMode = themeMode;
	q->update();
}

void ElaTreeSelectPrivate::onItemClicked(const QModelIndex &index)
{
	Q_Q(ElaTreeSelect);
	if (!index.isValid())
	{
		return;
	}
	QModelIndex sourceIndex = index;
	if (_proxyModel)
	{
		sourceIndex = _proxyModel->mapToSource(index);
	}
	if (!sourceIndex.isValid())
	{
		return;
	}
	if (_model && _model->hasChildren(sourceIndex))
	{
		return;
	}
	_currentIndex = sourceIndex;
	_updateDisplayText();
	_hidePopup();
	Q_EMIT q->currentIndexChanged(_currentIndex);
	Q_EMIT q->currentTextChanged(_currentText);
}

void ElaTreeSelectPrivate::onSearchTextChanged(const QString &text)
{
	if (_proxyModel)
	{
		_proxyModel->setFilterFixedString(text);
		if (!text.isEmpty())
		{
			_treeView->expandAll();
		}
		_updatePopupHeight();
	}
}

void ElaTreeSelectPrivate::onTreeExpanded(const QModelIndex &index)
{
	Q_UNUSED(index)
	_updatePopupHeight();
}

void ElaTreeSelectPrivate::onTreeCollapsed(const QModelIndex &index)
{
	Q_UNUSED(index)
	_updatePopupHeight();
}

void ElaTreeSelectPrivate::_showPopup()
{
	Q_Q(ElaTreeSelect);
	if (_isPopupVisible || !_model)
	{
		return;
	}
	_isPopupVisible = true;

	int popupHeight = _calculatePopupHeight();
	QPoint globalPos = q->mapToGlobal(QPoint(0, 0));
	QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();

	bool showAbove = (globalPos.y() + q->height() + popupHeight > screenGeometry.bottom()) &&
	                 (globalPos.y() - popupHeight >= screenGeometry.top());

	QPoint containerPos;
	if (showAbove)
	{
		containerPos = QPoint(globalPos.x(), globalPos.y() - popupHeight - 3);
	}
	else
	{
		containerPos = QPoint(globalPos.x(), globalPos.y() + q->height() + 3);
	}

	_popupContainer->setFixedWidth(q->width());
	_popupContainer->move(containerPos);
	_popupContainer->setFixedHeight(1);
	_popupContainer->show();
	_popupContainer->raise();

	if (_searchEdit && _pIsSearchVisible)
	{
		_searchEdit->setVisible(true);
		_searchEdit->clear();
		_searchEdit->setFocus();
	}

	qApp->installEventFilter(this);

	QPropertyAnimation *heightAnimation = new QPropertyAnimation(_popupContainer, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value)
	{
		_popupContainer->setFixedHeight(value.toInt());
	});
	heightAnimation->setStartValue(1);
	heightAnimation->setEndValue(popupHeight);
	heightAnimation->setEasingCurve(QEasingCurve::OutCubic);
	heightAnimation->setDuration(400);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "");
	connect(rotateAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value)
	{
		_expandIconRotate = value.toReal();
		q->update();
	});
	rotateAnimation->setStartValue(_expandIconRotate);
	rotateAnimation->setEndValue(-180.0);
	rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
	rotateAnimation->setDuration(300);
	rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	q->update();
}

void ElaTreeSelectPrivate::_hidePopup()
{
	Q_Q(ElaTreeSelect);
	if (!_isPopupVisible)
	{
		return;
	}
	_isPopupVisible = false;

	qApp->removeEventFilter(this);

	int currentHeight = _popupContainer->height();
	QPropertyAnimation *heightAnimation = new QPropertyAnimation(_popupContainer, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value)
	{
		_popupContainer->setFixedHeight(value.toInt());
	});
	connect(heightAnimation, &QPropertyAnimation::finished, q, [=]()
	{
		_popupContainer->hide();
		if (_proxyModel)
		{
			_proxyModel->setFilterFixedString("");
		}
	});
	heightAnimation->setStartValue(currentHeight);
	heightAnimation->setEndValue(1);
	heightAnimation->setEasingCurve(QEasingCurve::InCubic);
	heightAnimation->setDuration(300);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	QPropertyAnimation *rotateAnimation = new QPropertyAnimation(this, "");
	connect(rotateAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value)
	{
		_expandIconRotate = value.toReal();
		q->update();
	});
	rotateAnimation->setStartValue(_expandIconRotate);
	rotateAnimation->setEndValue(0.0);
	rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
	rotateAnimation->setDuration(300);
	rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	q->update();
}

void ElaTreeSelectPrivate::_updateDisplayText()
{
	if (_currentIndex.isValid() && _model)
	{
		QStringList pathParts;
		QModelIndex index = _currentIndex;
		while (index.isValid())
		{
			pathParts.prepend(index.data(Qt::DisplayRole).toString());
			index = index.parent();
		}
		_currentText = pathParts.join(" / ");
	}
	else
	{
		_currentText.clear();
	}
}

void ElaTreeSelectPrivate::_updatePopupHeight()
{
	Q_Q(ElaTreeSelect);
	if (!_isPopupVisible)
	{
		return;
	}
	int newHeight = _calculatePopupHeight();
	QPropertyAnimation *heightAnimation = new QPropertyAnimation(_popupContainer, "maximumHeight");
	connect(heightAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant &value)
	{
		_popupContainer->setFixedHeight(value.toInt());
	});
	heightAnimation->setStartValue(_popupContainer->height());
	heightAnimation->setEndValue(newHeight);
	heightAnimation->setEasingCurve(QEasingCurve::OutCubic);
	heightAnimation->setDuration(250);
	heightAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

int ElaTreeSelectPrivate::_calculatePopupHeight() const
{
	int visibleCount = _countVisibleItems();
	int maxItems = _pMaxVisibleItems > 0 ? _pMaxVisibleItems : 8;
	int itemCount = qMin(visibleCount, maxItems);
	if (itemCount < 1)
	{
		itemCount = 1;
	}
	int searchHeight = (_pIsSearchVisible && _searchEdit) ? 40 : 0;
	return itemCount * _pItemHeight + searchHeight + 16;
}

int ElaTreeSelectPrivate::_countVisibleItems(const QModelIndex &parent) const
{
	QAbstractItemModel *viewModel = _proxyModel ? static_cast<QAbstractItemModel *>(_proxyModel) : static_cast<QAbstractItemModel *>(_model);
	if (!viewModel)
	{
		return 0;
	}
	int count = 0;
	int rowCount = viewModel->rowCount(parent);
	for (int i = 0; i < rowCount; ++i)
	{
		count++;
		QModelIndex childIndex = viewModel->index(i, 0, parent);
		if (_treeView && _treeView->isExpanded(childIndex))
		{
			count += _countVisibleItems(childIndex);
		}
	}
	return count;
}