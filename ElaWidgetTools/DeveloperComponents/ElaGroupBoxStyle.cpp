#include "ElaGroupBoxStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QStyleOptionGroupBox>

#include "ElaGroupBox.h"
#include "ElaTheme.h"

ElaGroupBoxStyle::ElaGroupBoxStyle(QStyle* style)
{
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaGroupBoxStyle::~ElaGroupBoxStyle()
{
}

void ElaGroupBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    if (control == CC_GroupBox)
    {
        if (const QStyleOptionGroupBox* groupBox = qstyleoption_cast<const QStyleOptionGroupBox*>(option))
        {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

            int borderRadius = 6;
            if (const ElaGroupBox* elaGroupBox = qobject_cast<const ElaGroupBox*>(widget))
            {
                borderRadius = elaGroupBox->getBorderRadius();
            }

            QRect frameRect = proxy()->subControlRect(CC_GroupBox, option, SC_GroupBoxFrame, widget);
            QRect textRect = proxy()->subControlRect(CC_GroupBox, option, SC_GroupBoxLabel, widget);

            if (groupBox->subControls & QStyle::SC_GroupBoxFrame)
            {
                QRect drawRect = frameRect;
                QFontMetrics fm(groupBox->fontMetrics);
                int titleHeight = fm.height();
                drawRect.setTop(frameRect.top() + titleHeight / 2);

                QPainterPath framePath;
                framePath.addRoundedRect(drawRect, borderRadius, borderRadius);
                painter->setPen(QPen(ElaThemeColor(_themeMode, BasicBorder), 1.5));
                painter->setBrush(Qt::NoBrush);
                painter->drawPath(framePath);
            }

            if ((groupBox->subControls & QStyle::SC_GroupBoxLabel) && !groupBox->text.isEmpty())
            {
                QFontMetrics fm(groupBox->fontMetrics);
                int titleHeight = fm.height();
                int borderTop = frameRect.top() + titleHeight / 2;

                int textWidth = fm.horizontalAdvance(groupBox->text);
                int textLeft = textRect.center().x() - textWidth / 2;
                int textRight = textRect.center().x() + textWidth / 2;

                QRect titleBgRect(textLeft - 4, borderTop - 2, textWidth + 8, 2);
                painter->fillRect(titleBgRect, groupBox->palette.color(QPalette::Window));

                QColor textColor = ElaThemeColor(_themeMode, BasicText);
                if (option->state & State_HasFocus)
                {
                    textColor = ElaThemeColor(_themeMode, PrimaryNormal);
                }
                painter->setPen(textColor);
                painter->setBrush(Qt::NoBrush);

                int alignment = int(groupBox->textAlignment);
                if (!proxy()->styleHint(QStyle::SH_UnderlineShortcut, option, widget))
                {
                    alignment |= Qt::TextHideMnemonic;
                }

                painter->drawText(textRect, Qt::TextShowMnemonic | Qt::AlignHCenter | alignment, groupBox->text);
            }

            painter->restore();
            return;
        }
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void ElaGroupBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_FrameGroupBox:
    {
        return;
    }
    default:
        break;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int ElaGroupBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    return QProxyStyle::pixelMetric(metric, option, widget);
}
