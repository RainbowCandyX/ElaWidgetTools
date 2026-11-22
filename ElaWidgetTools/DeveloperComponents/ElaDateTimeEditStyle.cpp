#include "ElaDateTimeEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionComboBox>
#include <QStyleOptionSpinBox>
#include <cmath>

#include "ElaTheme.h"
ElaDateTimeEditStyle::ElaDateTimeEditStyle(QStyle* style)
{
    Q_UNUSED(style);
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

ElaDateTimeEditStyle::~ElaDateTimeEditStyle()
{
}

void ElaDateTimeEditStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_SpinBox:
    {
        if (!qstyleoption_cast<const QStyleOptionSpinBox*>(option))
        {
            break;
        }
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
        //背景
        _drawFrame(painter, option->rect.adjusted(1, 1, -1, -1), option->state);
        if (option->subControls.testFlag(QStyle::SC_SpinBoxUp) || option->subControls.testFlag(QStyle::SC_SpinBoxDown))
        {
            //添加按钮
            const QRect addLineRect = subControlRect(control, option, QStyle::SC_ScrollBarAddLine, widget);
            //减少按钮
            const QRect subLineRect = subControlRect(control, option, QStyle::SC_ScrollBarSubLine, widget);
            _drawButtonBackground(painter, addLineRect, option->state, option->activeSubControls.testFlag(QStyle::SC_ScrollBarAddLine));
            _drawButtonBackground(painter, subLineRect, option->state, option->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine));
            //图标绘制
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(17);
            painter->setFont(iconFont);
            painter->setPen(option->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
            painter->drawText(addLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleUp));
            painter->drawText(subLineRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::AngleDown));
        }
        painter->restore();
        return;
    }
    case QStyle::CC_ComboBox:
    {
        //日历弹出模式下 QDateTimeEdit 以 CC_ComboBox 绘制
        if (!qstyleoption_cast<const QStyleOptionComboBox*>(option))
        {
            break;
        }
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
        _drawFrame(painter, option->rect.adjusted(1, 1, -1, -1), option->state);
        if (option->subControls.testFlag(QStyle::SC_ComboBoxArrow))
        {
            const QRect arrowRect = subControlRect(control, option, QStyle::SC_ComboBoxArrow, widget);
            _drawButtonBackground(painter, arrowRect, option->state, option->state.testFlag(QStyle::State_Sunken));
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(15);
            painter->setFont(iconFont);
            painter->setPen(option->state.testFlag(QStyle::State_Enabled) ? ElaThemeColor(_themeMode, BasicText) : ElaThemeColor(_themeMode, BasicTextDisable));
            painter->drawText(arrowRect, Qt::AlignCenter, QChar((unsigned short)ElaIconType::CalendarDays));
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect ElaDateTimeEditStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    switch (cc)
    {
    case QStyle::CC_SpinBox:
    {
        const QRect frameRect = QProxyStyle::subControlRect(cc, opt, QStyle::SC_SpinBoxFrame, widget).adjusted(1, 1, -1, -1);
        //按钮尺寸
        const int buttonSize = frameRect.height() - 4;
        switch (sc)
        {
        case QStyle::SC_ScrollBarAddLine:
        {
            //添加按钮
            return QRect(frameRect.x() + frameRect.width() - buttonSize - 3, frameRect.center().y() - buttonSize / 2, buttonSize, buttonSize);
        }
        case QStyle::SC_ScrollBarSubLine:
        {
            //减少按钮
            return QRect(frameRect.x() + frameRect.width() - 2 * buttonSize - 5, frameRect.center().y() - buttonSize / 2, buttonSize, buttonSize);
        }
        case QStyle::SC_SpinBoxEditField:
        {
            const QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, QStyle::SC_SpinBoxFrame, widget);
            return {spinBoxRect.x(), spinBoxRect.y(), spinBoxRect.width() - 2 * buttonSize - 7, spinBoxRect.height()};
        }
        default:
        {
            break;
        }
        }
        break;
    }
    case QStyle::CC_ComboBox:
    {
        const QRect comboBoxRect = opt->rect;
        switch (sc)
        {
        case QStyle::SC_ComboBoxArrow:
        {
            //日历按钮
            const int buttonSize = comboBoxRect.height() - 8;
            return QRect(comboBoxRect.x() + comboBoxRect.width() - buttonSize - 5, comboBoxRect.center().y() - buttonSize / 2, buttonSize, buttonSize);
        }
        case QStyle::SC_ComboBoxEditField:
        {
            //文字区域
            return {comboBoxRect.x(), comboBoxRect.y(), comboBoxRect.width() - comboBoxRect.height() + 1, comboBoxRect.height()};
        }
        default:
        {
            break;
        }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

void ElaDateTimeEditStyle::_drawFrame(QPainter* painter, const QRect& frameRect, const QStyle::State& state) const
{
    painter->setPen(ElaThemeColor(_themeMode, BasicBorder));
    if (state.testFlag(QStyle::State_Enabled))
    {
        if (state.testFlag(QStyle::State_MouseOver))
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
        }
        else
        {
            painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
        }
    }
    else
    {
        painter->setBrush(ElaThemeColor(_themeMode, BasicDisable));
    }
    painter->drawRoundedRect(frameRect, 4, 4);
    //底边线
    painter->setPen(Qt::NoPen);
    painter->setBrush(ElaThemeColor(_themeMode, BasicHemline));
    const int left = frameRect.x();
    const int right = frameRect.x() + frameRect.width();
    const int bottom = frameRect.y() + frameRect.height();
    QPainterPath path;
    path.moveTo(left + 4, bottom);
    path.lineTo(right - 4, bottom);
    path.arcTo(QRectF(right - 8, bottom - 8, 8, 8), -90, 45);
    path.lineTo(left + 4 - 2 * std::sqrt(2), bottom - (4 - 2 * std::sqrt(2)));
    path.arcTo(QRectF(left, bottom - 8, 8, 8), 225, 45);
    path.closeSubpath();
    painter->drawPath(path);
}

void ElaDateTimeEditStyle::_drawButtonBackground(QPainter* painter, const QRect& buttonRect, const QStyle::State& state, bool isActive) const
{
    if (!isActive || !state.testFlag(QStyle::State_Enabled))
    {
        return;
    }
    painter->setPen(Qt::NoPen);
    if (state.testFlag(QStyle::State_Sunken) && state.testFlag(QStyle::State_MouseOver))
    {
        painter->setBrush(ElaThemeColor(_themeMode, BasicPressAlpha));
    }
    else
    {
        painter->setBrush(ElaThemeColor(_themeMode, BasicHoverAlpha));
    }
    painter->drawRoundedRect(buttonRect, 4, 4);
}
