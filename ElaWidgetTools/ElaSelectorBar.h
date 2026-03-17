#ifndef ELASELECTORBAR_H
#define ELASELECTORBAR_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaSelectorBarPrivate;
class ELA_EXPORT ElaSelectorBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSelectorBar)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaSelectorBar(QWidget* parent = nullptr);
    ~ElaSelectorBar() override;

    void addItem(const QString& text);
    void addItem(ElaIconType::IconName icon, const QString& text);
    void clearItems();
    int getItemCount() const;

    QSize sizeHint() const override;

Q_SIGNALS:
    Q_SIGNAL void currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

#endif // ELASELECTORBAR_H
