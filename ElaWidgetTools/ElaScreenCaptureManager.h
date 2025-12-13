#ifndef ELASCREENCAPTUREMANAGER_H
#define ELASCREENCAPTUREMANAGER_H

#include <QWidget>
#ifdef Q_OS_MAC
#include "ElaProperty.h"
#include "ElaSingleton.h"

class ElaScreenCaptureManagerPrivate;
class ELA_EXPORT ElaScreenCaptureManager : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(ElaScreenCaptureManager)
    Q_SINGLETON_CREATE_H(ElaScreenCaptureManager);

private:
    explicit ElaScreenCaptureManager(QObject* parent = nullptr);
    ~ElaScreenCaptureManager() override;

public:
    QStringList getDisplayList() const;
    QImage grabScreenToImage() const;
    void startGrabScreen();
    void stopGrabScreen();
    bool getIsGrabScreen() const;
    bool setDisplayID(int displayID);
    int getDisplayID() const;
    void setGrabArea(int width, int height);
    void setGrabArea(int x, int y, int width, int height);
    QRect getGrabArea() const;
    void setGrabFrameRate(int frameRateValue);
    int getGrabFrameRate() const;
Q_SIGNALS:
    Q_SIGNAL void grabImageUpdate(QImage img);
};

class ElaScreenCaptureScreenPrivate;
class ELA_EXPORT ElaScreenCaptureScreen : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaScreenCaptureScreen)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaScreenCaptureScreen(QWidget* parent = nullptr);
    ~ElaScreenCaptureScreen();
    void setIsSyncGrabSize(bool isSyncGrabSize);
    bool getIsSyncGrabSize() const;

protected:
    void paintEvent(QPaintEvent* event) override;
};
#endif
#endif // ELASCREENCAPTUREMANAGER_H
