#ifndef ELARATINGCONTROL_H
#define ELARATINGCONTROL_H

#include <QWidget>

#include "ElaProperty.h"
class ElaRatingControlPrivate;
class ELA_EXPORT ElaRatingControl : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaRatingControl)
    Q_PROPERTY_CREATE_Q_H(int, MaxRating)
    Q_PROPERTY_CREATE_Q_H(int, Rating)
    Q_PROPERTY_CREATE_Q_H(int, StarSize)
    Q_PROPERTY_CREATE_Q_H(int, Spacing)
    Q_PROPERTY_CREATE_Q_H(bool, IsReadOnly)
public:
    explicit ElaRatingControl(QWidget* parent = nullptr);
    ~ElaRatingControl() override;

Q_SIGNALS:
    Q_SIGNAL void ratingChanged(int rating);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELARATINGCONTROL_H
