#include "ElaDivider.h"

#include <QFontMetrics>
#include <QPainter>

#include "ElaTheme.h"
#include "private/ElaDividerPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaDivider, Qt::Orientation, Orientation)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaDivider, QString, Text)
Q_PROPERTY_CREATE_Q_CPP(ElaDivider, int, ContentPosition)

ElaDividerPrivate::ElaDividerPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaDividerPrivate::~ElaDividerPrivate()
{
}

ElaDivider::ElaDivider(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaDividerPrivate())
{
	Q_D(ElaDivider);
	d->q_ptr = this;
	d->_pOrientation = Qt::Horizontal;
	d->_pText = "";
	d->_pContentPosition = Center;
	setObjectName("ElaDivider");
	setFixedHeight(20);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	connect(this, &ElaDivider::pOrientationChanged, this, [=]()
	{
		if (d->_pOrientation == Qt::Horizontal)
		{
			setFixedHeight(20);
			setMinimumWidth(0);
			setMaximumWidth(QWIDGETSIZE_MAX);
			setMaximumHeight(20);
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		}
		else
		{
			setFixedWidth(20);
			setMinimumHeight(0);
			setMaximumHeight(QWIDGETSIZE_MAX);
			setMaximumWidth(20);
			setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
		}
		updateGeometry();
		update();
	});
	connect(this, &ElaDivider::pTextChanged, this, [=]()
	{
		update();
	});
	connect(this, &ElaDivider::pContentPositionChanged, this, [=]()
	{
		update();
	});
}

ElaDivider::ElaDivider(const QString &text, QWidget *parent)
	: ElaDivider(parent)
{
	Q_D(ElaDivider);
	d->_pText = text;
}

ElaDivider::~ElaDivider()
{
}

void ElaDivider::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaDivider);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QColor lineColor = ElaThemeColor(d->_themeMode, BasicBorder);
	QPen linePen(lineColor, 1);

	if (d->_pOrientation == Qt::Vertical)
	{
		// Vertical line at horizontal center
		int centerX = width() / 2;
		painter.setPen(linePen);
		painter.drawLine(centerX, 0, centerX, height());
		return;
	}

	// Horizontal mode
	int centerY = height() / 2;

	if (d->_pText.isEmpty())
	{
		// Simple horizontal line
		painter.setPen(linePen);
		painter.drawLine(0, centerY, width(), centerY);
		return;
	}

	// Draw text with gap in line
	QFont textFont = this->font();
	textFont.setPixelSize(12);
	painter.setFont(textFont);
	QFontMetrics fm(textFont);
	int textWidth = fm.horizontalAdvance(d->_pText);
	int textHeight = fm.height();
	int padding = 8;

	int textX = 0;
	if (d->_pContentPosition == Left)
	{
		textX = 20;
	}
	else if (d->_pContentPosition == Center)
	{
		textX = (width() - textWidth) / 2;
	}
	else
	{
		// Right
		textX = width() - 20 - textWidth;
	}

	int gapLeft = textX - padding;
	int gapRight = textX + textWidth + padding;

	// Draw left line segment
	painter.setPen(linePen);
	if (gapLeft > 0)
	{
		painter.drawLine(0, centerY, gapLeft, centerY);
	}

	// Draw right line segment
	if (gapRight < width())
	{
		painter.drawLine(gapRight, centerY, width(), centerY);
	}

	// Draw text
	QColor textColor = ElaThemeColor(d->_themeMode, BasicDetailsText);
	painter.setPen(textColor);
	QRectF textRect(textX, (height() - textHeight) / 2, textWidth, textHeight);
	painter.drawText(textRect, Qt::AlignCenter, d->_pText);
}

QSize ElaDivider::sizeHint() const
{
	Q_D(const ElaDivider);
	if (d->_pOrientation == Qt::Horizontal)
	{
		return QSize(200, 20);
	}
	return QSize(20, 200);
}