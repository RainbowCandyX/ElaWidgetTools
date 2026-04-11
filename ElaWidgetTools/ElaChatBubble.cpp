#include "ElaChatBubble.h"

#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>

#include "ElaTheme.h"
#include "private/ElaChatBubblePrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaChatBubble, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaChatBubble, int, AvatarSize)
Q_PROPERTY_CREATE_Q_CPP(ElaChatBubble, int, MaxBubbleWidth)

void ElaChatBubble::setMessageText(const QString &text)
{
	Q_D(ElaChatBubble);
	d->_pMessageText = text;
	setFixedHeight(sizeHint().height());
	update();
	Q_EMIT pMessageTextChanged();
}

const QString &ElaChatBubble::getMessageText() const
{
	return d_ptr->_pMessageText;
}

void ElaChatBubble::setSenderName(const QString &name)
{
	Q_D(ElaChatBubble);
	d->_pSenderName = name;
	setFixedHeight(sizeHint().height());
	update();
	Q_EMIT pSenderNameChanged();
}

const QString &ElaChatBubble::getSenderName() const
{
	return d_ptr->_pSenderName;
}

void ElaChatBubble::setTimestamp(const QString &ts)
{
	Q_D(ElaChatBubble);
	d->_pTimestamp = ts;
	setFixedHeight(sizeHint().height());
	update();
	Q_EMIT pTimestampChanged();
}

const QString &ElaChatBubble::getTimestamp() const
{
	return d_ptr->_pTimestamp;
}

