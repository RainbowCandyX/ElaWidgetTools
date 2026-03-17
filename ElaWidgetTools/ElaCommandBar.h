#ifndef ELACOMMANDBAR_H
#define ELACOMMANDBAR_H

#include <QWidget>

#include "ElaDef.h"

class ElaMenu;
class ElaCommandBarPrivate;
class ELA_EXPORT ElaCommandBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaCommandBar)
    Q_PROPERTY_CREATE_Q_H(int, ButtonSize)
public:
    struct CommandItem
    {
        ElaIconType::IconName icon = ElaIconType::None;
        QString text;
        bool isSeparator = false;
    };

    explicit ElaCommandBar(QWidget* parent = nullptr);
    ~ElaCommandBar();

    void addItem(const CommandItem& item);
    void addSeparator();
    void clearItems();

Q_SIGNALS:
    Q_SIGNAL void itemClicked(int index);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELACOMMANDBAR_H
