#ifndef ELACOPYBUTTONH
#define ELACOPYBUTTONH

#include <QPushButton>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaCopyButtonPrivate;
class ELA_EXPORT ElaCopyButton : public QPushButton
{
    Q_OBJECT
    Q_Q_CREATE(ElaCopyButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_REF_CREATE_Q_H(QString, CopyText)
    Q_PROPERTY_CREATE_Q_H(int, SuccessDuration)
    Q_PROPERTY_REF_CREATE_Q_H(QString, SuccessText)

public:
    explicit ElaCopyButton(QWidget* parent = nullptr);
    explicit ElaCopyButton(const QString& text, QWidget* parent = nullptr);
    ~ElaCopyButton();

    void setText(const QString& text);
    void setElaIcon(ElaIconType::IconName icon);
    void setSuccessIcon(ElaIconType::IconName icon);

Q_SIGNALS:
    Q_SIGNAL void copyCompleted(const QString& text);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELACOPYBUTTONH
