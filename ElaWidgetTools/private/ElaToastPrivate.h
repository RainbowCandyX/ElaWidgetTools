#ifndef ELATOASTPRIVATE_H
#define ELATOASTPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaToast.h"
class ElaToastPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaToast)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DisplayMsec)

public:
    explicit ElaToastPrivate(QObject* parent = nullptr);
    ~ElaToastPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int _shadowBorderWidth{0};
#else
    int _shadowBorderWidth{6};
#endif
    ElaToast::ToastType _toastType{ElaToast::Info};
    QString _text;
};

#endif // ELATOASTPRIVATE_H
