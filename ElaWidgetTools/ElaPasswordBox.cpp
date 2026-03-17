#include "ElaPasswordBox.h"

#include <QAction>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "ElaApplication.h"
#include "ElaDef.h"
#include "ElaEventBus.h"
#include "ElaIcon.h"
#include "ElaLineEditStyle.h"
#include "ElaMenu.h"
#include "ElaTheme.h"
#include "private/ElaPasswordBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPasswordBox, int, BorderRadius)

ElaPasswordBox::ElaPasswordBox(QWidget *parent)
	: QLineEdit(parent), d_ptr(new ElaPasswordBoxPrivate())
{
	Q_D(ElaPasswordBox);
	d->q_ptr = this;
	setObjectName("ElaPasswordBox");
	setFixedHeight(35);
	d->_themeMode = eTheme->getThemeMode();
	d->_pBorderRadius = 6;
	d->_pExpandMarkWidth = 0;
	d->_pIsPasswordVisible = false;
	setEchoMode(QLineEdit::Password);
	setFocusPolicy(Qt::StrongFocus);
	// 事件总线
	d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
	d->_focusEvent->registerAndInit();
	setMouseTracking(true);
	QFont textFont = font();
	textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
	setFont(textFont);
	setStyle(new ElaLineEditStyle(style()));
	setStyleSheet("#ElaPasswordBox{background-color:transparent;padding-left: 10px;padding-right: 36px;}");
	// Toggle action for password visibility
	d->_toggleAction = new QAction(this);
	d->_toggleAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Eye, 22, 32, 32, ElaThemeColor(d->_themeMode, BasicText)));
	addAction(d->_toggleAction, QLineEdit::TrailingPosition);
	connect(d->_toggleAction, &QAction::triggered, this, [=]()
	{
		setIsPasswordVisible(!getIsPasswordVisible());
	});
	d->onThemeChanged(eTheme->getThemeMode());
	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaPasswordBoxPrivate::onThemeChanged);
	setVisible(true);
}

ElaPasswordBox::~ElaPasswordBox()
{
	delete this->style();
}

void ElaPasswordBox::setIsPasswordVisible(bool isPasswordVisible)
{
	Q_D(ElaPasswordBox);
	d->_pIsPasswordVisible = isPasswordVisible;
	if (isPasswordVisible)
	{
		setEchoMode(QLineEdit::Normal);
		d->_toggleAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::EyeSlash, 22, 32, 32, ElaThemeColor(d->_themeMode, BasicText)));
	}
	else
	{
		setEchoMode(QLineEdit::Password);
		d->_toggleAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Eye, 22, 32, 32, ElaThemeColor(d->_themeMode, BasicText)));
	}
	Q_EMIT pIsPasswordVisibleChanged();
}

bool ElaPasswordBox::getIsPasswordVisible() const
{
	Q_D(const ElaPasswordBox);
	return d->_pIsPasswordVisible;
}

void ElaPasswordBox::focusInEvent(QFocusEvent *event)
{
	Q_D(ElaPasswordBox);
	Q_EMIT focusIn(this->text());
	if (event->reason() == Qt::MouseFocusReason)
	{
		QPropertyAnimation *markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
		{
			update();
		});
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->_pExpandMarkWidth);
		markAnimation->setEndValue(width() / 2 - d->_pBorderRadius / 2);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
	QLineEdit::focusInEvent(event);
}

void ElaPasswordBox::focusOutEvent(QFocusEvent *event)
{
	Q_D(ElaPasswordBox);
	Q_EMIT focusOut(this->text());
	if (event->reason() != Qt::PopupFocusReason)
	{
		QPropertyAnimation *markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
		connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant &value)
		{
			update();
		});
		markAnimation->setDuration(300);
		markAnimation->setEasingCurve(QEasingCurve::InOutSine);
		markAnimation->setStartValue(d->_pExpandMarkWidth);
		markAnimation->setEndValue(0);
		markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
		Q_EMIT wmFocusOut(text());
	}
	QLineEdit::focusOutEvent(event);
}

void ElaPasswordBox::paintEvent(QPaintEvent *event)
{
	Q_D(ElaPasswordBox);
	if (palette().color(QPalette::Text) != ElaThemeColor(d->_themeMode, BasicText))
	{
		d->onThemeChanged(d->_themeMode);
	}
	QLineEdit::paintEvent(event);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
	painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
	painter.restore();
}

void ElaPasswordBox::contextMenuEvent(QContextMenuEvent *event)
{
	ElaMenu *menu = new ElaMenu(this);
	menu->setMenuItemHeight(27);
	menu->setAttribute(Qt::WA_DeleteOnClose);
	QAction *action = nullptr;
	if (!isReadOnly())
	{
		action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
		action->setEnabled(isUndoAvailable());
		connect(action, &QAction::triggered, this, &ElaPasswordBox::undo);

		action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
		action->setEnabled(isRedoAvailable());
		connect(action, &QAction::triggered, this, &ElaPasswordBox::redo);
		menu->addSeparator();
	}
#ifndef QT_NO_CLIPBOARD
	if (!isReadOnly())
	{
		action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
		action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
		connect(action, &QAction::triggered, this, &ElaPasswordBox::paste);
	}
#endif
	if (!isReadOnly())
	{
		action = menu->addElaIconAction(ElaIconType::DeleteLeft, tr("删除"));
		action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
		connect(action, &QAction::triggered, this, [=](bool checked)
		{
			if (hasSelectedText())
			{
				int startIndex = selectionStart();
				int endIndex = selectionEnd();
				setText(text().remove(startIndex, endIndex - startIndex));
			}
		});
	}
	if (!menu->isEmpty())
	{
		menu->addSeparator();
	}
	action = menu->addAction(tr("全选"));
	action->setShortcut(QKeySequence::SelectAll);
	action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
	connect(action, &QAction::triggered, this, &ElaPasswordBox::selectAll);
	menu->popup(event->globalPos());
}

// ElaPasswordBoxPrivate
ElaPasswordBoxPrivate::ElaPasswordBoxPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaPasswordBoxPrivate::~ElaPasswordBoxPrivate()
{
}

void ElaPasswordBoxPrivate::onWMWindowClickedEvent(QVariantMap data)
{
	Q_Q(ElaPasswordBox);
	ElaAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<ElaAppBarType::WMMouseActionType>();
	if (actionType == ElaAppBarType::WMLBUTTONDOWN)
	{
		if (q->hasSelectedText() && q->hasFocus())
		{
			q->clearFocus();
		}
	}
	else if (actionType == ElaAppBarType::WMLBUTTONUP || actionType == ElaAppBarType::WMNCLBUTTONDOWN)
	{
		if (ElaApplication::containsCursorToItem(q) || (actionType == ElaAppBarType::WMLBUTTONUP && q->hasSelectedText()))
		{
			return;
		}
		if (q->hasFocus())
		{
			q->clearFocus();
		}
	}
}

void ElaPasswordBoxPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
	Q_Q(ElaPasswordBox);
	_themeMode = themeMode;
	QPalette palette = q->palette();
	palette.setColor(QPalette::Text, ElaThemeColor(_themeMode, BasicText));
	palette.setColor(QPalette::PlaceholderText, _themeMode == ElaThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
	q->setPalette(palette);
	// Update toggle icon color for new theme
	if (_toggleAction)
	{
		if (_pIsPasswordVisible)
		{
			_toggleAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::EyeSlash, 22, 32, 32, ElaThemeColor(_themeMode, BasicText)));
		}
		else
		{
			_toggleAction->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::Eye, 22, 32, 32, ElaThemeColor(_themeMode, BasicText)));
		}
	}
}