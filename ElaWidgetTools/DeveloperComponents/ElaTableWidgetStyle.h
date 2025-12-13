#ifndef ELATABLEWIDGETSTYLE_H
#define ELATABLEWIDGETSTYLE_H

#include <QModelIndex>
#include <QProxyStyle>

#include "ElaDef.h"

class QStyleOptionViewItem;

class ElaTableWidgetStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PROPERTY_CREATE(int, HeaderMargin)
    Q_PROPERTY_CREATE(int, CurrentHoverRow)
    Q_PROPERTY_CREATE(bool, IsTransparent)
public:
    explicit ElaTableWidgetStyle(QStyle* style = nullptr);
    ~ElaTableWidgetStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _horizontalPadding{11};
};

#endif // ELATABLEWIDGETSTYLE_H
