#include "ElaCodeEditor.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QTextBlock>

#include "DeveloperComponents/ElaCodeEditorLineArea.h"
#include "DeveloperComponents/ElaCodeHighlighter.h"
#include "ElaTheme.h"
#include "private/ElaCodeEditorPrivate.h"

// --- ElaCodeEditorPrivate ---
ElaCodeEditorPrivate::ElaCodeEditorPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaCodeEditorPrivate::~ElaCodeEditorPrivate()
{
}

void ElaCodeEditorPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
	Q_Q(ElaCodeEditor);
	_themeMode = themeMode;

	QPalette pal = _editor->palette();
	pal.setColor(QPalette::Base, ElaThemeColor(_themeMode, BasicBase));
	pal.setColor(QPalette::Text, ElaThemeColor(_themeMode, BasicText));
	_editor->setPalette(pal);

	_lineArea->update();

	QList<QTextEdit::ExtraSelection> extraSelections;
	if (!_editor->isReadOnly())
	{
		QTextEdit::ExtraSelection selection;
		selection.format.setBackground(ElaThemeColor(_themeMode, BasicHover));
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = _editor->textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}
	_editor->setExtraSelections(extraSelections);

	ElaCodeHighlighter *highlighter = static_cast<ElaCodeHighlighter *>(_highlighter);
	if (highlighter)
	{
		highlighter->setThemeMode(_themeMode);
	}

	// Force stylesheet update for editor background
	_editor->setStyleSheet(QString("QPlainTextEdit { background-color: %1; color: %2; }")
		.arg(ElaThemeColor(_themeMode, BasicBase).name(QColor::HexArgb))
		.arg(ElaThemeColor(_themeMode, BasicText).name(QColor::HexArgb)));
	_editor->update();
	_lineArea->update();
}

// --- ElaCodeEditor ---
Q_PROPERTY_CREATE_Q_CPP(ElaCodeEditor, bool, IsReadOnly)
Q_PROPERTY_CREATE_Q_CPP(ElaCodeEditor, int, TabSize)

void ElaCodeEditor::setCode(const QString &Code)
{
	Q_D(ElaCodeEditor);
	if (d->_pCode != Code)
	{
		d->_pCode = Code;
		d->_editor->setPlainText(Code);
		Q_EMIT pCodeChanged();
	}
}

const QString &ElaCodeEditor::getCode() const
{
	return d_ptr->_pCode;
}

void ElaCodeEditor::setLanguage(Language lang)
{
	Q_D(ElaCodeEditor);
	d->_language = static_cast<int>(lang);
	ElaCodeHighlighter *highlighter = static_cast<ElaCodeHighlighter *>(d->_highlighter);
	if (highlighter)
	{
		highlighter->setLanguage(d->_language);
	}
}

ElaCodeEditor::Language ElaCodeEditor::getLanguage() const
{
	return static_cast<Language>(d_ptr->_language);
}

ElaCodeEditor::ElaCodeEditor(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaCodeEditorPrivate())
{
	Q_D(ElaCodeEditor);
	d->q_ptr = this;
	d->_pIsReadOnly = false;
	d->_pTabSize = 4;
	setObjectName("ElaCodeEditor");

	d->_editor = new ElaCodePlainTextEdit(this);
	d->_editor->setFrameShape(QFrame::NoFrame);

	QFont monoFont;
#if defined(Q_OS_MAC)
	monoFont = QFont("Courier New", 13);
#elif defined(Q_OS_WIN)
	monoFont = QFont("Consolas", 13);
#else
	monoFont = QFont("Monospace", 13);
#endif
	d->_editor->setFont(monoFont);

	d->_editor->setTabStopDistance(d->_pTabSize * d->_editor->fontMetrics().horizontalAdvance(' '));

	ElaCodeEditorLineArea *lineArea = new ElaCodeEditorLineArea(d->_editor, d, this);
	d->_lineArea = lineArea;

	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(d->_lineArea);
	mainLayout->addWidget(d->_editor);

	d->_highlighter = new ElaCodeHighlighter(d->_editor->document(), eTheme->getThemeMode());

	d->_themeMode = eTheme->getThemeMode();
	d->_editor->setStyleSheet(QString("QPlainTextEdit { background-color: %1; color: %2; }")
		.arg(ElaThemeColor(d->_themeMode, BasicBase).name(QColor::HexArgb))
		.arg(ElaThemeColor(d->_themeMode, BasicText).name(QColor::HexArgb)));
	d->onThemeChanged(d->_themeMode);
	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaCodeEditorPrivate::onThemeChanged);

	auto updateLineAreaWidth = [d, lineArea]()
	{
		d->_lineArea->setFixedWidth(lineArea->lineAreaWidth());
	};
	updateLineAreaWidth();

	connect(d->_editor, &QPlainTextEdit::blockCountChanged, this, [updateLineAreaWidth]()
	{
		updateLineAreaWidth();
	});

	connect(d->_editor, &QPlainTextEdit::updateRequest, this, [d](const QRect &rect, int dy)
	{
		if (dy)
			d->_lineArea->scroll(0, dy);
		else
			d->_lineArea->update(0, rect.y(), d->_lineArea->width(), rect.height());
	});

	connect(d->_editor, &QPlainTextEdit::cursorPositionChanged, this, [d]()
	{
		QList<QTextEdit::ExtraSelection> extraSelections;
		if (!d->_editor->isReadOnly())
		{
			QTextEdit::ExtraSelection selection;
			selection.format.setBackground(ElaThemeColor(d->_themeMode, BasicHover));
			selection.format.setProperty(QTextFormat::FullWidthSelection, true);
			selection.cursor = d->_editor->textCursor();
			selection.cursor.clearSelection();
			extraSelections.append(selection);
		}
		d->_editor->setExtraSelections(extraSelections);
		d->_lineArea->update();
	});

	connect(d->_editor, &QPlainTextEdit::textChanged, this, [d]()
	{
		d->_pCode = d->_editor->toPlainText();
	});

	connect(this, &ElaCodeEditor::pIsReadOnlyChanged, this, [d]()
	{
		d->_editor->setReadOnly(d->_pIsReadOnly);
	});

	connect(this, &ElaCodeEditor::pTabSizeChanged, this, [d]()
	{
		d->_editor->setTabStopDistance(d->_pTabSize * d->_editor->fontMetrics().horizontalAdvance(' '));
	});
}

ElaCodeEditor::~ElaCodeEditor()
{
}