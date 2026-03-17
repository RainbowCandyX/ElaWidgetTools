#ifndef ELASPLITBUTTONPRIVATE_H
#define ELASPLITBUTTONPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaMenu;
class ElaSplitButton;
class ElaSplitButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSplitButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, ElaIcon)

public:
    explicit ElaSplitButtonPrivate(QObject* parent = nullptr);
    ~ElaSplitButtonPrivate();

private:
    static const int _dropdownWidth = 30;
    ElaMenu* _menu{nullptr};
    bool _isLeftHovered{false};
    bool _isRightHovered{false};
    bool _isLeftPressed{false};
    bool _isRightPressed{false};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASPLITBUTTONPRIVATE_H
