#ifndef ELAVIRTUALLISTPRIVATE_H
#define ELAVIRTUALLISTPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaVirtualList;
class ElaVirtualListStyle;
class ElaVirtualListPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaVirtualList)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsAlternatingRowColors)
public:
    explicit ElaVirtualListPrivate(QObject* parent = nullptr);
    ~ElaVirtualListPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaVirtualListStyle* _listViewStyle{nullptr};
    int _itemCount{0};
};

#endif // ELAVIRTUALLISTPRIVATE_H