ElaChatBubble::ElaChatBubble(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaChatBubblePrivate())
{
	Q_D(ElaChatBubble);
	d->q_ptr = this;
	d->_pBorderRadius = 12;
	d->_pAvatarSize = 40;
	d->_pMaxBubbleWidth = 320;
	setObjectName("ElaChatBubble");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaChatBubble::~ElaChatBubble()
{
}

void ElaChatBubble::setDirection(BubbleDirection direction)
{
	Q_D(ElaChatBubble);
	d->_direction = direction;
	update();
}

ElaChatBubble::BubbleDirection ElaChatBubble::getDirection() const
{
	return d_ptr->_direction;
}

void ElaChatBubble::setStatus(MessageStatus status)
{
	Q_D(ElaChatBubble);
	d->_status = status;
	update();
}

ElaChatBubble::MessageStatus ElaChatBubble::getStatus() const
{
	return d_ptr->_status;
}

void ElaChatBubble::setAvatar(const QPixmap &avatar)
{
	Q_D(ElaChatBubble);
	d->_avatar = avatar;
	update();
}

QPixmap ElaChatBubble::getAvatar() const
{
	return d_ptr->_avatar;
}

void ElaChatBubble::setBubbleColor(const QColor &color)
{
	Q_D(ElaChatBubble);
	d->_bubbleColor = color;
	update();
}

QColor ElaChatBubble::getBubbleColor() const
{
	return d_ptr->_bubbleColor;
}

QSize ElaChatBubble::sizeHint() const
{
	Q_D(const ElaChatBubble);
	int padding = 10;
	int avatarSpace = d->_pAvatarSize + 10;
	int nameHeight = d->_pSenderName.isEmpty() ? 0 : 20;
	int timestampHeight = d->_pTimestamp.isEmpty() ? 0 : 16;

	QFont msgFont = font();
	msgFont.setPixelSize(14);
	QFontMetrics fm(msgFont);
	int bubbleContentWidth = qMin(fm.horizontalAdvance(d->_pMessageText) + 24, d->_pMaxBubbleWidth);
	QRect textRect = fm.boundingRect(0, 0, bubbleContentWidth - 24, 10000, Qt::TextWordWrap, d->_pMessageText);

	int totalHeight = padding + nameHeight + textRect.height() + 24 + timestampHeight + padding;
	int totalWidth = avatarSpace + bubbleContentWidth + padding * 2;
	return QSize(totalWidth, totalHeight);
}

void ElaChatBubble::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaChatBubble);

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	int padding = 10;
	int avatarSpace = d->_pAvatarSize + 10;
	bool isRight = (d->_direction == Right);

	int avatarX = isRight ? (width() - padding - d->_pAvatarSize) : padding;
	int avatarY = padding;
	if (!d->_avatar.isNull())
	{
		QPainterPath clipPath;
		clipPath.addEllipse(avatarX, avatarY, d->_pAvatarSize, d->_pAvatarSize);
		painter.save();
		painter.setClipPath(clipPath);
		painter.drawPixmap(avatarX, avatarY, d->_pAvatarSize, d->_pAvatarSize, d->_avatar);
		painter.restore();
	}
	else
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseDeep));
		painter.drawEllipse(avatarX, avatarY, d->_pAvatarSize, d->_pAvatarSize);
		QFont iconFont("ElaAwesome");
		iconFont.setPixelSize(d->_pAvatarSize / 2);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		painter.drawText(QRect(avatarX, avatarY, d->_pAvatarSize, d->_pAvatarSize), Qt::AlignCenter,
		                 QChar(static_cast<int>(ElaIconType::User)));
	}

	int bubbleX = isRight ? padding : (padding + avatarSpace);
	int bubbleMaxWidth = width() - padding * 2 - avatarSpace;
	int currentY = padding;

	if (!d->_pSenderName.isEmpty())
	{
		QFont nameFont = font();
		nameFont.setPixelSize(12);
		nameFont.setBold(true);
		painter.setFont(nameFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		int nameX = bubbleX;
		if (isRight)
		{
			QFontMetrics nameFm(nameFont);
			nameX = bubbleX + bubbleMaxWidth - nameFm.horizontalAdvance(d->_pSenderName);
		}
		painter.drawText(nameX, currentY + 12, d->_pSenderName);
		currentY += 20;
	}

	QFont msgFont = font();
	msgFont.setPixelSize(14);
	QFontMetrics fm(msgFont);
	int textPadding = 12;
	int maxTextWidth = qMin(d->_pMaxBubbleWidth - textPadding * 2, bubbleMaxWidth - textPadding * 2);
	QRect textBound = fm.boundingRect(0, 0, maxTextWidth, 10000, Qt::TextWordWrap, d->_pMessageText);

	int bubbleWidth = textBound.width() + textPadding * 2;
	int bubbleHeight = textBound.height() + textPadding * 2;

	int bx = isRight ? (bubbleX + bubbleMaxWidth - bubbleWidth) : bubbleX;

	QColor bgColor;
	if (d->_bubbleColor.isValid())
	{
		bgColor = d->_bubbleColor;
	}
	else if (isRight)
	{
		bgColor = ElaThemeColor(d->_themeMode, PrimaryNormal);
	}
	else
	{
		bgColor = ElaThemeColor(d->_themeMode, BasicBase);
	}

	QPainterPath bubblePath;
	bubblePath.addRoundedRect(QRectF(bx, currentY, bubbleWidth, bubbleHeight),
	                          d->_pBorderRadius, d->_pBorderRadius);
	painter.setPen(Qt::NoPen);
	painter.fillPath(bubblePath, bgColor);

	if (!isRight)
	{
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
		painter.drawPath(bubblePath);
	}

	painter.setFont(msgFont);
	if (isRight)
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicTextInvert));
	}
	else
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	}
	painter.drawText(QRect(bx + textPadding, currentY + textPadding, maxTextWidth, textBound.height()),
	                 Qt::TextWordWrap, d->_pMessageText);

	currentY += bubbleHeight + 4;

	QFont timeFont = font();
	timeFont.setPixelSize(11);
	painter.setFont(timeFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));

	QString bottomText;
	if (!d->_pTimestamp.isEmpty())
	{
		bottomText = d->_pTimestamp;
	}

	if (d->_status != None)
	{
		QString statusText;
		switch (d->_status)
		{
			case Sending:
				statusText = "发送中...";
				break;
			case Sent:
				statusText = "已发送";
				break;
			case Read:
				statusText = "已读";
				break;
			case Failed:
				statusText = "发送失败";
				break;
			default:
				break;
		}
		if (!bottomText.isEmpty())
		{
			bottomText += "  ";
		}
		bottomText += statusText;
	}

	if (!bottomText.isEmpty())
	{
		QFontMetrics timeFm(timeFont);
		if (isRight)
		{
			int textW = timeFm.horizontalAdvance(bottomText);
			painter.drawText(bx + bubbleWidth - textW, currentY + 12, bottomText);
		}
		else
		{
			painter.drawText(bx, currentY + 12, bottomText);
		}

		if (d->_status == Failed)
		{
			painter.setPen(ElaThemeColor(d->_themeMode, StatusDanger));
			QString failText = "发送失败";
			if (isRight)
			{
				int failW = timeFm.horizontalAdvance(failText);
				painter.drawText(bx + bubbleWidth - failW, currentY + 12, failText);
			}
			else
			{
				int offset = timeFm.horizontalAdvance(bottomText) - timeFm.horizontalAdvance(failText);
				painter.drawText(bx + offset, currentY + 12, failText);
			}
		}
	}
}

ElaChatBubblePrivate::ElaChatBubblePrivate(QObject *parent)
	: QObject(parent)
{
}

ElaChatBubblePrivate::~ElaChatBubblePrivate()
{
}