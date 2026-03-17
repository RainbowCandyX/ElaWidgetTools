#ifndef ELASPLASHSCREEN_H
#define ELASPLASHSCREEN_H

#include <QWidget>

#include "ElaProperty.h"

class ElaSplashScreenPrivate;
class ELA_EXPORT ElaSplashScreen : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSplashScreen)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, Minimum)
    Q_PROPERTY_CREATE_Q_H(int, Maximum)
    Q_PROPERTY_CREATE_Q_H(int, Value)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowProgressBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowProgressRing)
    Q_PROPERTY_CREATE_Q_H(bool, IsClosable)
public:
    explicit ElaSplashScreen(QWidget* parent = nullptr);
    explicit ElaSplashScreen(const QPixmap& logo, QWidget* parent = nullptr);
    ~ElaSplashScreen() override;

    void setLogo(const QPixmap& logo);
    void setTitle(const QString& title);
    void setSubTitle(const QString& subTitle);
    void setStatusText(const QString& text);

    void show();
    void close();
    void finish(QWidget* mainWindow);

Q_SIGNALS:
    Q_SIGNAL void closed();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // ELASPLASHSCREEN_H
