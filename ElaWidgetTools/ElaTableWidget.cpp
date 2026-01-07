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

void ElaTableWidget::insertRows(int row, int count)
{
    if (row < 0 || count <= 0 || row > rowCount())
    {
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        insertRow(row);
    }
}

void ElaTableWidget::removeRows(int row, int count)
{
    if (row < 0 || count <= 0 || row >= rowCount())
    {
        return;
    }

    int actualCount = qMin(count, rowCount() - row);

    for (int i = row + actualCount - 1; i >= row; --i)
    {
        removeRow(i);
    }
}

void ElaTableWidget::insertColumns(int column, int count)
{
    if (column < 0 || count <= 0 || column > columnCount())
    {
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        insertColumn(column);
    }
}

void ElaTableWidget::removeColumns(int column, int count)
{
    if (column < 0 || count <= 0 || column >= columnCount())
    {
        return;
    }

    int actualCount = qMin(count, columnCount() - column);

    for (int i = column + actualCount - 1; i >= column; --i)
    {
        removeColumn(i);
    }
}

void ElaTableWidget::setItemText(int row, int column, const QString& text)
{
    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
    {
        return;
    }

    QTableWidgetItem* currentItem = item(row, column);
    if (currentItem)
    {
        currentItem->setText(text);
    }
    else
    {
        QTableWidgetItem* newItem = new QTableWidgetItem(text);
        setItem(row, column, newItem);
    }
}

QString ElaTableWidget::getItemText(int row, int column) const
{
    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
    {
        return QString();
    }

    QTableWidgetItem* currentItem = item(row, column);
    return currentItem ? currentItem->text() : QString();
}

void ElaTableWidget::setRowData(int row, const QStringList& data)
{
    if (row < 0 || row >= rowCount())
    {
        return;
    }

    int maxCol = qMin(data.size(), columnCount());
    for (int col = 0; col < maxCol; ++col)
    {
        setItemText(row, col, data[col]);
    }
}

QStringList ElaTableWidget::getRowData(int row) const
{
    QStringList data;
    if (row < 0 || row >= rowCount())
    {
        return data;
    }

    for (int col = 0; col < columnCount(); ++col)
    {
        data.append(getItemText(row, col));
    }
    return data;
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
        int newHoverRow = rowAt(event->pos().y());
        int oldHoverRow = d->_tableWidgetStyle->getCurrentHoverRow();
        if (newHoverRow != oldHoverRow)
        {
            d->_tableWidgetStyle->setCurrentHoverRow(newHoverRow);
            if (oldHoverRow >= 0)
            {
                for (int col = 0; col < columnCount(); ++col)
                {
                    QModelIndex index = model()->index(oldHoverRow, col);
                    update(visualRect(index));
                }
            }
            if (newHoverRow >= 0)
            {
                for (int col = 0; col < columnCount(); ++col)
                {
                    QModelIndex index = model()->index(newHoverRow, col);
                    update(visualRect(index));
                }
            }
        }
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
