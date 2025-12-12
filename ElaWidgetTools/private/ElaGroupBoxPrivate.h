#ifndef ELAGROUPBOXPRIVATE_H
#define ELAGROUPBOXPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaGroupBox;
class ElaGroupBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaGroupBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaGroupBoxPrivate(QObject* parent = nullptr);
    ~ElaGroupBoxPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAGROUPBOXPRIVATE_H
