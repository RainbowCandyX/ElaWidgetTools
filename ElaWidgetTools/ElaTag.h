#ifndef ELATAG_H
#define ELATAG_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaTagPrivate;
class ELA_EXPORT ElaTag : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTag)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_REF_CREATE_Q_H(QString, TagText)
    Q_PROPERTY_CREATE_Q_H(bool, IsClosable)
    Q_PROPERTY_CREATE_Q_H(bool, IsCheckable)
    Q_PROPERTY_CREATE_Q_H(bool, IsChecked)
public:
    enum TagColor
    {
        Default = 0,
        Primary,
        Success,
        Warning,
        Danger
    };
    Q_ENUM(TagColor)

    explicit ElaTag(QWidget* parent = nullptr);
    explicit ElaTag(const QString& text, QWidget* parent = nullptr);
    ~ElaTag() override;

    void setTagColor(TagColor color);
    TagColor getTagColor() const;

Q_SIGNALS:
    void closed();
    void clicked();
    void checkedChanged(bool checked);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELATAG_H
