#ifndef ELASPOTLIGHTPRIVATE_H
#define ELASPOTLIGHTPRIVATE_H

#include <QObject>
#include <QRectF>

#include "ElaDef.h"
#include "ElaSpotlight.h"
class QPropertyAnimation;
class ElaText;
class ElaPushButton;
class ElaSpotlight;
class ElaSpotlightPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSpotlight)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, Padding)
    Q_PROPERTY_CREATE_D(int, OverlayAlpha)
    Q_PROPERTY_CREATE_D(bool, IsCircle)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE(QRectF, SpotlightRect)
    Q_PROPERTY_CREATE(qreal, Opacity)
public:
    explicit ElaSpotlightPrivate(QObject* parent = nullptr);
    ~ElaSpotlightPrivate() override;

    void _showStep(int index, bool animate = true);
    void _updateOverlay();
    void _updateTipPosition();
    QRectF _getTargetRect(QWidget* target) const;

private:
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaSpotlight::SpotlightStep> _steps;
    int _currentStep{-1};
    QWidget* _currentTarget{nullptr};
    bool _isActive{false};
    int _maskAlpha{0};

    QWidget* _tipWidget{nullptr};
    ElaText* _tipTitle{nullptr};
    ElaText* _tipContent{nullptr};
    ElaPushButton* _prevButton{nullptr};
    ElaPushButton* _nextButton{nullptr};
    ElaText* _stepIndicator{nullptr};

    friend class ElaSpotlight;
};

#endif // ELASPOTLIGHTPRIVATE_H
