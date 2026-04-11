#include "ElaTerminalWidget.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextDocument>
#include <QVBoxLayout>

#include "ElaScrollBar.h"
#include "ElaTheme.h"
#include "private/ElaTerminalWidgetPrivate.h"

Q_PROPERTY_REF_CREATE_Q_CPP(ElaTerminalWidget, QString, Prompt)
Q_PROPERTY_CREATE_Q_CPP(ElaTerminalWidget, int, MaxHistorySize)
Q_PROPERTY_CREATE_Q_CPP(ElaTerminalWidget, int, MaxLineCount)

void ElaTerminalWidget::setFontPixelSize(int size)
{
	Q_D(ElaTerminalWidget);
	d->_pFontPixelSize = size;
	d->updateFont();
	Q_EMIT pFontPixelSizeChanged();
}

int ElaTerminalWidget::getFontPixelSize() const
{
	return d_ptr->_pFontPixelSize;
}

ElaTerminalWidget::ElaTerminalWidget(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaTerminalWidgetPrivate())
{
	Q_D(ElaTerminalWidget);
	d->q_ptr = this;
	d->_pPrompt = "$ ";
	d->_pMaxHistorySize = 500;
	d->_pMaxLineCount = 5000;
	d->_pFontPixelSize = 14;
	setObjectName("ElaTerminalWidget");

	d->_themeMode = eTheme->getThemeMode();

	d->_outputArea = new QPlainTextEdit(this);
	d->_outputArea->setReadOnly(true);
	d->_outputArea->setFrameShape(QFrame::NoFrame);
	d->_outputArea->setVerticalScrollBar(new ElaScrollBar(d->_outputArea));
	d->_outputArea->setHorizontalScrollBar(new ElaScrollBar(d->_outputArea));
	d->_outputArea->setLineWrapMode(QPlainTextEdit::NoWrap);

	d->_promptLabel = new QLabel(d->_pPrompt, this);

	d->_inputLine = new QLineEdit(this);
	d->_inputLine->setFrame(false);

	QHBoxLayout *inputLayout = new QHBoxLayout();
	inputLayout->setContentsMargins(6, 2, 6, 6);
	inputLayout->setSpacing(4);
	inputLayout->addWidget(d->_promptLabel);
	inputLayout->addWidget(d->_inputLine);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(d->_outputArea, 1);
	mainLayout->addLayout(inputLayout);

	auto applyTheme = [=](ElaThemeType::ThemeMode mode)
	{
		d->_themeMode = mode;
		QColor bgColor = (mode == ElaThemeType::Dark) ? QColor(0x1E, 0x1E, 0x2E) : QColor(0xF8, 0xF8, 0xF8);
		QColor textColor = (mode == ElaThemeType::Dark) ? QColor(0xCD, 0xD6, 0xF4) : QColor(0x2C, 0x2C, 0x2C);
		QColor inputBg = (mode == ElaThemeType::Dark) ? QColor(0x28, 0x28, 0x3A) : QColor(0xFF, 0xFF, 0xFF);
		QColor promptColor = ElaThemeColor(mode, PrimaryNormal);

		QString outputStyle = QString(
					"QPlainTextEdit { background-color: %1; color: %2; selection-background-color: %3; border: none; }")
				.arg(bgColor.name(), textColor.name(), ElaThemeColor(mode, PrimaryNormal).name());
		d->_outputArea->setStyleSheet(outputStyle);

		QString inputStyle = QString(
					"QLineEdit { background-color: %1; color: %2; border: none; padding: 4px; }")
				.arg(inputBg.name(), textColor.name());
		d->_inputLine->setStyleSheet(inputStyle);

		d->_promptLabel->setStyleSheet(QString("QLabel { color: %1; }").arg(promptColor.name()));

		setStyleSheet(QString("QWidget#ElaTerminalWidget { background-color: %1; }").arg(bgColor.name()));
	};
	applyTheme(d->_themeMode);
	connect(eTheme, &ElaTheme::themeModeChanged, this, applyTheme);

	d->updateFont();

	connect(this, &ElaTerminalWidget::pPromptChanged, this, [=]()
	{
		d->_promptLabel->setText(d->_pPrompt);
	});

	d->_inputLine->installEventFilter(this);

	connect(d->_inputLine, &QLineEdit::returnPressed, d, &ElaTerminalWidgetPrivate::submitCommand);
}

ElaTerminalWidget::~ElaTerminalWidget()
{
}

