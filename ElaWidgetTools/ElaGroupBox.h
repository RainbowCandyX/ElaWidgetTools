#ifndef ELAGROUPBOX_H
#define ELAGROUPBOX_H

#include <QGroupBox>

#include "ElaProperty.h"
class ElaGroupBoxPrivate;
class ELA_EXPORT ElaGroupBox : public QGroupBox
{
    Q_OBJECT
    Q_Q_CREATE(ElaGroupBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaGroupBox(QWidget* parent = nullptr);
    explicit ElaGroupBox(const QString& title, QWidget* parent = nullptr);
    ~ElaGroupBox() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAGROUPBOX_H
