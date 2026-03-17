#ifndef ELAVIRTUALLIST_H
#define ELAVIRTUALLIST_H

#include <QListView>

#include "ElaProperty.h"

class ElaVirtualListPrivate;
class ELA_EXPORT ElaVirtualList : public QListView
{
    Q_OBJECT
    Q_Q_CREATE(ElaVirtualList)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsAlternatingRowColors)
public:
    explicit ElaVirtualList(QWidget* parent = nullptr);
    ~ElaVirtualList() override;

    void setItemCount(int count);
    int getItemCount() const;

Q_SIGNALS:
    Q_SIGNAL void itemRequestData(int startIndex, int endIndex);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAVIRTUALLIST_H
