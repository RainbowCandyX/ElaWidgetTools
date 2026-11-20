#ifndef ELADATETIMEEDIT_H
#define ELADATETIMEEDIT_H

#include <QDateTimeEdit>

#include "ElaDef.h"

class ElaDateTimeEditPrivate;
class ELA_EXPORT ElaDateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
    Q_Q_CREATE(ElaDateTimeEdit)
public:
    explicit ElaDateTimeEdit(QWidget* parent = nullptr);
    explicit ElaDateTimeEdit(const QDateTime& dateTime, QWidget* parent = nullptr);
    ~ElaDateTimeEdit() override;

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // ELADATETIMEEDIT_H
