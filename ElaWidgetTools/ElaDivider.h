#ifndef ELADIVIDER_H
#define ELADIVIDER_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaDividerPrivate;
class ELA_EXPORT ElaDivider : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaDivider)
    Q_PROPERTY_CREATE_Q_H(Qt::Orientation, Orientation)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(int, ContentPosition)
public:
    enum ContentPositionType
    {
        Left = 0,
        Center = 1,
        Right = 2
    };
    Q_ENUM(ContentPositionType)

    explicit ElaDivider(QWidget* parent = nullptr);
    explicit ElaDivider(const QString& text, QWidget* parent = nullptr);
    ~ElaDivider() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELADIVIDER_H
