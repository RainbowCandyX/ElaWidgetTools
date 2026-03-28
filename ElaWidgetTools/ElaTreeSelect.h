#ifndef ELATREESELECT_H
#define ELATREESELECT_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class QStandardItemModel;
class QStandardItem;
class QModelIndex;
class ElaTreeSelectPrivate;
class ELA_EXPORT ElaTreeSelect : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTreeSelect)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, MaxVisibleItems)
    Q_PROPERTY_REF_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(bool, IsSearchVisible)
    Q_PROPERTY_CREATE_Q_H(bool, IsEditable)

public:
    explicit ElaTreeSelect(QWidget* parent = nullptr);
    ~ElaTreeSelect() override;

    void setModel(QStandardItemModel* model);
    QStandardItemModel* model() const;

    void setCurrentIndex(const QModelIndex& index);
    QModelIndex currentIndex() const;

    QString currentText() const;

    void expandAll();
    void collapseAll();

Q_SIGNALS:
    Q_SIGNAL void currentIndexChanged(const QModelIndex& index);
    Q_SIGNAL void currentTextChanged(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
};

#endif // ELATREESELECT_H
