#ifndef ELATRANSFER_H
#define ELATRANSFER_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaTransferPrivate;
class ELA_EXPORT ElaTransfer : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTransfer)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(QString, SourceTitle)
    Q_PROPERTY_CREATE_Q_H(QString, TargetTitle)
    Q_PROPERTY_CREATE_Q_H(bool, IsSearchVisible)
public:
    explicit ElaTransfer(QWidget* parent = nullptr);
    ~ElaTransfer() override;

    void setSourceItems(const QStringList& items);
    void addSourceItem(const QString& text);
    void addSourceItems(const QStringList& items);

    QStringList getSourceItems() const;
    QStringList getTargetItems() const;

    void moveToTarget();
    void moveToSource();
    void moveAllToTarget();
    void moveAllToSource();

Q_SIGNALS:
    Q_SIGNAL void transferChanged(const QStringList& sourceItems, const QStringList& targetItems);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELATRANSFER_H
