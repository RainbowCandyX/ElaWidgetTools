#ifndef ELAWATERMARKPRIVATE_H
#define ELAWATERMARKPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
#include "ElaWatermark.h"
class ElaWatermarkPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWatermark)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
    Q_PROPERTY_CREATE_D(qreal, Opacity)
    Q_PROPERTY_CREATE_D(qreal, Rotation)
    Q_PROPERTY_CREATE_D(int, GapX)
    Q_PROPERTY_CREATE_D(int, GapY)
    Q_PROPERTY_CREATE_D(QColor, TextColor)
    Q_PROPERTY_CREATE_D(QImage, Image)
    Q_PROPERTY_CREATE_D(int, ImageWidth)
    Q_PROPERTY_CREATE_D(int, ImageHeight)
public:
    explicit ElaWatermarkPrivate(QObject* parent = nullptr);
    ~ElaWatermarkPrivate() override;

    void rebuildPattern();

private:
    ElaThemeType::ThemeMode _themeMode;
    QPixmap _patternPixmap;
    bool _patternDirty{true};
};

#endif // ELAWATERMARKPRIVATE_H
