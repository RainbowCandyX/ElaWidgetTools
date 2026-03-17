#ifndef ELAEXPANDERPRIVATE_H
#define ELAEXPANDERPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaExpander.h"
class QVariantAnimation;
class QVBoxLayout;
class ElaExpanderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaExpander)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(ElaIconType::IconName, HeaderIcon)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
public:
    explicit ElaExpanderPrivate(QObject* parent = nullptr);
    ~ElaExpanderPrivate() override;

private:
    bool _isExpanded{false};
    bool _isPressed{false};
    ElaExpander::ExpandDirection _expandDirection{ElaExpander::Down};
    QWidget* _contentWidget{nullptr};
    QWidget* _headerCustomWidget{nullptr};
    QWidget* _contentContainer{nullptr};
    QVBoxLayout* _contentLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QWidget* _headerWidget{nullptr};
    QVariantAnimation* _expandAnimation{nullptr};
    int _headerHeight{48};
    int _contentHeight{0};
    ElaThemeType::ThemeMode _themeMode;
    void _toggleExpand();
    void _startExpandAnimation(bool expand);
};

#endif // ELAEXPANDERPRIVATE_H
