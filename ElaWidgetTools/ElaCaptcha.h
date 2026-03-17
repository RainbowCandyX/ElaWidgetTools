#ifndef ELACAPTCHA_H
#define ELACAPTCHA_H

#include <QWidget>

#include "ElaDef.h"

class ElaCaptchaPrivate;
class ELA_EXPORT ElaCaptcha : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaCaptcha)
    Q_PROPERTY_CREATE_Q_H(int, CodeLength)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BoxSize)
    Q_PROPERTY_CREATE_Q_H(int, BoxSpacing)
public:
    enum InputMode
    {
        DigitOnly = 0,
        AlphaNumeric
    };
    Q_ENUM(InputMode)

    explicit ElaCaptcha(QWidget* parent = nullptr);
    ~ElaCaptcha() override;

    void setInputMode(InputMode mode);
    InputMode getInputMode() const;

    QString getCode() const;
    void clear();

Q_SIGNALS:
    void codeCompleted(const QString& code);
    void codeChanged(const QString& code);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELACAPTCHA_H
