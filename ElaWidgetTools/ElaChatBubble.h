#ifndef ELACHATBUBBLE_H
#define ELACHATBUBBLE_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaChatBubblePrivate;
class ELA_EXPORT ElaChatBubble : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaChatBubble)
    Q_PROPERTY_REF_CREATE_Q_H(QString, MessageText)
    Q_PROPERTY_REF_CREATE_Q_H(QString, SenderName)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Timestamp)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, AvatarSize)
    Q_PROPERTY_CREATE_Q_H(int, MaxBubbleWidth)
public:
    enum BubbleDirection
    {
        Left = 0,
        Right
    };
    Q_ENUM(BubbleDirection)

    enum MessageStatus
    {
        None = 0,
        Sending,
        Sent,
        Read,
        Failed
    };
    Q_ENUM(MessageStatus)

    explicit ElaChatBubble(QWidget* parent = nullptr);
    ~ElaChatBubble() override;

    void setDirection(BubbleDirection direction);
    BubbleDirection getDirection() const;

    void setStatus(MessageStatus status);
    MessageStatus getStatus() const;

    void setAvatar(const QPixmap& avatar);
    QPixmap getAvatar() const;

    void setBubbleColor(const QColor& color);
    QColor getBubbleColor() const;

    void setMessageImage(const QPixmap& image);
    QPixmap getMessageImage() const;

    void setImageMaxWidth(int width);
    int getImageMaxWidth() const;

    QSize sizeHint() const override;

Q_SIGNALS:
    Q_SIGNAL void imageDoubleClicked(const QPixmap& image);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // ELACHATBUBBLE_H
