#include "ElaStatCard.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaStatCardPrivate.h"

Q_PROPERTY_REF_CREATE_Q_CPP(ElaStatCard, QString, Title)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaStatCard, QString, Value)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaStatCard, QString, Description)
Q_PROPERTY_CREATE_Q_CPP(ElaStatCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaStatCard, ElaIconType::IconName, CardIcon)

ElaStatCard::ElaStatCard(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaStatCardPrivate())
{
    Q_D(ElaStatCard);
    d->q_ptr = this;
    d->_pBorderRadius = 8;
    d->_pCardIcon = ElaIconType::None;
    setObjectName("ElaStatCard");
    setFixedSize(220, 120);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaStatCard::~ElaStatCard()
{
}

void ElaStatCard::setTrend(TrendType trend)
{
    Q_D(ElaStatCard);
    d->_trend = trend;
    update();
}

ElaStatCard::TrendType ElaStatCard::getTrend() const
{
    return d_ptr->_trend;
}

void ElaStatCard::setTrendText(const QString& text)
{
    Q_D(ElaStatCard);
    d->_trendText = text;
    update();
}

QString ElaStatCard::getTrendText() const
{
    return d_ptr->_trendText;
}

void ElaStatCard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    Q_D(ElaStatCard);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // Background rounded rect
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->_pBorderRadius, d->_pBorderRadius);
    painter.fillPath(backgroundPath, ElaThemeColor(d->_themeMode, BasicBase));
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
    painter.drawPath(backgroundPath);

    int padding = 16;
    int currentX = padding;
    int titleY = 16;

    // Icon + Title row
    if (d->_pCardIcon != ElaIconType::None)
    {
        QFont iconFont("ElaAwesome");
        iconFont.setPixelSize(20);
        painter.setFont(iconFont);
        painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
        QChar iconChar = QChar(static_cast<int>(d->_pCardIcon));
        painter.drawText(currentX, titleY + 18, QString(iconChar));
        currentX += 24;
    }

    if (!d->_pTitle.isEmpty())
    {
        QFont titleFont = font();
        titleFont.setPixelSize(13);
        painter.setFont(titleFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
        painter.drawText(currentX, titleY + 16, d->_pTitle);
    }

    // Value (big number)
    if (!d->_pValue.isEmpty())
    {
        QFont valueFont = font();
        valueFont.setPixelSize(28);
        valueFont.setBold(true);
        painter.setFont(valueFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        painter.drawText(padding, 42 + 26, d->_pValue);
    }

    // Trend + Description row
    int bottomY = 88 + 14;
    int trendX = padding;

    if (d->_trend != None)
    {
        QFont iconFont("ElaAwesome");
        iconFont.setPixelSize(12);

        QFont trendFont = font();
        trendFont.setPixelSize(12);

        if (d->_trend == Up)
        {
            QColor greenColor(0x0F, 0x7B, 0x0F);
            painter.setFont(iconFont);
            painter.setPen(greenColor);
            QChar arrowChar = QChar(static_cast<int>(ElaIconType::AngleUp));
            painter.drawText(trendX, bottomY, QString(arrowChar));
            trendX += 14;

            if (!d->_trendText.isEmpty())
            {
                painter.setFont(trendFont);
                painter.drawText(trendX, bottomY, d->_trendText);
                trendX += QFontMetrics(trendFont).horizontalAdvance(d->_trendText) + 6;
            }
        }
        else if (d->_trend == Down)
        {
            QColor redColor = ElaThemeColor(d->_themeMode, StatusDanger);
            painter.setFont(iconFont);
            painter.setPen(redColor);
            QChar arrowChar = QChar(static_cast<int>(ElaIconType::AngleDown));
            painter.drawText(trendX, bottomY, QString(arrowChar));
            trendX += 14;

            if (!d->_trendText.isEmpty())
            {
                painter.setFont(trendFont);
                painter.drawText(trendX, bottomY, d->_trendText);
                trendX += QFontMetrics(trendFont).horizontalAdvance(d->_trendText) + 6;
            }
        }
        else if (d->_trend == Neutral)
        {
            QColor neutralColor = ElaThemeColor(d->_themeMode, BasicDetailsText);
            painter.setFont(trendFont);
            painter.setPen(neutralColor);
            painter.drawText(trendX, bottomY, QStringLiteral("\u2014"));
            trendX += QFontMetrics(trendFont).horizontalAdvance(QStringLiteral("\u2014")) + 6;

            if (!d->_trendText.isEmpty())
            {
                painter.drawText(trendX, bottomY, d->_trendText);
                trendX += QFontMetrics(trendFont).horizontalAdvance(d->_trendText) + 6;
            }
        }
    }

    // Description
    if (!d->_pDescription.isEmpty())
    {
        QFont descFont = font();
        descFont.setPixelSize(12);
        painter.setFont(descFont);
        painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
        painter.drawText(trendX, bottomY, d->_pDescription);
    }
}

ElaStatCardPrivate::ElaStatCardPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaStatCardPrivate::~ElaStatCardPrivate()
{
}
