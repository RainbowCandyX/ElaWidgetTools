#include "ElaInfoBar.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaInfoBarPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, QString, Message)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, ElaInfoBarType::InfoBarSeverity, Severity)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, bool, IsClosable)
Q_PROPERTY_CREATE_Q_CPP(ElaInfoBar, ElaIconType::IconName, InfoBarIcon)

ElaInfoBar::ElaInfoBar(QWidget *parent) : QFrame{parent}, d_ptr(new ElaInfoBarPrivate())
{
	Q_D(ElaInfoBar);
	d->q_ptr = this;
	d->_pBorderRadius = 4;
	d->_pTitle = "";
	d->_pMessage = "";
	d->_pSeverity = ElaInfoBarType::Informational;
	d->_pIsClosable = true;
	d->_pInfoBarIcon = ElaIconType::None;
	d->_themeMode = eTheme->getThemeMode();
	setObjectName("ElaInfoBar");
	setMouseTracking(true);
	setMinimumHeight(48);
	setFrameShape(QFrame::NoFrame);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaInfoBar::ElaInfoBar(ElaInfoBarType::InfoBarSeverity severity, QWidget *parent)
	: ElaInfoBar(parent)
{
	Q_D(ElaInfoBar);
	d->_pSeverity = severity;
}

ElaInfoBar::~ElaInfoBar()
{
}

void ElaInfoBar::addAction(const QString &text, const std::function<void()> &callback)
{
	Q_D(ElaInfoBar);
	d->_actions.append({text, callback});
	update();
}

void ElaInfoBar::clearActions()
{
	Q_D(ElaInfoBar);
	d->_actions.clear();
	d->_actionButtonRects.clear();
	update();
}

void ElaInfoBar::closeInfoBar()
{
	Q_D(ElaInfoBar);
	Q_EMIT closeButtonClicked();
	d->_doCloseAnimation();
}

void ElaInfoBar::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaInfoBar);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QColor severityColor = d->_getSeverityColor();
	ElaIconType::IconName severityIcon = d->_getSeverityIcon();

	QColor bgColor;
	if (d->_themeMode == ElaThemeType::Light)
	{
		switch (d->_pSeverity)
		{
			case ElaInfoBarType::Success:
				bgColor = QColor(0xDF, 0xF6, 0xDD);
				break;
			case ElaInfoBarType::Warning:
				bgColor = QColor(0xFE, 0xF7, 0xE1);
				break;
			case ElaInfoBarType::Error:
				bgColor = QColor(0xFD, 0xE7, 0xE9);
				break;
			case ElaInfoBarType::Informational:
			default:
				bgColor = QColor(0xF6, 0xF6, 0xF6);
				break;
		}
	}
	else
	{
		switch (d->_pSeverity)
		{
			case ElaInfoBarType::Success:
				bgColor = QColor(0x39, 0x3D, 0x1B);
				break;
			case ElaInfoBarType::Warning:
				bgColor = QColor(0x43, 0x3D, 0x19);
				break;
			case ElaInfoBarType::Error:
				bgColor = QColor(0x44, 0x21, 0x26);
				break;
			case ElaInfoBarType::Informational:
			default:
				bgColor = QColor(0x2B, 0x2B, 0x2B);
				break;
		}
	}

	QPainterPath bgPath;
	bgPath.addRoundedRect(QRectF(rect()), d->_pBorderRadius, d->_pBorderRadius);
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
	painter.setBrush(bgColor);
	painter.drawPath(bgPath);

	QPainterPath stripPath;
	int stripWidth = 3;
	QRectF stripRect(0, 0, d->_pBorderRadius * 2 + stripWidth, height());
	stripPath.addRoundedRect(stripRect, d->_pBorderRadius, d->_pBorderRadius);
	QPainterPath clipPath;
	clipPath.addRect(QRectF(0, 0, stripWidth, height()));
	stripPath = stripPath.intersected(clipPath);
	painter.setPen(Qt::NoPen);
	painter.setBrush(severityColor);
	painter.drawPath(stripPath);

	int contentLeft = 16;
	int contentCenterY = height() / 2;

	QFont iconFont = QFont("ElaAwesome");
	iconFont.setPixelSize(16);
	painter.setFont(iconFont);
	painter.setPen(severityColor);
	QRect iconRect(contentLeft, contentCenterY - 10, 20, 20);
	painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) severityIcon));
	contentLeft += 28;

	int rightPadding = 12;
	int closeButtonSize = 20;
	int rightContentEnd = width() - rightPadding;

	if (d->_pIsClosable)
	{
		d->_closeButtonRect = QRect(rightContentEnd - closeButtonSize, contentCenterY - closeButtonSize / 2, closeButtonSize, closeButtonSize);
		rightContentEnd = d->_closeButtonRect.left() - 8;

		if (d->_closeButtonHover)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
			painter.drawRoundedRect(d->_closeButtonRect.adjusted(-2, -2, 2, 2), 3, 3);
		}

		QFont closeFont = QFont("ElaAwesome");
		closeFont.setPixelSize(10);
		painter.setFont(closeFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		painter.drawText(d->_closeButtonRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::Xmark));
	}

	d->_actionButtonRects.clear();
	if (!d->_actions.isEmpty())
	{
		QFont buttonFont = font();
		buttonFont.setPixelSize(12);
		QFontMetrics buttonFm(buttonFont);
		int buttonHeight = 26;
		int buttonSpacing = 6;
		int buttonPaddingH = 14;

		int buttonX = rightContentEnd;
		for (int i = d->_actions.size() - 1; i >= 0; --i)
		{
			int textWidth = buttonFm.horizontalAdvance(d->_actions[i].text);
			int buttonWidth = textWidth + buttonPaddingH * 2;
			buttonX -= buttonWidth;
			QRect btnRect(buttonX, contentCenterY - buttonHeight / 2, buttonWidth, buttonHeight);
			d->_actionButtonRects.prepend(btnRect);
			buttonX -= buttonSpacing;

			painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
			if (d->_hoverActionIndex == i)
			{
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicHover));
			}
			else
			{
				painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
			}
			painter.drawRoundedRect(btnRect, 4, 4);

			painter.setFont(buttonFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
			painter.drawText(btnRect, Qt::AlignCenter, d->_actions[i].text);
		}
		rightContentEnd = buttonX - 4;
	}

	int textAreaWidth = rightContentEnd - contentLeft;
	QFont titleFont = font();
	titleFont.setPixelSize(13);
	titleFont.setBold(true);
	QFontMetrics titleFm(titleFont);

	QFont messageFont = font();
	messageFont.setPixelSize(13);
	QFontMetrics messageFm(messageFont);

	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));

	if (!d->_pTitle.isEmpty() && !d->_pMessage.isEmpty())
	{
		QString elidedTitle = titleFm.elidedText(d->_pTitle, Qt::ElideRight, textAreaWidth);
		painter.setFont(titleFont);
		int titleWidth = titleFm.horizontalAdvance(elidedTitle);
		painter.drawText(QRect(contentLeft, contentCenterY - 10, titleWidth, 20), Qt::AlignVCenter | Qt::AlignLeft, elidedTitle);

		int messageLeft = contentLeft + titleWidth + 8;
		int messageWidth = rightContentEnd - messageLeft;
		if (messageWidth > 0)
		{
			painter.setFont(messageFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
			QString elidedMessage = messageFm.elidedText(d->_pMessage, Qt::ElideRight, messageWidth);
			painter.drawText(QRect(messageLeft, contentCenterY - 10, messageWidth, 20), Qt::AlignVCenter | Qt::AlignLeft, elidedMessage);
		}
	}
	else if (!d->_pTitle.isEmpty())
	{
		painter.setFont(titleFont);
		QString elidedTitle = titleFm.elidedText(d->_pTitle, Qt::ElideRight, textAreaWidth);
		painter.drawText(QRect(contentLeft, contentCenterY - 10, textAreaWidth, 20), Qt::AlignVCenter | Qt::AlignLeft, elidedTitle);
	}
	else if (!d->_pMessage.isEmpty())
	{
		painter.setFont(messageFont);
		QString elidedMessage = messageFm.elidedText(d->_pMessage, Qt::ElideRight, textAreaWidth);
		painter.drawText(QRect(contentLeft, contentCenterY - 10, textAreaWidth, 20), Qt::AlignVCenter | Qt::AlignLeft, elidedMessage);
	}
}

