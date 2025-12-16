#ifndef ELACONTENTDIALOGPRIVATE_H
#define ELACONTENTDIALOGPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class QVBoxLayout;
class QHBoxLayout;
class ElaPushButton;
class ElaContentDialog;
class ElaMaskWidget;
class ElaAppBar;
class ElaContentDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaContentDialog)
public:
    explicit ElaContentDialogPrivate(QObject* parent = nullptr);
    ~ElaContentDialogPrivate() override;

private:
    qint64 _currentWinID{0};
    ElaAppBar* _appBar{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaMaskWidget* _maskWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QWidget* _buttonWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QHBoxLayout* _buttonLayout{nullptr};
    QString _leftButtonText{"cancel"};
    QString _middleButtonText{"minimum"};
    QString _rightButtonText{"exit"};
    ElaPushButton* _leftButton{nullptr};
    ElaPushButton* _middleButton{nullptr};
    ElaPushButton* _rightButton{nullptr};
    bool _isLeftButtonVisible{true};
    bool _isMiddleButtonVisible{true};
    bool _isRightButtonVisible{true};
    void _doCloseAnimation(bool isAccept);
};

#endif // ELACONTENTDIALOGPRIVATE_H
