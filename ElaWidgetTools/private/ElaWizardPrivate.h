#ifndef ELAWIZARDPRIVATE_H
#define ELAWIZARDPRIVATE_H

#include <QObject>
#include <QStringList>

#include "ElaDef.h"

class ElaWizard;
class QStackedWidget;
class ElaWizardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaWizard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, CurrentStep)

public:
    explicit ElaWizardPrivate(QObject* parent = nullptr);
    ~ElaWizardPrivate() override;

    QStringList _stepTitles;
    QList<QWidget*> _stepPages;
    QStackedWidget* _stackedWidget{nullptr};
    ElaThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{10};

    QRect _cancelButtonRect;
    QRect _prevButtonRect;
    QRect _nextButtonRect;
};

#endif // ELAWIZARDPRIVATE_H
