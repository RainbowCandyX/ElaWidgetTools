#ifndef ELASCREENCAPTURE_H
#define ELASCREENCAPTURE_H

#include <QObject>
#ifdef Q_OS_MAC
#include <QElapsedTimer>
#include <QImage>

#include "ElaProperty.h"

class ElaScreenCapture : public QObject
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QStringList, DisplayList)
    Q_PRIVATE_CREATE(int, DisplayID)
    Q_PRIVATE_CREATE(QString, LastError)
    Q_PRIVATE_CREATE(bool, IsGrabActive)
    Q_PRIVATE_CREATE(QRect, GrabArea)
    Q_PRIVATE_CREATE(int, GrabFrameRate)
    Q_PRIVATE_CREATE(bool, IsInitSuccess)
    Q_PRIVATE_CREATE(bool, IsGrabStoped)
    Q_PRIVATE_CREATE(bool, IsGrabCenter)

public:
    explicit ElaScreenCapture(QObject* parent = nullptr);
    ~ElaScreenCapture() override;
    bool initialize(int displayID);
    QImage getGrabImage() const;
    Q_SLOT void onGrabScreen();
    Q_SIGNAL void grabScreenOver(QImage img);

private:
    class Private;
    Private* d{nullptr};
    QElapsedTimer _grabTimer;
    qint64 _lastGrabTime{0};
    qint64 _cpuSleepTime{0};
    QImage _lastImage;
    int _displayWidth{0};
    int _displayHeight{0};
    void releaseInterface();
    void cpuSleep(qint64 usec);
};
#endif
#endif // ELASCREENCAPTURE_H
