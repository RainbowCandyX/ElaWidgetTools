#ifndef ELAEXPANDER_H
#define ELAEXPANDER_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaExpanderPrivate;
class ELA_EXPORT ElaExpander : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaExpander)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, SubTitle)
    Q_PROPERTY_CREATE_Q_H(ElaIconType::IconName, HeaderIcon)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)
public:
    enum ExpandDirection
    {
        Down = 0,
        Up
    };
    Q_ENUM(ExpandDirection)

    explicit ElaExpander(QWidget* parent = nullptr);
    explicit ElaExpander(const QString& title, QWidget* parent = nullptr);
    ~ElaExpander() override;

    void setExpandDirection(ExpandDirection direction);
    ExpandDirection getExpandDirection() const;

    void setContentWidget(QWidget* widget);
    QWidget* getContentWidget() const;

    void setHeaderWidget(QWidget* widget);

    void setIsExpanded(bool expanded);
    bool getIsExpanded() const;

Q_SIGNALS:
    Q_SIGNAL void expandStateChanged(bool expanded);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool event(QEvent* event) override;
};

#endif // ELAEXPANDER_H
