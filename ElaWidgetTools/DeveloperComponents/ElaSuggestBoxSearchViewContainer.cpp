#include "ElaSuggestBoxSearchViewContainer.h"

#include <QPainter>

#include "ElaTheme.h"
ElaSuggestBoxSearchViewContainer::ElaSuggestBoxSearchViewContainer(QWidget* parent)
    : QWidget{parent}
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
    setContentsMargins(2, 2, 2, 2);
#else
    setContentsMargins(8, 8, 8, 8);
#endif
    setObjectName("ElaSuggestBoxSearchViewBaseWidget");
    setStyleSheet("#ElaSuggestBoxSearchViewBaseWidget{background-color:transparent}");
    _themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

ElaSuggestBoxSearchViewContainer::~ElaSuggestBoxSearchViewContainer()
{
}

void ElaSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
    QRect foregroundRect = rect();
#else
    eTheme->drawEffectShadow(&painter, rect(), 6, 8);
    QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6);
#endif
    painter.setPen(ElaThemeColor(_themeMode, PopupBorder));
    painter.setBrush(ElaThemeColor(_themeMode, PopupBase));
    painter.drawRoundedRect(foregroundRect, 8, 8);
    painter.restore();
}
