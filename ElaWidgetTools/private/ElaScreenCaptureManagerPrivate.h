#ifndef ELASCREENCAPTUREMANAGERPRIVATE_H
#define ELASCREENCAPTUREMANAGERPRIVATE_H
#include <QObject>
#ifdef Q_OS_MAC
#include "ElaProperty.h"
#include <QImage>
class ElaScreenCapture;
class ElaScreenCaptureManager;
class ElaScreenCaptureManagerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScreenCaptureManager)
public:
    explicit ElaScreenCaptureManagerPrivate(QObject* parent = nullptr);
    ~ElaScreenCaptureManagerPrivate();

private:
    Q_SIGNAL void grabScreen();
    bool _isAllowedGrabScreen{false};
    ElaScreenCapture* _screenCapture{nullptr};
    QThread* _captureThread{nullptr};
};

class ElaScreenCaptureScreen;
class ElaScreenCaptureScreenPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaScreenCaptureScreen)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaScreenCaptureScreenPrivate(QObject* parent = nullptr);
    ~ElaScreenCaptureScreenPrivate();

private:
    ElaScreenCaptureManager* _captureManager{nullptr};
    bool _isSyncGrabSize{false};
    QImage _img;
};
#endif
#endif // ELASCREENCAPTUREMANAGERPRIVATE_H
