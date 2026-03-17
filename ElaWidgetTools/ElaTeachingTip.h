#ifndef ELATEACHINGTIP_H
#define ELATEACHINGTIP_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaTeachingTipPrivate;
class ELA_EXPORT ElaTeachingTip : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTeachingTip)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, TipIcon)
    Q_PROPERTY_CREATE_Q_H(QPixmap, HeroImage)
    Q_PROPERTY_CREATE_Q_H(bool, IsLightDismiss)
public:
    enum TailPosition
    {
        Auto = 0,
        Top,
        Bottom,
        Left,
        Right
    };
    Q_ENUM(TailPosition)

    explicit ElaTeachingTip(QWidget* parent = nullptr);
    ~ElaTeachingTip() override;

    void setTailPosition(TailPosition position);
    TailPosition getTailPosition() const;

    void setTarget(QWidget* target);
    QWidget* getTarget() const;

    void setCloseButtonVisible(bool visible);

    void addAction(const QString& text, const std::function<void()>& callback);
    void clearActions();

    void showTip();
    void closeTip();

Q_SIGNALS:
    Q_SIGNAL void closed();
    Q_SIGNAL void closeButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
};

#endif // ELATEACHINGTIP_H
