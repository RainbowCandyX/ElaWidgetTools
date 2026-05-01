#ifndef ELATHEMEPRIVATE_H
#define ELATHEMEPRIVATE_H

#include <QColor>
#include <QMap>
#include <QMetaObject>
#include <QObject>

#include "ElaDef.h"
class ElaTheme;
class ElaThemePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTheme)
public:
    explicit ElaThemePrivate(QObject* parent = nullptr);
    ~ElaThemePrivate();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    QColor _lightThemeColorList[43];
    QColor _darkThemeColorList[43];
    void _initThemeColor();

    bool _isFollowSystemTheme{false};
    QMetaObject::Connection _colorSchemeConnection;
    ElaThemeType::ThemeMode _detectSystemThemeMode() const;
    void _applySystemTheme();
};

#endif // ELATHEMEPRIVATE_H
