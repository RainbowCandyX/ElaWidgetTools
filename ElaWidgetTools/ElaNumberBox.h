#ifndef ELANUMBERBOX_H
#define ELANUMBERBOX_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaNumberBoxPrivate;
class ELA_EXPORT ElaNumberBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaNumberBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(double, Value)
    Q_PROPERTY_CREATE_Q_H(double, Minimum)
    Q_PROPERTY_CREATE_Q_H(double, Maximum)
    Q_PROPERTY_CREATE_Q_H(double, Step)
    Q_PROPERTY_CREATE_Q_H(int, Decimals)
    Q_PROPERTY_CREATE_Q_H(bool, IsWrapping)
public:
    explicit ElaNumberBox(QWidget* parent = nullptr);
    ~ElaNumberBox() override;

    void stepUp();
    void stepDown();

Q_SIGNALS:
    Q_SIGNAL void valueChanged(double value);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELANUMBERBOX_H
