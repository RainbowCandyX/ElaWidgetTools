#ifndef ELACHATBUBBLEPRIVATE_H
#define ELACHATBUBBLEPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QRect>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaChatBubble;
class ElaChatBubblePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaChatBubble)
    Q_PROPERTY_CREATE_D(QString, MessageText)
    Q_PROPERTY_CREATE_D(QString, SenderName)
    Q_PROPERTY_CREATE_D(QString, Timestamp)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, AvatarSize)
    Q_PROPERTY_CREATE_D(int, MaxBubbleWidth)
public:
    explicit ElaChatBubblePrivate(QObject* parent = nullptr);
    ~ElaChatBubblePrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaChatBubble::BubbleDirection _direction{ElaChatBubble::Left};
    ElaChatBubble::MessageStatus _status{ElaChatBubble::None};
    QPixmap _avatar;
    QPixmap _messageImage;
    QPixmap _scaledImage;
    int _imageMaxWidth{280};
    QRect _imageRect;
    QColor _bubbleColor;
};

#endif // ELACHATBUBBLEPRIVATE_H
