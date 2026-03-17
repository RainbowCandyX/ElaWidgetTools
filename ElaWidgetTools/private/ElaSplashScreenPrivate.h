#ifndef ELASPLASHSCREENPRIVATE_H
#define ELASPLASHSCREENPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
class ElaText;
class ElaProgressBar;
class ElaProgressRing;
class ElaSplashScreen;
class ElaSplashScreenPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSplashScreen)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, Minimum)
    Q_PROPERTY_CREATE_D(int, Maximum)
    Q_PROPERTY_CREATE_D(int, Value)
    Q_PROPERTY_CREATE_D(bool, IsShowProgressBar)
    Q_PROPERTY_CREATE_D(bool, IsShowProgressRing)
    Q_PROPERTY_CREATE_D(bool, IsClosable)
public:
    explicit ElaSplashScreenPrivate(QObject* parent = nullptr);
    ~ElaSplashScreenPrivate() override;

    void _doShowAnimation();
    void _doCloseAnimation();

private:
    ElaThemeType::ThemeMode _themeMode;
    QPixmap _logo;
    ElaText* _titleText{nullptr};
    ElaText* _subTitleText{nullptr};
    ElaText* _statusText{nullptr};
    ElaProgressBar* _progressBar{nullptr};
    ElaProgressRing* _progressRing{nullptr};
    QPoint _dragStartPos;
    bool _isDragging{false};
};

#endif // ELASPLASHSCREENPRIVATE_H
