#include "ElaMessageDialog.h"

#include <QPainter>
#include <QPainterPath>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaTheme.h"
#include "private/ElaMessageDialogPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaMessageDialog, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageDialog, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageDialog, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageDialog, int, TitlePixelSize)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageDialog, int, ContentPixelSize)

ElaMessageDialog::ElaMessageDialog(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaMessageDialogPrivate())
{
	Q_D(ElaMessageDialog);
	d->q_ptr = this;

	d->_pBorderRadius = 8;
	d->_pTitle = "标题";
	d->_pContent = "";
	d->_pTitlePixelSize = 15;
	d->_pContentPixelSize = 13;

	setMinimumSize(280, 150);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});

	d->_confirmButton = new ElaMessageDialogButton(ElaMessageDialogButton::Confirm, this);
	d->_confirmButton->setFixedHeight(40);
	d->_cancelButton = new ElaMessageDialogButton(ElaMessageDialogButton::Cancel, this);
	d->_cancelButton->setFixedHeight(40);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	mainLayout->addStretch();

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->addWidget(d->_confirmButton, 1);
	buttonLayout->addWidget(d->_cancelButton, 1);

	mainLayout->addLayout(buttonLayout);

	connect(d->_confirmButton, &ElaMessageDialogButton::clicked, this, [=]()
	{
		Q_EMIT confirmed();
		hide();
	});
	connect(d->_cancelButton, &ElaMessageDialogButton::clicked, this, [=]()
	{
		Q_EMIT cancelled();
		hide();
	});
}

ElaMessageDialog::~ElaMessageDialog()
{
}

void ElaMessageDialog::paintEvent(QPaintEvent *event)
{
	Q_D(ElaMessageDialog);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter.setPen(Qt::NoPen);
	painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
	painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);

	QFont titleFont = this->font();
	titleFont.setPixelSize(d->_pTitlePixelSize);
	titleFont.setWeight(QFont::Bold);
	painter.setFont(titleFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	painter.drawText(QRect(15, 15, width() - 30, 25), Qt::AlignLeft | Qt::AlignTop, d->_pTitle);

	QFont contentFont = this->font();
	contentFont.setPixelSize(d->_pContentPixelSize);
	contentFont.setWeight(QFont::Normal);
	painter.setFont(contentFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	int contentHeight = height() - 40 - 45 - 15;
	painter.drawText(QRect(15, 45, width() - 30, contentHeight), Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, d->_pContent);

	painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
	painter.drawLine(0, height() - 40, width(), height() - 40);
}