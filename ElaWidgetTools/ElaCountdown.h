#ifndef ELACOUNTDOWN_H
#define ELACOUNTDOWN_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaCountdownPrivate;
class ELA_EXPORT ElaCountdown : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaCountdown)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowDays)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowHours)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowMinutes)
    Q_PROPERTY_CREATE_Q_H(bool, IsShowSeconds)
    Q_PROPERTY_CREATE_Q_H(int, DigitWidth)
    Q_PROPERTY_CREATE_Q_H(int, DigitHeight)
    Q_PROPERTY_CREATE_Q_H(int, DigitSpacing)
    Q_PROPERTY_CREATE_Q_H(int, FontPixelSize)
public:
    explicit ElaCountdown(QWidget* parent = nullptr);
    ~ElaCountdown() override;

    void setTargetDateTime(const QDateTime& dateTime);
    QDateTime getTargetDateTime() const;

    void setRemainingSeconds(qint64 seconds);
    qint64 getRemainingSeconds() const;

    void start();
    void pause();
    void resume();
    void stop();

    bool isRunning() const;

Q_SIGNALS:
    Q_SIGNAL void timeout();
    Q_SIGNAL void tick(qint64 remainingSeconds);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    void _updateSize();
};

#endif // ELACOUNTDOWN_H
