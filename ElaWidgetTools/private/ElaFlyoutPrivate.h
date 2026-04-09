#ifndef ELAFLYOUTPRIVATE_H
#define ELAFLYOUTPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaFlyout;
class QVBoxLayout;
class QGraphicsOpacityEffect;
class ElaFlyoutPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaFlyout)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(bool, IsLightDismiss)

public:
    explicit ElaFlyoutPrivate(QObject* parent = nullptr);
    ~ElaFlyoutPrivate() override;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QVBoxLayout* _mainLayout{nullptr};
    QWidget* _contentWidget{nullptr};
    QGraphicsOpacityEffect* _opacityEffect{nullptr};
    ElaThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int _shadowBorderWidth{0};
#else
    int _shadowBorderWidth{8};
#endif
    void _doShowAnimation();
};

#endif // ELAFLYOUTPRIVATE_H
