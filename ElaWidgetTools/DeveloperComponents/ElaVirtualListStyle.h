#ifndef ELAVIRTUALLISTSTYLE_H
#define ELAVIRTUALLISTSTYLE_H

#include <QProxyStyle>

#include "ElaDef.h"
class ElaVirtualListStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, ItemHeight)
public:
    explicit ElaVirtualListStyle(QStyle* style = nullptr);
    ~ElaVirtualListStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    int _leftPadding{11};
};

#endif // ELAVIRTUALLISTSTYLE_H
