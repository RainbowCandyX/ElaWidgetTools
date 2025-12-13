#include "ElaScreenCaptureManagerPrivate.h"
#ifdef Q_OS_MAC
ElaScreenCaptureManagerPrivate::ElaScreenCaptureManagerPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScreenCaptureManagerPrivate::~ElaScreenCaptureManagerPrivate()
{
}

ElaScreenCaptureScreenPrivate::ElaScreenCaptureScreenPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaScreenCaptureScreenPrivate::~ElaScreenCaptureScreenPrivate()
{
}
#endif
