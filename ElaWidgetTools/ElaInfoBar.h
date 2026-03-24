#ifndef ELAINFOBAR_H
#define ELAINFOBAR_H

#include <QFrame>
#include <functional>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaInfoBarPrivate;
class ELA_EXPORT ElaInfoBar : public QFrame
{
    Q_OBJECT
    Q_Q_CREATE(ElaInfoBar)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Message)
    Q_PROPERTY_CREATE_Q_H(ElaInfoBarType::InfoBarSeverity, Severity)
    Q_PROPERTY_CREATE_Q_H(bool, IsClosable)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, InfoBarIcon)
public:
    explicit ElaInfoBar(QWidget* parent = nullptr);
    explicit ElaInfoBar(ElaInfoBarType::InfoBarSeverity severity, QWidget* parent = nullptr);
    ~ElaInfoBar() override;

    void addAction(const QString& text, const std::function<void()>& callback);
    void clearActions();

    void closeInfoBar();

Q_SIGNALS:
    Q_SIGNAL void closed();
    Q_SIGNAL void closeButtonClicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

#endif // ELAINFOBAR_H
