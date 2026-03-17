#ifndef ELAPOPCONFIRMPRIVATE_H
#define ELAPOPCONFIRMPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class QVBoxLayout;
class QGraphicsOpacityEffect;
class ElaPushButton;
class ElaPopconfirm;
class ElaPopconfirmPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPopconfirm)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(QString, ConfirmButtonText)
    Q_PROPERTY_CREATE_D(QString, CancelButtonText)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, Icon)
    Q_PROPERTY_CREATE_D(bool, IsLightDismiss)
public:
    explicit ElaPopconfirmPrivate(QObject* parent = nullptr);
    ~ElaPopconfirmPrivate() override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QVBoxLayout* _mainLayout{nullptr};
    QGraphicsOpacityEffect* _opacityEffect{nullptr};
    ElaPushButton* _confirmButton{nullptr};
    ElaPushButton* _cancelButton{nullptr};
    int _shadowBorderWidth{8};
    void _doShowAnimation();

    friend class ElaPopconfirm;
};

#endif // ELAPOPCONFIRMPRIVATE_H
