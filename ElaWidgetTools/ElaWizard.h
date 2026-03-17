#ifndef ELAWIZARD_H
#define ELAWIZARD_H

#include <QWidget>

#include "ElaProperty.h"

class ElaWizardPrivate;
class ELA_EXPORT ElaWizard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaWizard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, CurrentStep)

public:
    explicit ElaWizard(QWidget* parent = nullptr);
    ~ElaWizard() override;

    void addStep(const QString& title, QWidget* page);
    void next();
    void previous();
    void finish();
    int getStepCount() const;

Q_SIGNALS:
    Q_SIGNAL void currentStepChanged(int step);
    Q_SIGNAL void finished();
    Q_SIGNAL void cancelled();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELAWIZARD_H
