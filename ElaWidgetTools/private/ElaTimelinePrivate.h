#ifndef ELATIMELINEPRIVATE_H
#define ELATIMELINEPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaTimeline.h"

class ElaTimelinePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTimeline)
public:
    explicit ElaTimelinePrivate(QObject* parent = nullptr);
    ~ElaTimelinePrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QList<ElaTimeline::TimelineItem> _items;
};

#endif // ELATIMELINEPRIVATE_H
