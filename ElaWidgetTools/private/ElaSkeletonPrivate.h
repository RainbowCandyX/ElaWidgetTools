#ifndef ELASKELETONPRIVATE_H
#define ELASKELETONPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaSkeleton.h"
class QVariantAnimation;
class ElaSkeletonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSkeleton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsAnimated)
    Q_PROPERTY_CREATE(qreal, ShimmerPosition)
public:
    explicit ElaSkeletonPrivate(QObject* parent = nullptr);
    ~ElaSkeletonPrivate() override;

private:
    ElaSkeleton::SkeletonType _skeletonType{ElaSkeleton::Text};
    ElaThemeType::ThemeMode _themeMode;
    QVariantAnimation* _shimmerAnimation{nullptr};
    void _startShimmerAnimation();
    void _stopShimmerAnimation();
};

#endif // ELASKELETONPRIVATE_H
