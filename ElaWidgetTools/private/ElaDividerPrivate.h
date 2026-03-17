#ifndef ELADIVIDERPRIVATE_H
#define ELADIVIDERPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaDivider.h"
class ElaDividerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaDivider)
    Q_PROPERTY_CREATE_D(Qt::Orientation, Orientation)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE_D(int, ContentPosition)
public:
    explicit ElaDividerPrivate(QObject* parent = nullptr);
    ~ElaDividerPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELADIVIDERPRIVATE_H
