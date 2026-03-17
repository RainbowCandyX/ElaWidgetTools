#ifndef ELASELECTORBARPRIVATE_H
#define ELASELECTORBARPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"

class QVariantAnimation;
class ElaSelectorBar;
class ElaSelectorBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSelectorBar)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaSelectorBarPrivate(QObject* parent = nullptr);
    ~ElaSelectorBarPrivate() override;

    struct SelectorItem
    {
        ElaIconType::IconName icon = ElaIconType::None;
        QString text;
    };

private:
    ElaThemeType::ThemeMode _themeMode;
    QList<SelectorItem> _items;
    int _hoveredIndex{-1};
    qreal _indicatorX{0};
    qreal _indicatorWidth{0};
    QVariantAnimation* _indicatorAnimation{nullptr};
    void _startIndicatorAnimation(qreal targetX, qreal targetWidth);
    int _itemIndexAtPos(const QPoint& pos) const;
};

#endif // ELASELECTORBARPRIVATE_H
