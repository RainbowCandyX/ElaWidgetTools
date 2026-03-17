#ifndef ELADROPDOWNBUTTONPRIVATE_H
#define ELADROPDOWNBUTTONPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMenu;
class ElaDropDownButton;
class ElaDropDownButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDropDownButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, ElaIcon)
public:
    explicit ElaDropDownButtonPrivate(QObject* parent = nullptr);
    ~ElaDropDownButtonPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    bool _isHover{false};
    bool _isPressed{false};
    ElaMenu* _menu{nullptr};
};

#endif // ELADROPDOWNBUTTONPRIVATE_H
