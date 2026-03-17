#ifndef ELARATINGCONTROLPRIVATE_H
#define ELARATINGCONTROLPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaRatingControl;
class ElaRatingControlPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaRatingControl)
    Q_PROPERTY_CREATE_D(int, MaxRating)
    Q_PROPERTY_CREATE_D(int, Rating)
    Q_PROPERTY_CREATE_D(int, StarSize)
    Q_PROPERTY_CREATE_D(int, Spacing)
    Q_PROPERTY_CREATE_D(bool, IsReadOnly)
public:
    explicit ElaRatingControlPrivate(QObject* parent = nullptr);
    ~ElaRatingControlPrivate() override;

private:
    int _hoveredStar{-1};
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELARATINGCONTROLPRIVATE_H
