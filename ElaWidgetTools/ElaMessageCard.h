#ifndef ELAMESSAGECARD_H
#define ELAMESSAGECARD_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMessageCardPrivate;
class ELA_EXPORT ElaMessageCard : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageCard)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsCompleted)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(int, ContentPixelSize)

public:
    explicit ElaMessageCard(QWidget* parent = nullptr);
    ~ElaMessageCard();

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMESSAGECARD_H
