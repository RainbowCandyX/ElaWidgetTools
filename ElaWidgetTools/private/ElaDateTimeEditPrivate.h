#ifndef ELADATETIMEEDITPRIVATE_H
#define ELADATETIMEEDITPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaMenu;
class ElaDateTimeEdit;
class ElaDateTimeEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDateTimeEdit)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaDateTimeEditPrivate(QObject* parent = nullptr);
    ~ElaDateTimeEditPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaMenu* _createStandardContextMenu();
};

#endif // ELADATETIMEEDITPRIVATE_H
