#include "ElaScreenCapture.h"
#ifdef Q_OS_MAC
#include <QDebug>
#include <mach/mach_time.h>

#include <ApplicationServices/ApplicationServices.h>

class ElaScreenCapture::Private
{
public:
    CGDirectDisplayID displayId{0};
    QVector<CGDirectDisplayID> displayIds;
};

ElaScreenCapture::ElaScreenCapture(QObject* parent)
    : QObject(parent)
    , d(new Private)
{
    _pIsInitSuccess = false;
    _pIsGrabActive = false;
    _pGrabFrameRate = 60;
    _pIsGrabStoped = true;
    _pIsGrabCenter = false;
}

ElaScreenCapture::~ElaScreenCapture()
{
    releaseInterface();
    delete d;
}

bool ElaScreenCapture::initialize(int displayID)
{
    _pIsInitSuccess = false;
    _pDisplayID = displayID;
    releaseInterface();

    // Get all displays
    uint32_t displayCount = 0;
    CGGetActiveDisplayList(0, nullptr, &displayCount);
    if (displayCount == 0)
    {
        _pLastError = "No active displays found";
        qDebug() << _pLastError;
        return false;
    }

    d->displayIds.resize(displayCount);
    CGGetActiveDisplayList(displayCount, d->displayIds.data(), &displayCount);

    _pDisplayList.clear();
    for (uint32_t i = 0; i < displayCount; i++)
    {
        CGDirectDisplayID display = d->displayIds[i];
        QString displayName = QString("Display %1 (%2x%3)")
                                  .arg(i + 1)
                                  .arg(CGDisplayPixelsWide(display))
                                  .arg(CGDisplayPixelsHigh(display));
        if (!_pDisplayList.contains(displayName))
        {
            _pDisplayList.append(displayName);
        }
    }

    if (displayID < 0 || displayID >= static_cast<int>(displayCount))
    {
        _pLastError = "Invalid display ID";
        qDebug() << _pLastError;
        return false;
    }

    d->displayId = d->displayIds[displayID];
    _displayWidth = static_cast<int>(CGDisplayPixelsWide(d->displayId));
    _displayHeight = static_cast<int>(CGDisplayPixelsHigh(d->displayId));

    _pIsInitSuccess = true;
    return true;
}

static QImage cgImageToQImage(CGImageRef cgImage)
{
    if (!cgImage)
    {
        return QImage();
    }

    size_t width = CGImageGetWidth(cgImage);
    size_t height = CGImageGetHeight(cgImage);

    QImage image(static_cast<int>(width), static_cast<int>(height), QImage::Format_ARGB32);

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        image.bits(),
        width,
        height,
        8,
        image.bytesPerLine(),
        colorSpace,
        kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);

    CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgImage);

    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);

    return image;
}

QImage ElaScreenCapture::getGrabImage() const
{
    if (!_pIsInitSuccess)
    {
        return QImage();
    }

    CGImageRef cgImage = CGDisplayCreateImage(d->displayId);
    if (!cgImage)
    {
        return QImage();
    }

    QImage image = cgImageToQImage(cgImage);
    CGImageRelease(cgImage);

    if (_pIsGrabCenter)
    {
        return image.copy(QRect((_displayWidth - _pGrabArea.width()) / 2,
                                (_displayHeight - _pGrabArea.height()) / 2,
                                _pGrabArea.width(),
                                _pGrabArea.height()));
    }
    else if (!_pGrabArea.isEmpty())
    {
        return image.copy(_pGrabArea);
    }
    return image;
}

void ElaScreenCapture::onGrabScreen()
{
    if (!_pIsInitSuccess || d->displayId == 0)
    {
        setIsGrabStoped(true);
        return;
    }

    _pIsGrabStoped = false;

    while (_pIsGrabActive)
    {
        _grabTimer.start();

        // Capture screen using CGDisplayCreateImage
        CGImageRef cgImage = CGDisplayCreateImage(d->displayId);
        if (cgImage)
        {
            _lastImage = cgImageToQImage(cgImage);
            CGImageRelease(cgImage);

            if (!_lastImage.isNull())
            {
                QImage outputImage;
                if (_pIsGrabCenter)
                {
                    outputImage = _lastImage.copy(
                        QRect((_displayWidth - _pGrabArea.width()) / 2,
                              (_displayHeight - _pGrabArea.height()) / 2,
                              _pGrabArea.width(),
                              _pGrabArea.height()));
                }
                else if (!_pGrabArea.isEmpty())
                {
                    outputImage = _lastImage.copy(_pGrabArea);
                }
                else
                {
                    outputImage = _lastImage;
                }
                Q_EMIT grabScreenOver(std::move(outputImage));
            }
        }

        if (_lastGrabTime == 0)
        {
            _lastGrabTime = _grabTimer.elapsed();
        }
        else
        {
            _lastGrabTime = (_grabTimer.elapsed() + _lastGrabTime) / 2;
        }
        _cpuSleepTime = (1000 - _lastGrabTime * _pGrabFrameRate) / _pGrabFrameRate;
        cpuSleep(_cpuSleepTime * 1000);
    }

    setIsGrabStoped(true);
}

void ElaScreenCapture::releaseInterface()
{
    d->displayId = 0;
    d->displayIds.clear();
}

void ElaScreenCapture::cpuSleep(qint64 usec)
{
    if (usec <= 0)
    {
        return;
    }
    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);
    uint64_t start = mach_absolute_time();
    uint64_t end;
    uint64_t elapsed;
    while (true)
    {
        end = mach_absolute_time();
        elapsed = (end - start) * timebase.numer / timebase.denom / 1000;
        if (static_cast<qint64>(elapsed) > usec)
        {
            break;
        }
    }
}
#endif
