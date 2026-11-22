#ifndef ELATABWIDGET_H
#define ELATABWIDGET_H

#include <QTabWidget>

#include "ElaProperty.h"

class ElaTabWidgetPrivate;
class ElaTabBar;
class ELA_EXPORT ElaTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTabWidget)
    Q_PROPERTY_CREATE_Q_H(bool, IsTabTransparent);
    Q_PROPERTY_CREATE_Q_H(bool, IsContainerAcceptDrops);
    Q_PROPERTY_CREATE_Q_H(QSize, TabSize)
    Q_PROPERTY_CREATE_Q_H(bool, TabAddButtonVisible)

public:
    explicit ElaTabWidget(QWidget* parent = nullptr);
    ~ElaTabWidget() override;
    void setTabPosition(TabPosition position);
    void removeTab(int index); // 如果调用了QTabWidget::removeTab()，会导致ElaTabWidget内部的_allTabWidgetList残留，析构时造成内存泄漏
    void clear();
    ElaTabBar* getElaTabBar();

Q_SIGNALS:
    Q_SIGNAL void tabAddRequested();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
    virtual void tabInserted(int index);

private:
    friend class ElaCustomTabWidget;
};

#endif // ELATABWIDGET_H
