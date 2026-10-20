#ifndef ELASTEPS_H
#define ELASTEPS_H

#include <QWidget>

#include "ElaProperty.h"

class ElaStepsPrivate;
class ELA_EXPORT ElaSteps : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSteps)
    Q_PROPERTY_CREATE_Q_H(int, CurrentStep)
    Q_PROPERTY_CREATE_Q_H(int, StepCount)

public:
    explicit ElaSteps(QWidget* parent = nullptr);
    ~ElaSteps() override;

    void setStepTitles(const QStringList& titles);
    QStringList getStepTitles() const;

    void next();
    void previous();

    // 布局方向：Qt::Horizontal（默认，横向）/ Qt::Vertical（纵向，标题自动换行）
    void setStepOrientation(Qt::Orientation orientation);
    Qt::Orientation stepOrientation() const;

Q_SIGNALS:
    void currentStepChanged(int step);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void updateSize();
};

#endif // ELASTEPS_H
