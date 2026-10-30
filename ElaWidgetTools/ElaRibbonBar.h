#ifndef ELARIBBONBAR_H
#define ELARIBBONBAR_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class QStackedWidget;
class ElaRibbonGroup;
class ElaRibbonTabBar;
class ElaRibbonBarPrivate;

class ELA_EXPORT ElaRibbonBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaRibbonBar)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(bool, BackgroundTransparent)
public:
    explicit ElaRibbonBar(QWidget* parent = nullptr);
    ~ElaRibbonBar() override;

    void bindTabBar(ElaRibbonTabBar* tabBar);
    ElaRibbonTabBar* tabBar() const;

    QWidget* addTab(const QString& title);
    ElaRibbonGroup* addGroup(QWidget* page, const QString& title);

    int tabCount() const;
    QString tabText(int index) const;

    void setCollapsed(bool collapsed);
    bool isCollapsed() const;

    void setPinned(bool pinned);
    bool isPinned() const;

    void setAnimationDuration(int durationMs);
    int getAnimationDuration() const;

    void showPinContextMenu(const QPoint& globalPos);

Q_SIGNALS:
    Q_SIGNAL void tabClicked(int index);
    Q_SIGNAL void collapsedChanged(bool collapsed);
    Q_SIGNAL void pinnedChanged(bool pinned);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELARIBBONBAR_H
