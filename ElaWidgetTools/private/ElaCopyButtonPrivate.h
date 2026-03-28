#ifndef ELACOPYBUTTONPRIVATE_H
#define ELACOPYBUTTONPRIVATE_H

#include <QColor>
#include <QObject>

#include "ElaDef.h"

class QTimer;
class ElaCopyButton;
class ElaCopyButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCopyButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, CopyText)
    Q_PROPERTY_CREATE_D(int, SuccessDuration)
    Q_PROPERTY_CREATE_D(QString, SuccessText)

public:
    explicit ElaCopyButtonPrivate(QObject* parent = nullptr);
    ~ElaCopyButtonPrivate();

private:
    bool _isPressed{false};
    bool _isSuccess{false};
    qreal _iconOpacity{1.0};
    int _shadowBorderWidth{3};
    ElaThemeType::ThemeMode _themeMode;
    ElaIconType::IconName _normalIcon{ElaIconType::Copy};
    ElaIconType::IconName _successIcon{ElaIconType::ClipboardCheck};
    QTimer* _resetTimer{nullptr};

    friend class ElaCopyButton;
};

#endif // ELACOPYBUTTONPRIVATE_H
