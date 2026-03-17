#ifndef ELAPAGINATION_H
#define ELAPAGINATION_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaPaginationPrivate;
class ELA_EXPORT ElaPagination : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaPagination)
    Q_PROPERTY_CREATE_Q_H(int, CurrentPage)
    Q_PROPERTY_CREATE_Q_H(int, TotalPages)
    Q_PROPERTY_CREATE_Q_H(int, ButtonSize)
    Q_PROPERTY_CREATE_Q_H(int, PagerCount)
    Q_PROPERTY_CREATE_Q_H(bool, JumperVisible)
Q_SIGNALS:
    void currentPageChanged(int page);

public:
    explicit ElaPagination(QWidget* parent = nullptr);
    ~ElaPagination() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELAPAGINATION_H
