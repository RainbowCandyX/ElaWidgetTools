#include "ElaTableWidget.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/ElaTableWidgetStyle.h"
#include "ElaScrollBar.h"
#include "ElaTableWidgetPrivate.h"

ElaTableWidget::ElaTableWidget(QWidget* parent)
    : QTableWidget(parent), d_ptr(new ElaTableWidgetPrivate())
{
    Q_D(ElaTableWidget);
    d->q_ptr = this;
    setMouseTracking(true);
    setObjectName("ElaTableWidget");
    setStyleSheet(
        "QTableWidget{background-color:transparent;}"
        "QHeaderView{background-color:transparent;border:0px;}");
    setShowGrid(false);
    setVerticalScrollBar(new ElaScrollBar(this));
    setHorizontalScrollBar(new ElaScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d->_tableWidgetStyle = new ElaTableWidgetStyle(style());
    setStyle(d->_tableWidgetStyle);
}

ElaTableWidget::~ElaTableWidget()
{
    Q_D(ElaTableWidget);
    delete d->_tableWidgetStyle;
}

void ElaTableWidget::setItemHeight(int itemHeight)
{
    Q_D(ElaTableWidget);
    if (itemHeight > 0)
    {
        d->_tableWidgetStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int ElaTableWidget::getItemHeight() const
{
    Q_D(const ElaTableWidget);
    return d->_tableWidgetStyle->getItemHeight();
}

void ElaTableWidget::setHeaderMargin(int headerMargin)
{
    Q_D(ElaTableWidget);
    if (headerMargin >= 0)
    {
        d->_tableWidgetStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int ElaTableWidget::getHeaderMargin() const
{
    Q_D(const ElaTableWidget);
    return d->_tableWidgetStyle->getHeaderMargin();
}

void ElaTableWidget::setIsTransparent(bool isTransparent)
{
    Q_D(ElaTableWidget);
    d->_tableWidgetStyle->setIsTransparent(isTransparent);
    update();
}

bool ElaTableWidget::getIsTransparent() const
{
    Q_D(const ElaTableWidget);
    return d->_tableWidgetStyle->getIsTransparent();
}

void ElaTableWidget::showEvent(QShowEvent* event)
{
    Q_EMIT tableWidgetShow();
    QTableWidget::showEvent(event);
}

void ElaTableWidget::hideEvent(QHideEvent* event)
{
    Q_EMIT tableWidgetHide();
    QTableWidget::hideEvent(event);
}

void ElaTableWidget::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaTableWidget);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_tableWidgetStyle->setCurrentHoverRow(rowAt(event->pos().y()));
        update();
    }
    QTableWidget::mouseMoveEvent(event);
}

void ElaTableWidget::leaveEvent(QEvent* event)
{
    Q_D(ElaTableWidget);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_tableWidgetStyle->setCurrentHoverRow(-1);
        update();
    }
    QTableWidget::leaveEvent(event);
}
