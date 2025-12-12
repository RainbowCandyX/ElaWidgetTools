#ifndef ELAGROUPBOXSTYLE_H
#define ELAGROUPBOXSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"

class ElaGroupBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaGroupBoxStyle(QStyle* style = nullptr);
    ~ElaGroupBoxStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAGROUPBOXSTYLE_H
