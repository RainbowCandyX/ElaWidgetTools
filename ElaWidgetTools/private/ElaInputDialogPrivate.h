#ifndef ELAINPUTDIALOGPRIVATE_H
#define ELAINPUTDIALOGPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaInputDialog;
class ElaMaskWidget;
class ElaAppBar;
class ElaLineEdit;
class ElaSpinBox;
class ElaDoubleSpinBox;
class ElaText;
class ElaPushButton;
class QWidget;
class QTextEdit;
class QVBoxLayout;

class ElaInputDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaInputDialog)
    Q_PROPERTY_CREATE_D(QString, TitleText)
    Q_PROPERTY_CREATE_D(QString, SubTitleText)
    Q_PROPERTY_CREATE_D(QString, LabelText)
    Q_PROPERTY_CREATE_D(QString, TextValue)
    Q_PROPERTY_CREATE_D(int, IntValue)
    Q_PROPERTY_CREATE_D(double, DoubleValue)
    Q_PROPERTY_CREATE_D(QString, OkButtonText)
    Q_PROPERTY_CREATE_D(QString, CancelButtonText)
    Q_PROPERTY_CREATE_D(QString, PlaceholderText)
    Q_PROPERTY_CREATE_D(int, InputMinimumWidth)
    Q_PROPERTY_CREATE_D(int, InputMaximumWidth)

public:
    explicit ElaInputDialogPrivate(QObject* parent = nullptr);
    ~ElaInputDialogPrivate() override;

private:
    qint64 _currentWinID{0};
    ElaAppBar* _appBar{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    ElaMaskWidget* _maskWidget{nullptr};
    ElaText* _titleLabel{nullptr};
    ElaText* _subTitleLabel{nullptr};
    ElaText* _inputLabel{nullptr};
    ElaLineEdit* _lineEdit{nullptr};
    ElaSpinBox* _spinBox{nullptr};
    ElaDoubleSpinBox* _doubleSpinBox{nullptr};
    QTextEdit* _textEdit{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    QWidget* _buttonWidget{nullptr};
    ElaPushButton* _okButton{nullptr};
    ElaPushButton* _cancelButton{nullptr};
    bool _isMultiLine{false};
    bool _isIntMode{false};
    bool _isDoubleMode{false};
    void _doCloseAnimation(bool isAccept);
    friend class ElaInputDialog;
};

#endif // ELAINPUTDIALOGPRIVATE_H
