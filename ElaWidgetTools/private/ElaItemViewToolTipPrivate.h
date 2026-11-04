#ifndef ELAITEMVIEWTOOLTIPPRIVATE_H
#define ELAITEMVIEWTOOLTIPPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaItemViewToolTip;
class ElaText;
class QVBoxLayout;
class QTimer;
class QAbstractItemView;

class ElaItemViewToolTipPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaItemViewToolTip)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DisplayMsec)
    Q_PROPERTY_CREATE_D(int, ShowDelayMsec)
    Q_PROPERTY_CREATE_D(int, HideDelayMsec)
    Q_PROPERTY_CREATE_D(QString, ToolTip)
public:
    explicit ElaItemViewToolTipPrivate(QObject* parent = nullptr);
    ~ElaItemViewToolTipPrivate() override;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void _doShowAnimation();
    void _updatePos();

    ElaText* _toolTipText{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    ElaThemeType::ThemeMode _themeMode{ElaThemeType::Light};
    int _shadowBorderWidth{6};

    QAbstractItemView* _view{nullptr};
    QPointer<QWidget> _viewport;
    QTimer* _showTimer{nullptr};
    QTimer* _displayTimer{nullptr};
    QTimer* _hideTimer{nullptr};

    QModelIndex _hoverIndex;
    bool _suppressed{false};   // DisplayMsec 到点隐藏后，同一行不再重弹
};

#endif // ELAITEMVIEWTOOLTIPPRIVATE_H
