#ifndef ELAINFOBAR_PRIVATE_H
#define ELAINFOBAR_PRIVATE_H

#include <QObject>
#include <QRect>
#include <functional>

#include "ElaDef.h"

class ElaInfoBar;
class ElaInfoBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaInfoBar)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Message)
    Q_PROPERTY_CREATE_D(ElaInfoBarType::InfoBarSeverity, Severity)
    Q_PROPERTY_CREATE_D(bool, IsClosable)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, InfoBarIcon)
public:
    explicit ElaInfoBarPrivate(QObject* parent = nullptr);
    ~ElaInfoBarPrivate() override;

    struct ActionItem
    {
        QString text;
        std::function<void()> callback;
    };

    ElaThemeType::ThemeMode _themeMode;
    QList<ActionItem> _actions;
    QRect _closeButtonRect;
    QList<QRect> _actionButtonRects;
    int _closeButtonHover{false};
    int _hoverActionIndex{-1};
    bool _isClosing{false};

    QColor _getSeverityColor() const;
    ElaIconType::IconName _getSeverityIcon() const;
    void _doCloseAnimation();
};

#endif // ELAINFOBAR_PRIVATE_H
