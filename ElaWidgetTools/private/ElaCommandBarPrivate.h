#ifndef ELACOMMANDBARPRIVATE_H
#define ELACOMMANDBARPRIVATE_H

#include <QObject>

#include "ElaProperty.h"
#include "ElaCommandBar.h"

class ElaMenu;
class ElaCommandBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCommandBar)
    Q_PROPERTY_CREATE_D(int, ButtonSize)
public:
    explicit ElaCommandBarPrivate(QObject* parent = nullptr);
    ~ElaCommandBarPrivate();

    QList<QRect> _getVisibleItemRects() const;
    int _getItemWidth(const ElaCommandBar::CommandItem& item) const;

private:
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaCommandBar::CommandItem> _items;
    int _hoveredIndex{-1};
    int _pressedIndex{-1};
    int _visibleCount{0};
    ElaMenu* _overflowMenu{nullptr};
};

#endif // ELACOMMANDBARPRIVATE_H
