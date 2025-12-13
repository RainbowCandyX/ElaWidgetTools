#ifndef ELATABLEWIDGETPRIVATE_H
#define ELATABLEWIDGETPRIVATE_H

#include <QObject>

#include "ElaProperty.h"

class ElaTableWidget;
class ElaTableWidgetStyle;

class ElaTableWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTableWidget)
public:
    explicit ElaTableWidgetPrivate(QObject* parent = nullptr);
    ~ElaTableWidgetPrivate();

private:
    ElaTableWidgetStyle* _tableWidgetStyle{nullptr};
};

#endif // ELATABLEWIDGETPRIVATE_H
