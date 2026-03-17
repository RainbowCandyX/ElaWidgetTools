#ifndef ELACAPTCHAPRIVATE_H
#define ELACAPTCHAPRIVATE_H

#include <QList>
#include <QObject>

#include "ElaDef.h"

class QLineEdit;
class QHBoxLayout;
class ElaCaptcha;
class ElaCaptchaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCaptcha)
    Q_PROPERTY_CREATE_D(int, CodeLength)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BoxSize)
    Q_PROPERTY_CREATE_D(int, BoxSpacing)
public:
    explicit ElaCaptchaPrivate(QObject* parent = nullptr);
    ~ElaCaptchaPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    ElaThemeType::ThemeMode _themeMode;
    int _inputMode{0}; // 0=DigitOnly, 1=AlphaNumeric
    QList<QLineEdit*> _boxes;
    QHBoxLayout* _boxLayout{nullptr};
    void _buildBoxes();
    void _updateValidators();
    void _applyThemeStyle();
    void _applyBoxStyle(QLineEdit* box, bool focused);
    void _onTextChanged(QLineEdit* box, int index);
};

#endif // ELACAPTCHAPRIVATE_H
