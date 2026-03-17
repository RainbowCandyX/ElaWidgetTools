#include "ElaSelectorBar.h"

#include <QEasingCurve>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

#include "ElaTheme.h"
#include "private/ElaSelectorBarPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSelectorBar, int, CurrentIndex)
Q_PROPERTY_CREATE_Q_CPP(ElaSelectorBar, int, BorderRadius)

ElaSelectorBar::ElaSelectorBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaSelectorBarPrivate())
{
    Q_D(ElaSelectorBar);
    d->q_ptr = this;
    d->_pCurrentIndex = 0;
    d->_pBorderRadius = 4;
    setObjectName("ElaSelectorBar");
    setMouseTracking(true);
    setFixedHeight(36);
    setMinimumWidth(120);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaSelectorBar::~ElaSelectorBar()
{
}

void ElaSelectorBar::addItem(const QString& text)
{
    addItem(ElaIconType::None, text);
}

void ElaSelectorBar::addItem(ElaIconType::IconName icon, const QString& text)
{
    Q_D(ElaSelectorBar);
    ElaSelectorBarPrivate::SelectorItem item;
    item.icon = icon;
    item.text = text;
    d->_items.append(item);

    if (d->_items.count() == 1)
    {
        d->_pCurrentIndex = 0;
        qreal itemWidth = static_cast<qreal>(width()) / d->_items.count();
        d->_indicatorX = 0;
        d->_indicatorWidth = itemWidth;
    }
    updateGeometry();
    update();
}

void ElaSelectorBar::clearItems()
{
    Q_D(ElaSelectorBar);
    d->_items.clear();
    d->_pCurrentIndex = 0;
    d->_hoveredIndex = -1;
    d->_indicatorX = 0;
    d->_indicatorWidth = 0;
    updateGeometry();
    update();
}

int ElaSelectorBar::getItemCount() const
{
    return d_ptr->_items.count();
}

QSize ElaSelectorBar::sizeHint() const
{
    int count = d_ptr->_items.count();
    int w = qMax(count * 80, 120);
    return QSize(w, 36);
}

void ElaSelectorBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    Q_D(ElaSelectorBar);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // Background
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->_pBorderRadius, d->_pBorderRadius);
    painter.fillPath(backgroundPath, ElaThemeColor(d->_themeMode, BasicBase));
    painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
    painter.drawPath(backgroundPath);

    int itemCount = d->_items.count();
    if (itemCount == 0)
    {
        return;
    }

    qreal itemWidth = static_cast<qreal>(width()) / itemCount;

    // Draw items
    for (int i = 0; i < itemCount; ++i)
    {
        QRectF itemRect(i * itemWidth, 0, itemWidth, height());

        // Hovered background
        if (i == d->_hoveredIndex && i != d->_pCurrentIndex)
        {
            QPainterPath hoverPath;
            hoverPath.addRoundedRect(itemRect.adjusted(2, 2, -2, -2), d->_pBorderRadius, d->_pBorderRadius);
            painter.fillPath(hoverPath, ElaThemeColor(d->_themeMode, BasicHover));
        }

        const ElaSelectorBarPrivate::SelectorItem& item = d->_items.at(i);
        bool isSelected = (i == d->_pCurrentIndex);

        // Text and icon
        QFont textFont = font();
        textFont.setPixelSize(13);
        if (isSelected)
        {
            textFont.setBold(true);
        }
        painter.setFont(textFont);
        painter.setPen(isSelected ? ElaThemeColor(d->_themeMode, PrimaryNormal) : ElaThemeColor(d->_themeMode, BasicText));

        if (item.icon != ElaIconType::None)
        {
            QFont iconFont("ElaAwesome");
            iconFont.setPixelSize(14);
            QFontMetrics iconFm(iconFont);
            QFontMetrics textFm(textFont);
            int iconWidth = iconFm.horizontalAdvance(QChar(static_cast<int>(item.icon)));
            int textWidth = textFm.horizontalAdvance(item.text);
            int totalWidth = iconWidth + 4 + textWidth;
            qreal startX = itemRect.center().x() - totalWidth / 2.0;

            painter.setFont(iconFont);
            painter.drawText(QPointF(startX, itemRect.center().y() + iconFm.ascent() / 2.0 - 1),
                             QString(QChar(static_cast<int>(item.icon))));

            painter.setFont(textFont);
            painter.drawText(QPointF(startX + iconWidth + 4, itemRect.center().y() + textFm.ascent() / 2.0 - 1),
                             item.text);
        }
        else
        {
            painter.drawText(itemRect, Qt::AlignCenter, item.text);
        }
    }

    // Bottom indicator
    if (d->_indicatorWidth > 0)
    {
        qreal indicatorInset = itemWidth * 0.25;
        QRectF indicatorRect(d->_indicatorX + indicatorInset, height() - 3, d->_indicatorWidth - indicatorInset * 2, 3);
        QPainterPath indicatorPath;
        indicatorPath.addRoundedRect(indicatorRect, 1.5, 1.5);
        painter.fillPath(indicatorPath, ElaThemeColor(d->_themeMode, PrimaryNormal));
    }
}

void ElaSelectorBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaSelectorBar);
    if (event->button() == Qt::LeftButton)
    {
        int index = d->_itemIndexAtPos(event->pos());
        if (index >= 0 && index != d->_pCurrentIndex)
        {
            d->_pCurrentIndex = index;
            qreal itemWidth = static_cast<qreal>(width()) / d->_items.count();
            d->_startIndicatorAnimation(index * itemWidth, itemWidth);
            Q_EMIT currentIndexChanged(index);
            update();
        }
    }
    QWidget::mousePressEvent(event);
}

void ElaSelectorBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaSelectorBar);
    int index = d->_itemIndexAtPos(event->pos());
    if (index != d->_hoveredIndex)
    {
        d->_hoveredIndex = index;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ElaSelectorBar::leaveEvent(QEvent* event)
{
    Q_D(ElaSelectorBar);
    d->_hoveredIndex = -1;
    update();
    QWidget::leaveEvent(event);
}

ElaSelectorBarPrivate::ElaSelectorBarPrivate(QObject* parent)
    : QObject{parent}
{
    _indicatorAnimation = new QVariantAnimation(this);
    _indicatorAnimation->setDuration(200);
    _indicatorAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

ElaSelectorBarPrivate::~ElaSelectorBarPrivate()
{
}

void ElaSelectorBarPrivate::_startIndicatorAnimation(qreal targetX, qreal targetWidth)
{
    Q_Q(ElaSelectorBar);
    _indicatorAnimation->stop();
    qreal startX = _indicatorX;
    _indicatorAnimation->setStartValue(startX);
    _indicatorAnimation->setEndValue(targetX);
    QObject::disconnect(_indicatorAnimation, nullptr, nullptr, nullptr);
    QObject::connect(_indicatorAnimation, &QVariantAnimation::valueChanged, q, [=](const QVariant& value) {
        _indicatorX = value.toReal();
        _indicatorWidth = targetWidth;
        q->update();
    });
    _indicatorAnimation->start();
}

int ElaSelectorBarPrivate::_itemIndexAtPos(const QPoint& pos) const
{
    Q_Q(const ElaSelectorBar);
    int itemCount = _items.count();
    if (itemCount == 0)
    {
        return -1;
    }
    qreal itemWidth = static_cast<qreal>(q->width()) / itemCount;
    int index = static_cast<int>(pos.x() / itemWidth);
    if (index < 0 || index >= itemCount)
    {
        return -1;
    }
    return index;
}
