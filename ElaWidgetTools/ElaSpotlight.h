#ifndef ELASPOTLIGHT_H
#define ELASPOTLIGHT_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaSpotlightPrivate;
class ELA_EXPORT ElaSpotlight : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSpotlight)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, Padding)
    Q_PROPERTY_CREATE_Q_H(int, OverlayAlpha)
    Q_PROPERTY_CREATE_Q_H(bool, IsCircle)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
public:
    explicit ElaSpotlight(QWidget* parent = nullptr);
    ~ElaSpotlight() override;

    void showSpotlight(QWidget* target, const QString& buttonText = "知道了");

    struct SpotlightStep {
        QWidget* target{nullptr};
        QString title;
        QString content;
        bool isCircle{false};
    };
    void setSteps(const QList<SpotlightStep>& steps);
    void start();
    void next();
    void previous();
    void finish();

    int currentStep() const;
    int stepCount() const;

Q_SIGNALS:
    Q_SIGNAL void stepChanged(int step);
    Q_SIGNAL void finished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELASPOTLIGHT_H
