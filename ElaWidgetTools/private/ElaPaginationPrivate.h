#ifndef ELAPAGINATIONPRIVATE_H
#define ELAPAGINATIONPRIVATE_H

#include <QList>
#include <QObject>
#include <QPair>
#include <QRect>

#include "ElaDef.h"
class ElaLineEdit;
class ElaPagination;
class ElaPaginationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPagination)
    Q_PROPERTY_CREATE_D(int, CurrentPage)
    Q_PROPERTY_CREATE_D(int, TotalPages)
    Q_PROPERTY_CREATE_D(int, ButtonSize)
    Q_PROPERTY_CREATE_D(int, PagerCount)
    Q_PROPERTY_CREATE_D(bool, JumperVisible)
public:
    explicit ElaPaginationPrivate(QObject* parent = nullptr);
    ~ElaPaginationPrivate() override;

    QList<QPair<QRect, int>> _getButtonRects() const;
    QList<int> _getVisiblePages() const;
    void _updateJumperPosition();

    ElaLineEdit* _jumperEdit{nullptr};

private:
    ElaThemeType::ThemeMode _themeMode;
    int _hoverIndex{-1};
};

#endif // ELAPAGINATIONPRIVATE_H
