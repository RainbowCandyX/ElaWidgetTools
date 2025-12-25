#include "ElaMessageCard.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaMessageCardPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, bool, IsCompleted)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, int, TitlePixelSize)
Q_PROPERTY_CREATE_Q_CPP(ElaMessageCard, int, ContentPixelSize)

ElaMessageCard::ElaMessageCard(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaMessageCardPrivate())
{
	Q_D(ElaMessageCard);
	d->q_ptr = this;

	d->_pBorderRadius = 8;
	d->_pIsCompleted = false;
	d->_pTitle = "信息";
	d->_pContent = "";
	d->_pTitlePixelSize = 15;
	d->_pContentPixelSize = 13;

	setMinimumSize(250, 75);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaMessageCard::~ElaMessageCard()
{
}

void ElaMessageCard::paintEvent(QPaintEvent *event)
{
	Q_D(ElaMessageCard);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int bubbleLeft = 5;
	int bubbleTop = 5;
	int bubbleWidth = width() - 10;
	int bubbleHeight = height() - 10;

	painter.save();
	QPainterPath bubblePath;
	bubblePath.addRoundedRect(QRectF(bubbleLeft, bubbleTop, bubbleWidth, bubbleHeight), d->_pBorderRadius,
	                          d->_pBorderRadius);

	painter.setPen(Qt::NoPen);
	painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
	painter.drawPath(bubblePath);

	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
	painter.drawPath(bubblePath);
	painter.restore();

	if (d->_pIsCompleted)
	{
		int iconLeft = bubbleLeft + 15;
		int iconTop = bubbleTop + height() / 2 - 5;

		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(0, 180, 100));
		painter.drawEllipse(QPoint(iconLeft, iconTop), 11, 11);

		painter.setPen(QPen(Qt::white, 2));
		painter.drawLine(QPoint(iconLeft - 4, iconTop), QPoint(iconLeft - 2, iconTop + 3));
		painter.drawLine(QPoint(iconLeft - 2, iconTop + 3), QPoint(iconLeft + 4, iconTop - 3));
		painter.restore();
	}

	QFont titleFont = this->font();
	titleFont.setPixelSize(d->_pTitlePixelSize);
	titleFont.setWeight(QFont::Bold);
	painter.setFont(titleFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	painter.drawText(QRect(bubbleLeft + 40, bubbleTop + 8, bubbleWidth - 50, 22), Qt::AlignLeft | Qt::AlignVCenter, d->_pTitle);

	QFont contentFont = this->font();
	contentFont.setPixelSize(d->_pContentPixelSize);
	contentFont.setWeight(QFont::Normal);
	painter.setFont(contentFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	painter.drawText(QRect(bubbleLeft + 40, bubbleTop + 32, bubbleWidth - 50, bubbleHeight - 40), Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, d->_pContent);
}