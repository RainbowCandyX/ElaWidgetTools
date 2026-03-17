#include "ElaSplitButton.h"

#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaMenu.h"
#include "ElaTheme.h"
#include "private/ElaSplitButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaSplitButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSplitButton, QString, Text)
Q_PROPERTY_CREATE_Q_CPP(ElaSplitButton, ElaIconType::IconName, ElaIcon)

ElaSplitButton::ElaSplitButton(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaSplitButtonPrivate())
{
	Q_D(ElaSplitButton);
	d->q_ptr = this;
	d->_pBorderRadius = 3;
	d->_pText = QString();
	d->_pElaIcon = ElaIconType::None;
	d->_themeMode = eTheme->getThemeMode();
	setMouseTracking(true);
	setFixedHeight(35);
	QFont font = this->font();
	font.setPixelSize(15);
	setFont(font);
	setObjectName("ElaSplitButton");
	setStyleSheet("#ElaSplitButton{background-color:transparent;}");
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaSplitButton::~ElaSplitButton()
{
}

void ElaSplitButton::setMenu(ElaMenu *menu)
{
	Q_D(ElaSplitButton);
	d->_menu = menu;
}

ElaMenu *ElaSplitButton::getMenu() const
{
	Q_D(const ElaSplitButton);
	return d->_menu;
}

void ElaSplitButton::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaSplitButton);
	if (event->button() == Qt::LeftButton)
	{
		int splitX = width() - d->_dropdownWidth;
		if (event->pos().x() < splitX)
		{
			d->_isLeftPressed = true;
		}
		else
		{
			d->_isRightPressed = true;
		}
		update();
	}
	QWidget::mousePressEvent(event);
}

void ElaSplitButton::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaSplitButton);
	if (event->button() == Qt::LeftButton)
	{
		int splitX = width() - d->_dropdownWidth;
		if (d->_isLeftPressed && event->pos().x() < splitX)
		{
			Q_EMIT clicked();
		}
		else if (d->_isRightPressed && event->pos().x() >= splitX)
		{
			if (d->_menu)
			{
				QPoint pos = mapToGlobal(QPoint(0, height()));
				d->_menu->popup(pos);
			}
		}
		d->_isLeftPressed = false;
		d->_isRightPressed = false;
		update();
	}
	QWidget::mouseReleaseEvent(event);
}

void ElaSplitButton::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaSplitButton);
	int splitX = width() - d->_dropdownWidth;
	bool leftHovered = event->pos().x() < splitX;
	bool rightHovered = !leftHovered;
	if (d->_isLeftHovered != leftHovered || d->_isRightHovered != rightHovered)
	{
		d->_isLeftHovered = leftHovered;
		d->_isRightHovered = rightHovered;
		update();
	}
	QWidget::mouseMoveEvent(event);
}

void ElaSplitButton::leaveEvent(QEvent *event)
{
	Q_D(ElaSplitButton);
	d->_isLeftHovered = false;
	d->_isRightHovered = false;
	d->_isLeftPressed = false;
	d->_isRightPressed = false;
	update();
	QWidget::leaveEvent(event);
}

void ElaSplitButton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaSplitButton);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.save();

	int borderRadius = d->_pBorderRadius;
	int splitX = width() - d->_dropdownWidth;
	QRect leftRect(0, 0, splitX, height());
	QRect rightRect(splitX, 0, d->_dropdownWidth, height());
	QRect fullRect(0, 0, width(), height());

	QColor baseColor = ElaThemeColor(d->_themeMode, BasicBase);
	QColor hoverColor = ElaThemeColor(d->_themeMode, BasicHover);
	QColor pressColor = ElaThemeColor(d->_themeMode, BasicPress);
	QColor textColor = ElaThemeColor(d->_themeMode, BasicText);
	QColor borderColor = ElaThemeColor(d->_themeMode, BasicBorder);

	// Determine left and right fill colors
	QColor leftColor = baseColor;
	if (d->_isLeftPressed)
	{
		leftColor = pressColor;
	}
	else if (d->_isLeftHovered)
	{
		leftColor = hoverColor;
	}

	QColor rightColor = baseColor;
	if (d->_isRightPressed)
	{
		rightColor = pressColor;
	}
	else if (d->_isRightHovered)
	{
		rightColor = hoverColor;
	}

	// Draw full rounded rect background with base color
	QPainterPath fullPath;
	fullPath.addRoundedRect(fullRect, borderRadius, borderRadius);
	painter.setClipPath(fullPath);

	// Draw left area
	painter.setPen(Qt::NoPen);
	painter.setBrush(leftColor);
	painter.drawRect(leftRect);

	// Draw right area
	painter.setBrush(rightColor);
	painter.drawRect(rightRect);

	// Reset clip and draw border
	painter.setClipPath(fullPath);
	if (d->_themeMode == ElaThemeType::Light)
	{
		painter.setPen(borderColor);
	}
	else
	{
		painter.setPen(Qt::NoPen);
	}
	painter.setBrush(Qt::NoBrush);
	painter.drawRoundedRect(fullRect, borderRadius, borderRadius);

	// Draw vertical separator line
	painter.setPen(QPen(borderColor, 1));
	painter.drawLine(splitX, 4, splitX, height() - 4);

	// Draw left area content: icon + text
	painter.setPen(textColor);
	if (d->_pElaIcon != ElaIconType::None)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(18);
		QFontMetrics iconFm(iconFont);
		int iconWidth = iconFm.horizontalAdvance(QChar((unsigned short) d->_pElaIcon));

		QFont textFont = this->font();
		QFontMetrics textFm(textFont);
		int textWidth = d->_pText.isEmpty() ? 0 : textFm.horizontalAdvance(d->_pText);

		int spacing = d->_pText.isEmpty() ? 0 : 8;
		int totalWidth = iconWidth + spacing + textWidth;
		int startX = leftRect.x() + (leftRect.width() - totalWidth) / 2;

		painter.setFont(iconFont);
		painter.drawText(QRect(startX, leftRect.y(), iconWidth, leftRect.height()), Qt::AlignCenter, QChar((unsigned short) d->_pElaIcon));

		if (!d->_pText.isEmpty())
		{
			painter.setFont(textFont);
			painter.drawText(QRect(startX + iconWidth + spacing, leftRect.y(), textWidth, leftRect.height()), Qt::AlignVCenter | Qt::AlignLeft, d->_pText);
		}
	}
	else
	{
		painter.setFont(this->font());
		painter.drawText(leftRect, Qt::AlignCenter, d->_pText);
	}

	// Draw right area: AngleDown icon
	QFont arrowFont = QFont("ElaAwesome");
	arrowFont.setPixelSize(14);
	painter.setFont(arrowFont);
	painter.setPen(textColor);
	painter.drawText(rightRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::AngleDown));

	painter.restore();
}

QSize ElaSplitButton::sizeHint() const
{
	Q_D(const ElaSplitButton);
	QFontMetrics fm(font());
	int textWidth = d->_pText.isEmpty() ? 0 : fm.horizontalAdvance(d->_pText);
	int iconWidth = (d->_pElaIcon != ElaIconType::None) ? 24 : 0;
	int spacing = (!d->_pText.isEmpty() && d->_pElaIcon != ElaIconType::None) ? 8 : 0;
	int leftWidth = iconWidth + spacing + textWidth + 20;
	return QSize(leftWidth + d->_dropdownWidth, 35);
}

ElaSplitButtonPrivate::ElaSplitButtonPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaSplitButtonPrivate::~ElaSplitButtonPrivate()
{
}