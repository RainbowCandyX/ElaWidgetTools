#include "ElaCaptcha.h"

#include "ElaCaptchaPrivate.h"
#include "ElaTheme.h"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QRegularExpressionValidator>

Q_PROPERTY_CREATE_Q_CPP(ElaCaptcha, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaCaptcha, int, BoxSpacing)

void ElaCaptcha::setCodeLength(int CodeLength)
{
	Q_D(ElaCaptcha);
	if (d->_pCodeLength == CodeLength)
	{
		return;
	}
	d->_pCodeLength = CodeLength;
	d->_buildBoxes();
	Q_EMIT pCodeLengthChanged();
}

int ElaCaptcha::getCodeLength() const
{
	return d_ptr->_pCodeLength;
}

void ElaCaptcha::setBoxSize(int BoxSize)
{
	Q_D(ElaCaptcha);
	if (d->_pBoxSize == BoxSize)
	{
		return;
	}
	d->_pBoxSize = BoxSize;
	for (QLineEdit *box: d->_boxes)
	{
		box->setFixedSize(d->_pBoxSize, d->_pBoxSize);
	}
	setFixedHeight(d->_pBoxSize + 8);
	Q_EMIT pBoxSizeChanged();
}

int ElaCaptcha::getBoxSize() const
{
	return d_ptr->_pBoxSize;
}

void ElaCaptcha::setInputMode(InputMode mode)
{
	Q_D(ElaCaptcha);
	d->_inputMode = static_cast<int>(mode);
	d->_updateValidators();
}

ElaCaptcha::InputMode ElaCaptcha::getInputMode() const
{
	return static_cast<InputMode>(d_ptr->_inputMode);
}

ElaCaptcha::ElaCaptcha(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaCaptchaPrivate())
{
	Q_D(ElaCaptcha);
	d->q_ptr = this;
	d->_pCodeLength = 6;
	d->_pBorderRadius = 6;
	d->_pBoxSize = 42;
	d->_pBoxSpacing = 8;
	d->_inputMode = AlphaNumeric;
	setObjectName("ElaCaptcha");
	setFixedHeight(d->_pBoxSize + 8);

	d->_boxLayout = new QHBoxLayout(this);
	d->_boxLayout->setContentsMargins(0, 0, 0, 0);
	d->_boxLayout->setSpacing(d->_pBoxSpacing);

	d->_themeMode = eTheme->getThemeMode();
	d->_buildBoxes();
	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaCaptchaPrivate::onThemeChanged);
}

ElaCaptcha::~ElaCaptcha()
{
}

QString ElaCaptcha::getCode() const
{
	Q_D(const ElaCaptcha);
	QString code;
	for (QLineEdit *box: d->_boxes)
	{
		code += box->text();
	}
	return code;
}

void ElaCaptcha::clear()
{
	Q_D(ElaCaptcha);
	for (QLineEdit *box: d->_boxes)
	{
		box->blockSignals(true);
		box->clear();
		box->blockSignals(false);
	}
	d->_applyThemeStyle();
	if (!d->_boxes.isEmpty())
	{
		d->_boxes.first()->setFocus();
	}
	Q_EMIT codeChanged(QString());
}

