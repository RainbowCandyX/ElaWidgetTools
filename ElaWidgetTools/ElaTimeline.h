#ifndef ELATIMELINE_H
#define ELATIMELINE_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaTimelinePrivate;
class ELA_EXPORT ElaTimeline : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTimeline)
public:
    struct TimelineItem
    {
        QString title;
        QString content;
        QString timestamp;
        ElaIconType::IconName icon = ElaIconType::None;
    };

    explicit ElaTimeline(QWidget* parent = nullptr);
    ~ElaTimeline() override;

    void addItem(const TimelineItem& item);
    void clearItems();
    int getItemCount() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
};

#endif // ELATIMELINE_H
