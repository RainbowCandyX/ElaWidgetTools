#ifndef ELAPERSONPICTURE_H
#define ELAPERSONPICTURE_H

#include <QPixmap>
#include <QWidget>

#include "ElaProperty.h"
class ElaPersonPicturePrivate;
class ELA_EXPORT ElaPersonPicture : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPersonPicture)
    Q_PROPERTY_CREATE_Q_H(int, PictureSize)
    Q_PROPERTY_CREATE_Q_H(QString, DisplayName)
    Q_PROPERTY_REF_CREATE_Q_H(QPixmap, Picture)

public:
    explicit ElaPersonPicture(QWidget* parent = nullptr);
    ~ElaPersonPicture() override;
    QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAPERSONPICTURE_H
