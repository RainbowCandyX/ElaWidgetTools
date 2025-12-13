#include "ElaScreenCapture.h"
#ifdef Q_OS_MAC
#include <QDebug>
#include <mach/mach_time.h>

#include <ScreenCaptureKit/ScreenCaptureKit.h>
#include <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>

class ElaScreenCapture::Private
{
public:
    CGDirectDisplayID displayId{0};
    QVector<CGDirectDisplayID> displayIds;
    SCDisplay* scDisplay{nil};
    NSArray<SCDisplay*>* scDisplays{nil};
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

    __block bool success = false;
    __block NSError* error = nil;
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);

    [SCShareableContent getShareableContentWithCompletionHandler:^(SCShareableContent* content, NSError* err) {
        if (err) {
            error = err;
            dispatch_semaphore_signal(semaphore);
            return;
        }

        if (content.displays.count == 0) {
            dispatch_semaphore_signal(semaphore);
            return;
        }

        d->scDisplays = [content.displays retain];
        success = true;
        dispatch_semaphore_signal(semaphore);
    }];

    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    dispatch_release(semaphore);

    if (!success || d->scDisplays == nil || d->scDisplays.count == 0) {
        _pLastError = error ? QString::fromNSString(error.localizedDescription) : "No active displays found";
        qDebug() << _pLastError;
        return false;
    }

    _pDisplayList.clear();
    for (NSUInteger i = 0; i < d->scDisplays.count; i++) {
        SCDisplay* display = d->scDisplays[i];
        QString displayName = QString("Display %1 (%2x%3)")
                                  .arg(i + 1)
                                  .arg(display.width)
                                  .arg(display.height);
        if (!_pDisplayList.contains(displayName)) {
            _pDisplayList.append(displayName);
        }
    }

    if (displayID < 0 || displayID >= static_cast<int>(d->scDisplays.count)) {
        _pLastError = "Invalid display ID";
        qDebug() << _pLastError;
        return false;
    }

    d->scDisplay = d->scDisplays[displayID];
    d->displayId = d->scDisplay.displayID;
    _displayWidth = static_cast<int>(d->scDisplay.width);
    _displayHeight = static_cast<int>(d->scDisplay.height);

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

static CGImageRef captureDisplayUsingScreenCaptureKit(SCDisplay* display)
{
    if (!display) {
        return nullptr;
    }

    __block CGImageRef capturedImage = nullptr;
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);

    SCContentFilter* filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:@[]];
    SCStreamConfiguration* config = [[SCStreamConfiguration alloc] init];

    [SCScreenshotManager captureImageWithFilter:filter
                                  configuration:config
                              completionHandler:^(CGImageRef sampleBuffer, NSError* error) {
        if (sampleBuffer && !error) {
            capturedImage = CGImageRetain(sampleBuffer);
        }
        dispatch_semaphore_signal(semaphore);
    }];

    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    dispatch_release(semaphore);

    [filter release];
    [config release];

    return capturedImage;
}

QImage ElaScreenCapture::getGrabImage() const
{
    if (!_pIsInitSuccess || !d->scDisplay)
    {
        return QImage();
    }

    CGImageRef cgImage = captureDisplayUsingScreenCaptureKit(d->scDisplay);
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
    if (!_pIsInitSuccess || !d->scDisplay)
    {
        setIsGrabStoped(true);
        return;
    }

    _pIsGrabStoped = false;

    while (_pIsGrabActive)
    {
        _grabTimer.start();

        CGImageRef cgImage = captureDisplayUsingScreenCaptureKit(d->scDisplay);
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
    if (d->scDisplays) {
        [d->scDisplays release];
        d->scDisplays = nil;
    }
    d->scDisplay = nil;
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
