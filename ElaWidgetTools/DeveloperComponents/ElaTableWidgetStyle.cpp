#include "ElaTableWidgetStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

#include "ElaTableWidget.h"
#include "ElaTheme.h"

ElaTableWidgetStyle::ElaTableWidgetStyle(QStyle* style)
{
    _pItemHeight = 35;
    _pHeaderMargin = 6;
    _pCurrentHoverRow = -1;
    _pIsTransparent = false;
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaTableWidgetStyle::~ElaTableWidgetStyle()
{
}

void ElaTableWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            const ElaTableWidget* tableWidget = dynamic_cast<const ElaTableWidget*>(widget);
            if (!tableWidget)
            {
                return;
            }

            QVariant backgroundData = vopt->index.data(Qt::BackgroundRole);
            bool hasCustomBackground = backgroundData.isValid();

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);

            if (hasCustomBackground)
            {
                QBrush customBrush = backgroundData.value<QBrush>();
                painter->setBrush(customBrush);
                painter->drawRect(vopt->rect);
            }

            QAbstractItemView::SelectionBehavior selectionBehavior = tableWidget->selectionBehavior();
            if (selectionBehavior == QAbstractItemView::SelectRows)
            {
                if (vopt->index.row() == _pCurrentHoverRow && !hasCustomBackground)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            else
            {
                if ((vopt->state.testFlag(QStyle::State_Selected) || vopt->state.testFlag(QStyle::State_MouseOver)) && !hasCustomBackground)
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            QVariant backgroundData = vopt->index.data(Qt::BackgroundRole);
            bool hasCustomBackground = backgroundData.isValid();

            if (hasCustomBackground)
            {
                return;
            }

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            painter->setPen(Qt::NoPen);

            if (vopt->state & QStyle::State_Selected)
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicSelectedAlpha));
                painter->drawRect(itemRect);
            }
            else
            {
                if (vopt->features.testFlag(QStyleOptionViewItem::Alternate))
                {
                    painter->setBrush(ElaThemeColor(_themeMode, BasicAlternating));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_Widget:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void ElaTableWidgetStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (!_pIsTransparent)
        {
            QRect frameRect = option->rect;
            frameRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(ElaThemeColor(_themeMode, PopupBorder));
            painter->setBrush(ElaThemeColor(_themeMode, BasicBaseAlpha));
            painter->drawRoundedRect(frameRect, 3, 3);
            painter->restore();
        }
        return;
    }
    case QStyle::CE_HeaderLabel:
    {
        if (const QStyleOptionHeader* hopt = qstyleoption_cast<const QStyleOptionHeader*>(option))
        {
            QRect headerRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            if (!hopt->text.isEmpty())
            {
                painter->setPen(ElaThemeColor(_themeMode, BasicText));
                painter->drawText(headerRect, hopt->textAlignment, hopt->text);
            }
            painter->restore();
        }
        return;
    }
    case QStyle::CE_HeaderSection:
    {
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeepAlpha));
        painter->drawRect(option->rect);
        if (option->state.testFlag(QStyle::State_Sunken))
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
        }
        else
        {
            if (option->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
            }
        }
        painter->drawRect(option->rect);
        painter->restore();
        return;
    }
    case QStyle::CE_HeaderEmptyArea:
    {
        QRect frameRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(ElaThemeColor(_themeMode, BasicBaseDeep));
        painter->drawRect(frameRect);
        painter->restore();
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            const ElaTableWidget* tableWidget = dynamic_cast<const ElaTableWidget*>(widget);
            if (!tableWidget)
            {
                return;
            }
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
            QAbstractItemView::SelectionBehavior selectionBehavior = tableWidget->selectionBehavior();
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            if (vopt->index.column() == 0)
            {
                iconRect.adjust(_horizontalPadding, 0, _horizontalPadding, 0);
                textRect.adjust(_horizontalPadding, 0, 0, 0);
            }
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            if (!vopt->text.isEmpty())
            {
                QVariant foregroundData = vopt->index.data(Qt::ForegroundRole);
                if (foregroundData.isValid())
                {
                    QBrush foregroundBrush = foregroundData.value<QBrush>();
                    painter->setPen(foregroundBrush.color());
                }
                else
                {
                    painter->setPen(ElaThemeColor(_themeMode, BasicText));
                }
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            int heightOffset = itemRect.height() / 4;
            painter->setPen(Qt::NoPen);
            painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
            if (vopt->state.testFlag(QStyle::State_Selected))
            {
                if (selectionBehavior == QAbstractItemView::SelectRows && vopt->index.column() == 0)
                {
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
                }
            }
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize ElaTableWidgetStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(_pItemHeight);
        return itemSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

int ElaTableWidgetStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_HeaderMargin:
    {
        return _pHeaderMargin;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
