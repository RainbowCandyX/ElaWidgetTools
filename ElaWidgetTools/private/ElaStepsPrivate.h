#ifndef ELASTEPSPRIVATE_H
#define ELASTEPSPRIVATE_H

#include <QObject>
#include <QStringList>

#include "ElaDef.h"

class ElaSteps;
class ElaStepsPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSteps)
    Q_PROPERTY_CREATE_D(int, CurrentStep)
    Q_PROPERTY_CREATE_D(int, StepCount)

public:
    explicit ElaStepsPrivate(QObject* parent = nullptr);
    ~ElaStepsPrivate() override;

    QStringList _stepTitles;
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELASTEPSPRIVATE_H