void ElaInfoBar::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaInfoBar);
	if (event->button() != Qt::LeftButton)
	{
		QFrame::mousePressEvent(event);
		return;
	}

	if (d->_pIsClosable && d->_closeButtonRect.contains(event->pos()))
	{
		closeInfoBar();
		return;
	}

	for (int i = 0; i < d->_actionButtonRects.size(); ++i)
	{
		if (d->_actionButtonRects[i].contains(event->pos()))
		{
			if (d->_actions[i].callback)
			{
				d->_actions[i].callback();
			}
			return;
		}
	}
	QFrame::mousePressEvent(event);
}

void ElaInfoBar::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaInfoBar);
	bool needUpdate = false;

	if (d->_pIsClosable)
	{
		bool hover = d->_closeButtonRect.contains(event->pos());
		if (hover != d->_closeButtonHover)
		{
			d->_closeButtonHover = hover;
			needUpdate = true;
		}
	}

	int hoverIndex = -1;
	for (int i = 0; i < d->_actionButtonRects.size(); ++i)
	{
		if (d->_actionButtonRects[i].contains(event->pos()))
		{
			hoverIndex = i;
			break;
		}
	}
	if (hoverIndex != d->_hoverActionIndex)
	{
		d->_hoverActionIndex = hoverIndex;
		needUpdate = true;
	}

	if (needUpdate)
	{
		setCursor(d->_closeButtonHover || hoverIndex >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor);
		update();
	}
	QFrame::mouseMoveEvent(event);
}

void ElaInfoBar::leaveEvent(QEvent *event)
{
	Q_D(ElaInfoBar);
	bool needUpdate = false;
	if (d->_closeButtonHover)
	{
		d->_closeButtonHover = false;
		needUpdate = true;
	}
	if (d->_hoverActionIndex != -1)
	{
		d->_hoverActionIndex = -1;
		needUpdate = true;
	}
	if (needUpdate)
	{
		setCursor(Qt::ArrowCursor);
		update();
	}
	QFrame::leaveEvent(event);
}