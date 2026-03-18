#ifndef ELAWATERMARK_H
#define ELAWATERMARK_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaWatermarkPrivate;
class ELA_EXPORT ElaWatermark : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaWatermark)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(int, FontPixelSize)
    Q_PROPERTY_CREATE_Q_H(qreal, Opacity)
    Q_PROPERTY_CREATE_Q_H(qreal, Rotation)
    Q_PROPERTY_CREATE_Q_H(int, GapX)
    Q_PROPERTY_CREATE_Q_H(int, GapY)
    Q_PROPERTY_REF_CREATE_Q_H(QColor, TextColor)
    Q_PROPERTY_REF_CREATE_Q_H(QImage, Image)
    Q_PROPERTY_CREATE_Q_H(int, ImageWidth)
    Q_PROPERTY_CREATE_Q_H(int, ImageHeight)
public:
    explicit ElaWatermark(QWidget* parent = nullptr);
    explicit ElaWatermark(const QString& text, QWidget* parent = nullptr);
    ~ElaWatermark() override;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWATERMARK_H