bool ElaCaptcha::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaCaptcha);
	QLineEdit *box = qobject_cast<QLineEdit *>(watched);
	if (!box)
	{
		return QWidget::eventFilter(watched, event);
	}
	int index = d->_boxes.indexOf(box);
	if (index < 0)
	{
		return QWidget::eventFilter(watched, event);
	}

	if (event->type() == QEvent::FocusIn)
	{
		d->_applyThemeStyle();
		return false;
	}
	if (event->type() == QEvent::FocusOut)
	{
		d->_applyThemeStyle();
		return false;
	}
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

		// Paste handling (Ctrl+V)
		if (keyEvent->matches(QKeySequence::Paste))
		{
			QString clipText = QApplication::clipboard()->text().trimmed().toUpper();
			if (d->_inputMode == ElaCaptcha::DigitOnly)
			{
				clipText.remove(QRegularExpression("[^0-9]"));
			}
			else
			{
				clipText.remove(QRegularExpression("[^A-Z0-9]"));
			}
			if (!clipText.isEmpty())
			{
				for (int i = 0; i < clipText.length() && (index + i) < d->_boxes.count(); ++i)
				{
					d->_boxes[index + i]->blockSignals(true);
					d->_boxes[index + i]->setText(clipText.mid(i, 1));
					d->_boxes[index + i]->blockSignals(false);
				}
				d->_applyThemeStyle();
				int nextIndex = qMin(index + clipText.length(), d->_boxes.count() - 1);
				d->_boxes[nextIndex]->setFocus();
				// Emit signals
				QString code = getCode();
				Q_EMIT codeChanged(code);
				if (code.length() == d->_pCodeLength)
				{
					Q_EMIT codeCompleted(code);
				}
			}
			return true;
		}

		// Arrow key navigation
		if (keyEvent->key() == Qt::Key_Left)
		{
			if (index > 0)
			{
				d->_boxes[index - 1]->setFocus();
			}
			return true;
		}
		if (keyEvent->key() == Qt::Key_Right)
		{
			if (index < d->_boxes.count() - 1)
			{
				d->_boxes[index + 1]->setFocus();
			}
			return true;
		}

		// Backspace on empty box: move to previous and clear it
		if (keyEvent->key() == Qt::Key_Backspace)
		{
			if (box->text().isEmpty())
			{
				if (index > 0)
				{
					d->_boxes[index - 1]->clear();
					d->_boxes[index - 1]->setFocus();
					d->_applyThemeStyle();
					Q_EMIT codeChanged(getCode());
				}
				return true;
			}
			// Non-empty: let default handle clear, then update
			box->clear();
			d->_applyThemeStyle();
			Q_EMIT codeChanged(getCode());
			return true;
		}

		// Input handling
		QString text = keyEvent->text();
		if (!text.isEmpty())
		{
			QChar ch = text.at(0);
			bool valid = false;
			if (d->_inputMode == ElaCaptcha::DigitOnly)
			{
				valid = ch.isDigit();
			}
			else
			{
				valid = ch.isLetterOrNumber();
			}
			if (!valid)
			{
				return true;
			}
			// Auto uppercase
			text = text.toUpper();
			box->blockSignals(true);
			box->setText(text);
			box->blockSignals(false);
			d->_applyThemeStyle();
			// Auto-advance
			if (index < d->_boxes.count() - 1)
			{
				d->_boxes[index + 1]->setFocus();
			}
			QString code = getCode();
			Q_EMIT codeChanged(code);
			if (code.length() == d->_pCodeLength)
			{
				Q_EMIT codeCompleted(code);
			}
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

// Private implementation

ElaCaptchaPrivate::ElaCaptchaPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaCaptchaPrivate::~ElaCaptchaPrivate()
{
}

void ElaCaptchaPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
	_themeMode = themeMode;
	_applyThemeStyle();
}

void ElaCaptchaPrivate::_buildBoxes()
{
	Q_Q(ElaCaptcha);
	// Remove existing boxes
	for (QLineEdit *box: _boxes)
	{
		_boxLayout->removeWidget(box);
		box->removeEventFilter(q);
		box->deleteLater();
	}
	_boxes.clear();

	_boxLayout->setSpacing(_pBoxSpacing);

	for (int i = 0; i < _pCodeLength; ++i)
	{
		QLineEdit *box = new QLineEdit(q);
		box->setMaxLength(1);
		box->setAlignment(Qt::AlignCenter);
		box->setFixedSize(_pBoxSize, _pBoxSize);
		box->setFocusPolicy(Qt::StrongFocus);
		box->setContextMenuPolicy(Qt::NoContextMenu);
		QFont font = box->font();
		font.setPixelSize(20);
		font.setBold(true);
		box->setFont(font);
		box->installEventFilter(q);
		_boxLayout->addWidget(box);
		_boxes.append(box);
	}

	_applyThemeStyle();

	if (!_boxes.isEmpty())
	{
		_boxes.first()->setFocus();
	}
}

void ElaCaptchaPrivate::_updateValidators()
{
	Q_Q(ElaCaptcha);
	QString pattern = (_inputMode == ElaCaptcha::DigitOnly) ? "[0-9]" : "[A-Za-z0-9]";
	QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression(pattern), q);
	for (QLineEdit *box: _boxes)
	{
		box->setValidator(validator);
	}
}

void ElaCaptchaPrivate::_applyThemeStyle()
{
	for (int i = 0; i < _boxes.count(); ++i)
	{
		QLineEdit *box = _boxes[i];
		bool focused = box->hasFocus();
		_applyBoxStyle(box, focused);
	}
}

void ElaCaptchaPrivate::_applyBoxStyle(QLineEdit *box, bool focused)
{
	QColor bgColor = ElaThemeColor(_themeMode, BasicBase);
	QColor textColor = ElaThemeColor(_themeMode, BasicText);
	QColor borderColor;
	if (focused || !box->text().isEmpty())
	{
		borderColor = ElaThemeColor(_themeMode, PrimaryNormal);
	}
	else
	{
		borderColor = ElaThemeColor(_themeMode, BasicBorder);
	}
	box->setStyleSheet(
		QString("QLineEdit { background: %1; border: 1px solid %2; border-radius: %3px; "
			"font-size: 20px; font-weight: bold; color: %4; }"
			"QLineEdit:focus { border: 2px solid %5; }")
		.arg(bgColor.name(QColor::HexArgb))
		.arg(borderColor.name(QColor::HexArgb))
		.arg(_pBorderRadius)
		.arg(textColor.name(QColor::HexArgb))
		.arg(ElaThemeColor(_themeMode, PrimaryNormal).name(QColor::HexArgb)));
}