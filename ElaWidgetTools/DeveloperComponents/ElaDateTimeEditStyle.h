#ifndef ELADATETIMEEDITSTYLE_H
#define ELADATETIMEEDITSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaDateTimeEditStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit ElaDateTimeEditStyle(QStyle* style = nullptr);
    ~ElaDateTimeEditStyle() override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
    void _drawFrame(QPainter* painter, const QRect& frameRect, const QStyle::State& state) const;
    void _drawButtonBackground(QPainter* painter, const QRect& buttonRect, const QStyle::State& state, bool isActive) const;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELADATETIMEEDITSTYLE_H
