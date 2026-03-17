#include "ElaTag.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaTagPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaTag, int, BorderRadius)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaTag, QString, TagText)
Q_PROPERTY_CREATE_Q_CPP(ElaTag, bool, IsClosable)
Q_PROPERTY_CREATE_Q_CPP(ElaTag, bool, IsCheckable)
Q_PROPERTY_CREATE_Q_CPP(ElaTag, bool, IsChecked)

ElaTagPrivate::ElaTagPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaTagPrivate::~ElaTagPrivate()
{
}

QColor ElaTagPrivate::_getBackgroundColor() const
{
	switch (_tagColor)
	{
		case ElaTag::Default:
			return ElaThemeColor(_themeMode, BasicBase);
		case ElaTag::Primary:
		{
			QColor c = ElaThemeColor(_themeMode, PrimaryNormal);
			c.setAlpha(30);
			return c;
		}
		case ElaTag::Success:
		{
			QColor c(0x0F, 0x7B, 0x0F);
			c.setAlpha(30);
			return c;
		}
		case ElaTag::Warning:
		{
			QColor c(0xF7, 0x93, 0x0E);
			c.setAlpha(30);
			return c;
		}
		case ElaTag::Danger:
		{
			QColor c = ElaThemeColor(_themeMode, StatusDanger);
			c.setAlpha(30);
			return c;
		}
	}
	return ElaThemeColor(_themeMode, BasicBase);
}

QColor ElaTagPrivate::_getForegroundColor() const
{
	switch (_tagColor)
	{
		case ElaTag::Default:
			return ElaThemeColor(_themeMode, BasicText);
		case ElaTag::Primary:
			return ElaThemeColor(_themeMode, PrimaryNormal);
		case ElaTag::Success:
			return QColor(0x0F, 0x7B, 0x0F);
		case ElaTag::Warning:
			return QColor(0xF7, 0x93, 0x0E);
		case ElaTag::Danger:
			return ElaThemeColor(_themeMode, StatusDanger);
	}
	return ElaThemeColor(_themeMode, BasicText);
}

ElaTag::ElaTag(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaTagPrivate())
{
	Q_D(ElaTag);
	d->q_ptr = this;
	d->_pBorderRadius = 4;
	d->_pTagText = "";
	d->_pIsClosable = false;
	d->_pIsCheckable = false;
	d->_pIsChecked = false;
	setObjectName("ElaTag");
	setFixedHeight(28);
	setMouseTracking(true);
	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	connect(this, &ElaTag::pTagTextChanged, this, [=]()
	{
		updateGeometry();
		adjustSize();
		update();
	});
	connect(this, &ElaTag::pIsClosableChanged, this, [=]()
	{
		updateGeometry();
		adjustSize();
		update();
	});
	connect(this, &ElaTag::pIsCheckableChanged, this, [=]()
	{
		updateGeometry();
		adjustSize();
		update();
	});
	connect(this, &ElaTag::pIsCheckedChanged, this, [=]()
	{
		updateGeometry();
		adjustSize();
		update();
	});
}

ElaTag::ElaTag(const QString &text, QWidget *parent)
	: ElaTag(parent)
{
	Q_D(ElaTag);
	d->_pTagText = text;
}

ElaTag::~ElaTag()
{
}

void ElaTag::setTagColor(TagColor color)
{
	Q_D(ElaTag);
	d->_tagColor = color;
	update();
}

ElaTag::TagColor ElaTag::getTagColor() const
{
	return d_ptr->_tagColor;
}

void ElaTag::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaTag);
	if (event->button() == Qt::LeftButton)
	{
		if (d->_pIsClosable)
		{
			// Check if click is in the close button area (right side)
			int closeAreaX = width() - d->_closeButtonWidth - d->_padding / 2;
			if (event->pos().x() >= closeAreaX)
			{
				Q_EMIT closed();
				return;
			}
		}
		if (d->_pIsCheckable)
		{
			setIsChecked(!d->_pIsChecked);
			Q_EMIT checkedChanged(d->_pIsChecked);
		}
		Q_EMIT clicked();
	}
	QWidget::mousePressEvent(event);
}

void ElaTag::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaTag);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QColor bgColor = d->_getBackgroundColor();
	QColor fgColor = d->_getForegroundColor();

	// When checkable and checked, override to primary color
	if (d->_pIsCheckable && d->_pIsChecked)
	{
		QColor primaryColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
		bgColor = primaryColor;
		bgColor.setAlpha(30);
		fgColor = primaryColor;
	}

	// Draw background rounded rectangle
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	QPainterPath path;
	path.addRoundedRect(QRectF(0, 0, width(), height()), d->_pBorderRadius, d->_pBorderRadius);
	painter.drawPath(path);

	// Calculate text draw area
	int xOffset = d->_padding;

	// Draw check mark if checkable and checked
	if (d->_pIsCheckable && d->_pIsChecked)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(12);
		painter.setFont(iconFont);
		painter.setPen(fgColor);
		QRectF checkRect(xOffset, 0, d->_checkIconWidth, height());
		painter.drawText(checkRect, Qt::AlignCenter, QChar((unsigned short) 0xea6c));
		xOffset += d->_checkIconWidth;
	}

	// Draw text
	QFont textFont = this->font();
	textFont.setPixelSize(13);
	painter.setFont(textFont);
	painter.setPen(fgColor);
	int textWidth = width() - xOffset - d->_padding;
	if (d->_pIsClosable)
	{
		textWidth -= d->_closeButtonWidth;
	}
	QRectF textRect(xOffset, 0, textWidth, height());
	painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, d->_pTagText);

	// Draw close button if closable
	if (d->_pIsClosable)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(10);
		painter.setFont(iconFont);
		painter.setPen(fgColor);
		int closeX = width() - d->_closeButtonWidth - d->_padding / 2;
		QRectF closeRect(closeX, 0, d->_closeButtonWidth, height());
		painter.drawText(closeRect, Qt::AlignCenter, QChar((unsigned short) 0xf4ce));
	}
}

QSize ElaTag::sizeHint() const
{
	Q_D(const ElaTag);
	QFont textFont = this->font();
	textFont.setPixelSize(13);
	QFontMetrics fm(textFont);
	int textWidth = fm.horizontalAdvance(d->_pTagText);
	int w = textWidth + d->_padding * 2;
	if (d->_pIsClosable)
	{
		w += d->_closeButtonWidth;
	}
	if (d->_pIsCheckable && d->_pIsChecked)
	{
		w += d->_checkIconWidth;
	}
	return QSize(qMax(w, 32), 28);
}