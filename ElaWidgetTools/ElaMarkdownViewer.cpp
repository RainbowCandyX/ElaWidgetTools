#include "ElaMarkdownViewer.h"

#include <QTextBrowser>
#include <QVBoxLayout>

#include "ElaMarkdownViewerPrivate.h"
#include "ElaTheme.h"

Q_PROPERTY_REF_CREATE_Q_CPP(ElaMarkdownViewer, QString, Markdown)
Q_PROPERTY_CREATE_Q_CPP(ElaMarkdownViewer, int, BorderRadius)

ElaMarkdownViewer::ElaMarkdownViewer(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaMarkdownViewerPrivate())
{
	Q_D(ElaMarkdownViewer);
	d->q_ptr = this;
	d->_pBorderRadius = 0;
	setObjectName("ElaMarkdownViewer");

	d->_textBrowser = new QTextBrowser(this);
	d->_textBrowser->setFrameShape(QFrame::NoFrame);
	d->_textBrowser->setReadOnly(true);
	d->_textBrowser->setOpenExternalLinks(true);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(d->_textBrowser);

	d->_themeMode = eTheme->getThemeMode();
	d->_applyThemeStyle();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		d->_applyThemeStyle();
	});
	connect(this, &ElaMarkdownViewer::pMarkdownChanged, this, [=]()
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		d->_textBrowser->setMarkdown(d->_pMarkdown);
#else
		QString html = d->_pMarkdown;
		html.replace("\n\n", "<br/><br/>");
		d->_textBrowser->setHtml(html);
#endif
	});
}

ElaMarkdownViewer::~ElaMarkdownViewer()
{
}


ElaMarkdownViewerPrivate::ElaMarkdownViewerPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaMarkdownViewerPrivate::~ElaMarkdownViewerPrivate()
{
}

void ElaMarkdownViewerPrivate::_applyThemeStyle()
{
	if (!_textBrowser)
	{
		return;
	}
	QColor textColor = ElaThemeColor(_themeMode, BasicText);
	QColor linkColor = ElaThemeColor(_themeMode, PrimaryNormal);
	QColor codeBackground = ElaThemeColor(_themeMode, BasicBaseDeep);
	QString styleSheet = QString(
				"QTextBrowser {"
				"    background-color: transparent;"
				"    color: %1;"
				"}"
				"a { color: %2; }"
				"code, pre {"
				"    background-color: %3;"
				"}")
			.arg(textColor.name(), linkColor.name(), codeBackground.name());
	_textBrowser->setStyleSheet(styleSheet);
}