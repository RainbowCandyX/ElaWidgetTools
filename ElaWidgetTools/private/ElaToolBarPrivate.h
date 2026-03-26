#ifndef ELATOOLBARPRIVATE_H
#define ELATOOLBARPRIVATE_H
#include <QObject>

#include "ElaDef.h"
class ElaToolBar;
class ElaToolBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToolBar)
public:
    explicit ElaToolBarPrivate(QObject* parent = nullptr);
    ~ElaToolBarPrivate();

private:
    ElaThemeType::ThemeMode _themeMode;
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
    int _shadowBorderWidth{0};
#else
    int _shadowBorderWidth{6};
#endif
};

#endif // ELATOOLBARPRIVATE_H
