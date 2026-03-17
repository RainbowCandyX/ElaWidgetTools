#include "ElaRatingControl.h"

#include <QMouseEvent>
#include <QPainter>

#include "ElaDef.h"
#include "ElaTheme.h"
#include "private/ElaRatingControlPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaRatingControl, int, MaxRating)
Q_PROPERTY_CREATE_Q_CPP(ElaRatingControl, int, StarSize)
Q_PROPERTY_CREATE_Q_CPP(ElaRatingControl, int, Spacing)
Q_PROPERTY_CREATE_Q_CPP(ElaRatingControl, bool, IsReadOnly)

void ElaRatingControl::setRating(int Rating)
{
    Q_D(ElaRatingControl);
    if (d->_pRating == Rating)
    {
        return;
    }
    d->_pRating = Rating;
    Q_EMIT pRatingChanged();
    Q_EMIT ratingChanged(Rating);
    update();
}

int ElaRatingControl::getRating() const
{
    return d_ptr->_pRating;
}

ElaRatingControlPrivate::ElaRatingControlPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaRatingControlPrivate::~ElaRatingControlPrivate()
{
}

ElaRatingControl::ElaRatingControl(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaRatingControlPrivate())
{
    Q_D(ElaRatingControl);
    d->q_ptr = this;
    d->_pMaxRating = 5;
    d->_pRating = 0;
    d->_pStarSize = 24;
    d->_pSpacing = 4;
    d->_pIsReadOnly = false;
    setObjectName("ElaRatingControl");
    setMouseTracking(true);
    setFixedHeight(d->_pStarSize + 4);
    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaRatingControl::~ElaRatingControl()
{
}

void ElaRatingControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(ElaRatingControl);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(d->_pStarSize - 4);
    painter.setFont(iconFont);

    int effectiveRating = d->_pRating;
    if (!d->_pIsReadOnly && d->_hoveredStar >= 0)
    {
        effectiveRating = d->_hoveredStar + 1;
    }

    for (int i = 0; i < d->_pMaxRating; ++i)
    {
        QRect starRect(i * (d->_pStarSize + d->_pSpacing), 2, d->_pStarSize, d->_pStarSize);
        if (i < effectiveRating)
        {
            painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
        }
        else
        {
            painter.setPen(ElaThemeColor(d->_themeMode, BasicBorderDeep));
        }
        painter.drawText(starRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::Star));
    }
}

void ElaRatingControl::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaRatingControl);
    if (d->_pIsReadOnly)
    {
        QWidget::mouseMoveEvent(event);
        return;
    }
    int x = event->pos().x();
    int starUnit = d->_pStarSize + d->_pSpacing;
    int hoveredStar = -1;
    if (x >= 0 && starUnit > 0)
    {
        hoveredStar = x / starUnit;
        if (hoveredStar >= d->_pMaxRating)
        {
            hoveredStar = -1;
        }
    }
    if (d->_hoveredStar != hoveredStar)
    {
        d->_hoveredStar = hoveredStar;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ElaRatingControl::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaRatingControl);
    if (!d->_pIsReadOnly && d->_hoveredStar >= 0)
    {
        setRating(d->_hoveredStar + 1);
    }
    QWidget::mousePressEvent(event);
}

void ElaRatingControl::leaveEvent(QEvent* event)
{
    Q_D(ElaRatingControl);
    d->_hoveredStar = -1;
    update();
    QWidget::leaveEvent(event);
}

QSize ElaRatingControl::sizeHint() const
{
    Q_D(const ElaRatingControl);
    int w = d->_pMaxRating * (d->_pStarSize + d->_pSpacing) - d->_pSpacing;
    int h = d->_pStarSize + 4;
    return QSize(w, h);
}
