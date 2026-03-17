#include "ElaDropDownButton.h"

#include <QEvent>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "ElaMenu.h"
#include "ElaTheme.h"
#include "private/ElaDropDownButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaDropDownButton, int, BorderRadius)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaDropDownButton, QString, Text)
Q_PROPERTY_CREATE_Q_CPP(ElaDropDownButton, ElaIconType::IconName, ElaIcon)

ElaDropDownButton::ElaDropDownButton(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaDropDownButtonPrivate())
{
    Q_D(ElaDropDownButton);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pElaIcon = ElaIconType::None;
    setObjectName("ElaDropDownButton");
    setFixedHeight(35);
    setMouseTracking(true);
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaDropDownButton::~ElaDropDownButton()
{
}

void ElaDropDownButton::setMenu(ElaMenu* menu)
{
    Q_D(ElaDropDownButton);
    d->_menu = menu;
}

ElaMenu* ElaDropDownButton::getMenu() const
{
    return d_ptr->_menu;
}

bool ElaDropDownButton::event(QEvent* event)
{
    Q_D(ElaDropDownButton);
    switch (event->type())
    {
    case QEvent::HoverEnter:
    {
        d->_isHover = true;
        update();
        break;
    }
    case QEvent::HoverLeave:
    {
        d->_isHover = false;
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void ElaDropDownButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaDropDownButton);
    if (event->button() == Qt::LeftButton)
    {
        d->_isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void ElaDropDownButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaDropDownButton);
    if (event->button() == Qt::LeftButton)
    {
        d->_isPressed = false;
        update();
        if (d->_menu && rect().contains(event->pos()))
        {
            d->_menu->popup(mapToGlobal(QPoint(0, height())));
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void ElaDropDownButton::leaveEvent(QEvent* event)
{
    Q_D(ElaDropDownButton);
    d->_isHover = false;
    d->_isPressed = false;
    update();
    QWidget::leaveEvent(event);
}

void ElaDropDownButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    Q_D(ElaDropDownButton);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // Background
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->_pBorderRadius, d->_pBorderRadius);
    if (d->_isPressed)
    {
        painter.fillPath(backgroundPath, ElaThemeColor(d->_themeMode, BasicPress));
    }
    else if (d->_isHover)
    {
        painter.fillPath(backgroundPath, ElaThemeColor(d->_themeMode, BasicHover));
    }
    else
    {
        painter.fillPath(backgroundPath, ElaThemeColor(d->_themeMode, BasicBase));
    }

    // Border (light mode only)
    if (d->_themeMode == ElaThemeType::Light)
    {
        painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
        painter.drawPath(backgroundPath);
    }

    // Arrow area
    int arrowAreaWidth = 20;
    int arrowRightMargin = 8;

    // Content area (excluding arrow)
    int contentLeft = 10;
    int contentRight = width() - arrowAreaWidth;
    int contentWidth = contentRight - contentLeft;

    // Calculate icon + text total width for centering
    int iconWidth = 0;
    int iconTextGap = 6;
    int textWidth = 0;

    if (d->_pElaIcon != ElaIconType::None)
    {
        QFont iconFont("ElaAwesome");
        iconFont.setPixelSize(16);
        iconWidth = QFontMetrics(iconFont).horizontalAdvance(QChar(static_cast<int>(d->_pElaIcon)));
    }

    if (!d->_pText.isEmpty())
    {
        QFont textFont = font();
        textFont.setPixelSize(14);
        textWidth = QFontMetrics(textFont).horizontalAdvance(d->_pText);
    }

    int totalContentWidth = iconWidth;
    if (iconWidth > 0 && textWidth > 0)
    {
        totalContentWidth += iconTextGap;
    }
    totalContentWidth += textWidth;

    int drawX = contentLeft + (contentWidth - totalContentWidth) / 2;

    // Draw icon
    if (d->_pElaIcon != ElaIconType::None)
    {
        QFont iconFont("ElaAwesome");
        iconFont.setPixelSize(16);
        painter.setFont(iconFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        QRect iconRect(drawX, 0, iconWidth, height());
        painter.drawText(iconRect, Qt::AlignCenter, QString(QChar(static_cast<int>(d->_pElaIcon))));
        drawX += iconWidth + iconTextGap;
    }

    // Draw text
    if (!d->_pText.isEmpty())
    {
        QFont textFont = font();
        textFont.setPixelSize(14);
        painter.setFont(textFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        QRect textRect(drawX, 0, textWidth, height());
        painter.drawText(textRect, Qt::AlignCenter, d->_pText);
    }

    // Draw arrow (AngleDown)
    QFont arrowFont("ElaAwesome");
    arrowFont.setPixelSize(16);
    painter.setFont(arrowFont);
    painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
    QRect arrowRect(width() - arrowAreaWidth - arrowRightMargin, 0, arrowAreaWidth, height());
    painter.drawText(arrowRect, Qt::AlignCenter, QString(QChar(static_cast<int>(ElaIconType::AngleDown))));
}

QSize ElaDropDownButton::sizeHint() const
{
    Q_D(const ElaDropDownButton);

    int iconWidth = 0;
    int iconTextGap = 6;
    int textWidth = 0;
    int arrowAreaWidth = 20;
    int arrowRightMargin = 8;
    int leftPadding = 10;
    int rightPadding = 10;

    if (d->_pElaIcon != ElaIconType::None)
    {
        QFont iconFont("ElaAwesome");
        iconFont.setPixelSize(16);
        iconWidth = QFontMetrics(iconFont).horizontalAdvance(QChar(static_cast<int>(d->_pElaIcon)));
    }

    if (!d->_pText.isEmpty())
    {
        QFont textFont = font();
        textFont.setPixelSize(14);
        textWidth = QFontMetrics(textFont).horizontalAdvance(d->_pText);
    }

    int totalWidth = leftPadding + iconWidth;
    if (iconWidth > 0 && textWidth > 0)
    {
        totalWidth += iconTextGap;
    }
    totalWidth += textWidth + arrowAreaWidth + arrowRightMargin + rightPadding;

    return QSize(totalWidth, 35);
}

ElaDropDownButtonPrivate::ElaDropDownButtonPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDropDownButtonPrivate::~ElaDropDownButtonPrivate()
{
}
