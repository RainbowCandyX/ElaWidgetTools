#ifndef ELAPASSWORDBOXPRIVATE_H
#define ELAPASSWORDBOXPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class QAction;
class ElaEvent;
class ElaPasswordBox;
class ElaPasswordBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPasswordBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsPasswordVisible)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit ElaPasswordBoxPrivate(QObject* parent = nullptr);
    ~ElaPasswordBoxPrivate() override;
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaEvent* _focusEvent{nullptr};
    QAction* _toggleAction{nullptr};
    qreal _textSpacing{0.5};
};

#endif // ELAPASSWORDBOXPRIVATE_H
