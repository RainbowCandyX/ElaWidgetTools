#ifndef ELAMESSAGECARDPRIVATE_H
#define ELAMESSAGECARDPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaMessageCard;
class ElaMessageCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMessageCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, IsCompleted)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(int, TitlePixelSize)
    Q_PROPERTY_CREATE_D(int, ContentPixelSize)

public:
    explicit ElaMessageCardPrivate(QObject* parent = nullptr);
    ~ElaMessageCardPrivate();

    ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAMESSAGECARDPRIVATE_H
