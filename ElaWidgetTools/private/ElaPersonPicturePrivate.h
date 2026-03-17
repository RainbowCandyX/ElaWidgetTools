#ifndef ELAPERSONPICTUREPRIVATE_H
#define ELAPERSONPICTUREPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "ElaDef.h"
class ElaPersonPicture;
class ElaPersonPicturePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaPersonPicture)
    Q_PROPERTY_CREATE_D(int, PictureSize)
    Q_PROPERTY_CREATE_D(QString, DisplayName)
    Q_PROPERTY_CREATE_D(QPixmap, Picture)

public:
    explicit ElaPersonPicturePrivate(QObject* parent = nullptr);
    ~ElaPersonPicturePrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAPERSONPICTUREPRIVATE_H
