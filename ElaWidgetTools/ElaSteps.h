#ifndef ELASTEPS_H
#define ELASTEPS_H

#include <QWidget>

#include "ElaProperty.h"

class ElaStepsPrivate;
class QMouseEvent;
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

    // 是否启用点击步骤跳转（含悬停高亮/手型光标），默认 true
    void setStepClickEnabled(bool enabled);
    bool getStepClickEnabled() const;

Q_SIGNALS:
    void currentStepChanged(int step);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    int stepAtPoint(const QPoint& pos) const;
    void updateSize();
};

#endif // ELASTEPS_H
