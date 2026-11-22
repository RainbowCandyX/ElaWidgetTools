#ifndef ELAMESSAGEBOXPRIVATE_H
#define ELAMESSAGEBOXPRIVATE_H

#include <QColor>
#include <QObject>
#include <QStringList>

#include "ElaDef.h"

class ElaMessageBox;
class ElaMaskWidget;
class ElaAppBar;
class ElaText;
class ElaPushButton;
class QHBoxLayout;
class QWidget;

class ElaMessageBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageBox)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(QString, InformationText)
    Q_PROPERTY_CREATE_D(QColor, IconColor)
public:
    explicit ElaMessageBoxPrivate(QObject* parent = nullptr);
    ~ElaMessageBoxPrivate() override;

    void _rebuildButtons();
    void _updateContent();
    void _doCloseAnimation(int buttonIndex);
    void _moveToCenter();
    int _effectiveDefaultButtonIndex() const;

    ElaThemeType::ThemeMode _themeMode;
    ElaIconType::IconName _icon{ElaIconType::None};
    ElaMaskWidget* _maskWidget{nullptr};
    ElaAppBar* _appBar{nullptr};
    ElaText* _iconLabel{nullptr};
    ElaText* _titleLabel{nullptr};
    ElaText* _textLabel{nullptr};
    ElaText* _informationLabel{nullptr};
    QWidget* _buttonWidget{nullptr};
    QHBoxLayout* _buttonLayout{nullptr};
    QList<ElaPushButton*> _buttons;
    QStringList _buttonTexts;
    int _defaultButtonIndex{-1};
    bool _isDefaultButtonIndexSet{false};
    int _clickedButtonIndex{-1};

private:
    friend class ElaMessageBox;
};

#endif // ELAMESSAGEBOXPRIVATE_H
