#ifndef ELATABLEWIDGET_H
#define ELATABLEWIDGET_H

#include <QTableWidget>

#include "ElaProperty.h"

class ElaTableWidgetPrivate;
class ELA_EXPORT ElaTableWidget : public QTableWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTableWidget)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
    explicit ElaTableWidget(QWidget* parent = nullptr);
    ~ElaTableWidget();

    void removeRows(int row, int count);

Q_SIGNALS:
    Q_SIGNAL void tableWidgetShow();
    Q_SIGNAL void tableWidgetHide();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
};

#endif // ELATABLEWIDGET_H
