#ifndef ELATOAST_H
#define ELATOAST_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaToastPrivate;
class ELA_EXPORT ElaToast : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaToast)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)

public:
    enum ToastType
    {
        Success = 0,
        Info,
        Warning,
        Error
    };
    Q_ENUM(ToastType)

    static void success(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr);
    static void info(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr);
    static void warning(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr);
    static void error(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    explicit ElaToast(ToastType type, const QString& text, int displayMsec, QWidget* parent = nullptr);
    ~ElaToast();
};

#endif // ELATOAST_H
