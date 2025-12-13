#include "ElaScreenCaptureManager.h"
#ifdef Q_OS_MAC
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QThread>

#include "ElaScreenCapture.h"
#include "ElaScreenCaptureManagerPrivate.h"

Q_SINGLETON_CREATE_CPP(ElaScreenCaptureManager);
ElaScreenCaptureManager::ElaScreenCaptureManager(QObject* parent)
    : QObject{parent}, d_ptr(new ElaScreenCaptureManagerPrivate())
{
    Q_D(ElaScreenCaptureManager);
    d->q_ptr = this;
    d->_captureThread = new QThread(this);
    d->_screenCapture = new ElaScreenCapture();

    QScreen* screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen ? screen->size() : QSize(1920, 1080);
    setGrabArea(0, 0, screenSize.width(), screenSize.height());

    bool ret = d->_screenCapture->initialize(0);
    if (!ret)
    {
        for (int i = 1; i < d->_screenCapture->getDisplayList().count(); i++)
        {
            ret = d->_screenCapture->initialize(i);
            if (ret)
            {
                break;
            }
        }
    }
    if (!ret)
    {
        d->_screenCapture->initialize(0);
        qCritical() << "No available screenshot devices";
    }
    d->_screenCapture->moveToThread(d->_captureThread);
    d->_captureThread->start();
    connect(d, &ElaScreenCaptureManagerPrivate::grabScreen, d->_screenCapture, &ElaScreenCapture::onGrabScreen);
    connect(d->_screenCapture, &ElaScreenCapture::grabScreenOver, this, &ElaScreenCaptureManager::grabImageUpdate);
}

ElaScreenCaptureManager::~ElaScreenCaptureManager()
{
    Q_D(ElaScreenCaptureManager);
    if (d->_screenCapture)
    {
        d->_screenCapture->setIsGrabActive(false);
    }
    if (d->_captureThread->isRunning())
    {
        d->_captureThread->quit();
        d->_captureThread->wait();
    }
    delete d->_screenCapture;
}

QStringList ElaScreenCaptureManager::getDisplayList() const
{
    Q_D(const ElaScreenCaptureManager);
    return d->_screenCapture->getDisplayList();
}

QImage ElaScreenCaptureManager::grabScreenToImage() const
{
    Q_D(const ElaScreenCaptureManager);
    if (!d->_screenCapture->getIsInitSuccess())
    {
        return QImage();
    }
    return d->_screenCapture->getGrabImage();
}

void ElaScreenCaptureManager::startGrabScreen()
{
    Q_D(ElaScreenCaptureManager);
    d->_isAllowedGrabScreen = true;
    if (!d->_screenCapture->getIsGrabActive())
    {
        d->_screenCapture->setIsGrabActive(true);
        Q_EMIT d->grabScreen();
    }
}

void ElaScreenCaptureManager::stopGrabScreen()
{
    Q_D(ElaScreenCaptureManager);
    d->_isAllowedGrabScreen = false;
    d->_screenCapture->setIsGrabActive(false);
}

bool ElaScreenCaptureManager::getIsGrabScreen() const
{
    Q_D(const ElaScreenCaptureManager);
    return d->_screenCapture->getIsGrabActive();
}

bool ElaScreenCaptureManager::setDisplayID(int displayID)
{
    Q_D(ElaScreenCaptureManager);
    if (displayID < 0 || d->_screenCapture->getDisplayList().count() <= displayID)
    {
        return false;
    }
    d->_screenCapture->setIsGrabActive(false);
    while (!d->_screenCapture->getIsGrabStoped())
    {
        QApplication::processEvents();
    }
    if (d->_screenCapture->initialize(displayID))
    {
        if (d->_isAllowedGrabScreen)
        {
            d->_screenCapture->setIsGrabActive(true);
            Q_EMIT d->grabScreen();
        }
        return true;
    }
    return false;
}

int ElaScreenCaptureManager::getDisplayID() const
{
    Q_D(const ElaScreenCaptureManager);
    return d->_screenCapture->getDisplayID();
}

void ElaScreenCaptureManager::setGrabArea(int width, int height)
{
    Q_D(ElaScreenCaptureManager);
    QScreen* screen = QGuiApplication::primaryScreen();
    int maxWidth = screen ? screen->size().width() : 1920;
    int maxHeight = screen ? screen->size().height() : 1080;
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_screenCapture->setIsGrabCenter(true);
    d->_screenCapture->setGrabArea(QRect(0, 0, width, height));
}

void ElaScreenCaptureManager::setGrabArea(int x, int y, int width, int height)
{
    Q_D(ElaScreenCaptureManager);
    QScreen* screen = QGuiApplication::primaryScreen();
    int maxWidth = screen ? screen->size().width() : 1920;
    int maxHeight = screen ? screen->size().height() : 1080;
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_screenCapture->setIsGrabCenter(false);
    d->_screenCapture->setGrabArea(QRect(x, y, width, height));
}

QRect ElaScreenCaptureManager::getGrabArea() const
{
    Q_D(const ElaScreenCaptureManager);
    return d->_screenCapture->getGrabArea();
}

void ElaScreenCaptureManager::setGrabFrameRate(int frameRateValue)
{
    Q_D(ElaScreenCaptureManager);
    if (frameRateValue > 0)
    {
        d->_screenCapture->setGrabFrameRate(frameRateValue);
    }
}

int ElaScreenCaptureManager::getGrabFrameRate() const
{
    Q_D(const ElaScreenCaptureManager);
    return d->_screenCapture->getGrabFrameRate();
}

Q_PROPERTY_CREATE_Q_CPP(ElaScreenCaptureScreen, int, BorderRadius)
ElaScreenCaptureScreen::ElaScreenCaptureScreen(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaScreenCaptureScreenPrivate())
{
    Q_D(ElaScreenCaptureScreen);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    d->_captureManager = ElaScreenCaptureManager::getInstance();
    setFixedSize(700, 500);
    connect(d->_captureManager, &ElaScreenCaptureManager::grabImageUpdate, this, [=](QImage img) {
        if (isVisible())
        {
            d->_img = std::move(img);
            update();
        }
    });
}

ElaScreenCaptureScreen::~ElaScreenCaptureScreen()
{
}

void ElaScreenCaptureScreen::paintEvent(QPaintEvent* event)
{
    Q_D(ElaScreenCaptureScreen);
    if (d->_captureManager->getIsGrabScreen())
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter.drawImage(rect(), d->_img);
        painter.restore();
    }
}

void ElaScreenCaptureScreen::setIsSyncGrabSize(bool isSyncGrabSize)
{
    Q_D(ElaScreenCaptureScreen);
    if (isSyncGrabSize)
    {
        setFixedSize(d->_captureManager->getGrabArea().size());
    }
}

bool ElaScreenCaptureScreen::getIsSyncGrabSize() const
{
    Q_D(const ElaScreenCaptureScreen);
    return d->_isSyncGrabSize;
}
#endif
