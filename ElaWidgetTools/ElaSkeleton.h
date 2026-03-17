#ifndef ELASKELETON_H
#define ELASKELETON_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaSkeletonPrivate;
class ELA_EXPORT ElaSkeleton : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSkeleton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsAnimated)
public:
    enum SkeletonType
    {
        Text = 0,
        Circle,
        Rectangle
    };
    Q_ENUM(SkeletonType)

    explicit ElaSkeleton(QWidget* parent = nullptr);
    ~ElaSkeleton() override;

    void setSkeletonType(SkeletonType type);
    SkeletonType getSkeletonType() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELASKELETON_H
