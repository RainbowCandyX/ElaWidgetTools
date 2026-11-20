#include "ElaPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaIcon.h"
#include "ElaTheme.h"
#include "private/ElaPushButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(ElaPushButton, QColor, DarkPressColor)
ElaPushButton::ElaPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaPushButtonPrivate())
{
    Q_D(ElaPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = eTheme->getThemeMode();
    setPrimary(false);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("ElaPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

ElaPushButton::ElaPushButton(QString text, QWidget* parent)
    : ElaPushButton(parent)
{
    setText(text);
}

ElaPushButton::~ElaPushButton()
{
}

void ElaPushButton::setLightTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_lightTextColor = color;
}

QColor ElaPushButton::getLightTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_lightTextColor;
}

void ElaPushButton::setDarkTextColor(QColor color)
{
    Q_D(ElaPushButton);
    d->_darkTextColor = color;
}

QColor ElaPushButton::getDarkTextColor() const
{
    Q_D(const ElaPushButton);
    return d->_darkTextColor;
}

void ElaPushButton::setElaIcon(ElaIconType::IconName icon)
{
    Q_D(ElaPushButton);
    d->_icon = icon;
    d->_hasIcon = (icon != ElaIconType::None);
    update();
}

void ElaPushButton::setElaIcon(ElaIconType::IconName icon, int iconSize)
{
    Q_D(ElaPushButton);
    d->_icon = icon;
    d->_iconSize = iconSize;
    d->_hasIcon = (icon != ElaIconType::None);
    update();
}

void ElaPushButton::setPrimary(bool isPrimary) 
{
    Q_D(ElaPushButton);
    if (isPrimary) {
        d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, PrimaryNormal);
        d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, PrimaryNormal);
        d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, PrimaryHover);
        d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, PrimaryHover);
        d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, PrimaryPress);
        d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, PrimaryPress);
        d->_lightTextColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
        d->_darkTextColor = ElaThemeColor(ElaThemeType::Light, BasicText);
    }
    else {
        d->_pLightDefaultColor = ElaThemeColor(ElaThemeType::Light, BasicBase);
        d->_pDarkDefaultColor = ElaThemeColor(ElaThemeType::Dark, BasicBase);
        d->_pLightHoverColor = ElaThemeColor(ElaThemeType::Light, BasicHover);
        d->_pDarkHoverColor = ElaThemeColor(ElaThemeType::Dark, BasicHover);
        d->_pLightPressColor = ElaThemeColor(ElaThemeType::Light, BasicPress);
        d->_pDarkPressColor = ElaThemeColor(ElaThemeType::Dark, BasicPress);
        d->_lightTextColor = ElaThemeColor(ElaThemeType::Light, BasicText);
        d->_darkTextColor = ElaThemeColor(ElaThemeType::Dark, BasicText);
    }
}


void ElaPushButton::setHoverEnabled(bool enabled)
{
    Q_D(ElaPushButton);
    d->_isHoverEnabled = enabled;
    update();
}

bool ElaPushButton::isHoverEnabled() const
{
    Q_D(const ElaPushButton);
    return d->_isHoverEnabled;
}

void ElaPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void ElaPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaPushButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void ElaPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(ElaPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    const bool isFlat = this->isFlat();
    if (!isFlat)
    {
        eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);
    }

    // 背景绘制
    painter.save();
    const int borderWidth = isFlat ? 0 : d->_shadowBorderWidth;
    QRect foregroundRect(borderWidth, borderWidth, width() - 2 * borderWidth, height() - 2 * borderWidth);
    const bool isHovered = d->_isHoverEnabled && underMouse();
    QColor backgroundColor;
    bool hasBackground = true;
    if (isEnabled())
    {
        if (d->_themeMode == ElaThemeType::Light)
        {
            backgroundColor = d->_isPressed ? d->_pLightPressColor : isHovered ? d->_pLightHoverColor : d->_pLightDefaultColor;
        }
        else
        {
            backgroundColor = d->_isPressed ? d->_pDarkPressColor : isHovered ? d->_pDarkHoverColor : d->_pDarkDefaultColor;
        }
        // 扁平模式仅在悬停或按下时绘制背景
        hasBackground = !isFlat || d->_isPressed || isHovered;
    }
    else
    {
        // 扁平模式禁用时不绘制背景
        hasBackground = !isFlat;
        backgroundColor = ElaThemeColor(d->_themeMode, BasicDisable);
    }
    if (hasBackground)
    {
        if (!isFlat && d->_themeMode == ElaThemeType::Light)
        {
            painter.setPen(ElaThemeColor(ElaThemeType::Light, BasicBorder));
        }
        else
        {
            painter.setPen(Qt::NoPen);
        }
        painter.setBrush(backgroundColor);
        painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    }
    // 底边线绘制
    if (!isFlat && !d->_isPressed)
    {
        painter.setPen(ElaThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    painter.setPen(isEnabled() ? d->_themeMode == ElaThemeType::Light ? d->_lightTextColor : d->_darkTextColor : ElaThemeColor(d->_themeMode, BasicTextDisable));
    if (d->_hasIcon)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(d->_iconSize);

        QFontMetrics iconFontMetrics(iconFont);
        int iconWidth = iconFontMetrics.horizontalAdvance(QChar((unsigned short)d->_icon));

        QFont textFont = this->font();
        QFontMetrics textFontMetrics(textFont);
        int textWidth = textFontMetrics.horizontalAdvance(text());

        int spacing = text().isEmpty() ? 0 : 8;
        int totalWidth = iconWidth + spacing + textWidth;
        int startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;

        painter.setFont(iconFont);
        painter.drawText(QRect(startX, foregroundRect.y(), iconWidth, foregroundRect.height()), Qt::AlignLeft | Qt::AlignVCenter, QChar((unsigned short)d->_icon));

        if (!text().isEmpty())
        {
            painter.setFont(textFont);
            painter.drawText(QRect(startX + iconWidth + spacing, foregroundRect.y(), textWidth, foregroundRect.height()), Qt::AlignLeft | Qt::AlignVCenter, text());
        }
    }
    else
    {
        painter.drawText(foregroundRect, Qt::AlignCenter, text());
    }
    painter.restore();
}