void ElaTerminalWidget::appendOutput(const QString &text, const QColor &color)
{
	Q_D(ElaTerminalWidget);
	if (color.isValid())
	{
		d->_outputArea->appendHtml(
			QString("<span style=\"color:%1;\">%2</span>")
			.arg(color.name(), text.toHtmlEscaped().replace("\n", "<br>")));
	}
	else
	{
		d->_outputArea->appendPlainText(text);
	}
	d->trimOutput();
	d->_outputArea->verticalScrollBar()->setValue(d->_outputArea->verticalScrollBar()->maximum());
}

void ElaTerminalWidget::appendHtml(const QString &html)
{
	Q_D(ElaTerminalWidget);
	d->_outputArea->appendHtml(html);
	d->trimOutput();
	d->_outputArea->verticalScrollBar()->setValue(d->_outputArea->verticalScrollBar()->maximum());
}

void ElaTerminalWidget::appendError(const QString &text)
{
	appendOutput(text, ElaThemeColor(d_ptr->_themeMode, StatusDanger));
}

void ElaTerminalWidget::appendSuccess(const QString &text)
{
	QColor green = (d_ptr->_themeMode == ElaThemeType::Dark) ? QColor(0xA6, 0xE3, 0xA1) : QColor(0x0F, 0x7B, 0x0F);
	appendOutput(text, green);
}

void ElaTerminalWidget::clear()
{
	Q_D(ElaTerminalWidget);
	d->_outputArea->clear();
}

QStringList ElaTerminalWidget::getCommandHistory() const
{
	return d_ptr->_commandHistory;
}

bool ElaTerminalWidget::eventFilter(QObject *watched, QEvent *event)
{
	Q_D(ElaTerminalWidget);
	if (watched == d->_inputLine && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Up)
		{
			d->navigateHistory(-1);
			return true;
		}
		else if (keyEvent->key() == Qt::Key_Down)
		{
			d->navigateHistory(1);
			return true;
		}
		else if (keyEvent->key() == Qt::Key_Tab)
		{
			Q_EMIT tabPressed(d->_inputLine->text());
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

ElaTerminalWidgetPrivate::ElaTerminalWidgetPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaTerminalWidgetPrivate::~ElaTerminalWidgetPrivate()
{
}

void ElaTerminalWidgetPrivate::submitCommand()
{
	Q_Q(ElaTerminalWidget);
	QString command = _inputLine->text().trimmed();
	if (command.isEmpty())
	{
		return;
	}

	QColor promptColor = ElaThemeColor(_themeMode, PrimaryNormal);
	_outputArea->appendHtml(
		QString("<span style=\"color:%1;\">%2</span>%3")
		.arg(promptColor.name(), _pPrompt.toHtmlEscaped(), command.toHtmlEscaped()));

	if (_commandHistory.isEmpty() || _commandHistory.last() != command)
	{
		_commandHistory.append(command);
		if (_commandHistory.size() > _pMaxHistorySize)
		{
			_commandHistory.removeFirst();
		}
	}
	_historyIndex = -1;
	_savedInput.clear();

	_inputLine->clear();
	trimOutput();
	_outputArea->verticalScrollBar()->setValue(_outputArea->verticalScrollBar()->maximum());

	Q_EMIT q->commandSubmitted(command);
}

void ElaTerminalWidgetPrivate::navigateHistory(int direction)
{
	if (_commandHistory.isEmpty())
	{
		return;
	}

	if (_historyIndex == -1)
	{
		_savedInput = _inputLine->text();
	}

	if (direction < 0)
	{
		if (_historyIndex == -1)
		{
			_historyIndex = _commandHistory.size() - 1;
		}
		else if (_historyIndex > 0)
		{
			_historyIndex--;
		}
		_inputLine->setText(_commandHistory.at(_historyIndex));
	}
	else
	{
		if (_historyIndex == -1)
		{
			return;
		}
		if (_historyIndex < _commandHistory.size() - 1)
		{
			_historyIndex++;
			_inputLine->setText(_commandHistory.at(_historyIndex));
		}
		else
		{
			_historyIndex = -1;
			_inputLine->setText(_savedInput);
		}
	}
}

void ElaTerminalWidgetPrivate::updateFont()
{
#ifdef Q_OS_MAC
	QFont monoFont("Menlo");
#elif defined(Q_OS_WIN)
	QFont monoFont("Consolas");
#else
	QFont monoFont("DejaVu Sans Mono");
#endif
	monoFont.setPixelSize(_pFontPixelSize);
	_outputArea->setFont(monoFont);
	_inputLine->setFont(monoFont);
	_promptLabel->setFont(monoFont);
}

void ElaTerminalWidgetPrivate::trimOutput()
{
	QTextDocument *doc = _outputArea->document();
	while (doc->blockCount() > _pMaxLineCount)
	{
		QTextCursor cursor(doc->begin());
		cursor.select(QTextCursor::BlockUnderCursor);
		cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
		cursor.removeSelectedText();
	}
}