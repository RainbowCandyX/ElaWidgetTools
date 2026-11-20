#ifndef ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELALOADINGMASK_H_
#define ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELALOADINGMASK_H_

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaLoadingMaskPrivate;
class ELA_EXPORT ElaLoadingMask : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaLoadingMask)
    Q_PROPERTY_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(int, MaskAlpha)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)
public:
    explicit ElaLoadingMask(QWidget* parent = nullptr);
    ~ElaLoadingMask() override;

    void start();
    void finish();
    bool isRunning() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

};

#endif // ELAFRAMEWORK_ELAWIDGETTOOLS_INCLUDE_ELALOADINGMASK_H_
