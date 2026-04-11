#ifndef T_NEWCOMPONENTS2_H
#define T_NEWCOMPONENTS2_H

#include "T_BasePage.h"

class ElaChatBubble;
class ElaDashboardGauge;
class ElaTerminalWidget;
class T_NewComponents2 : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_NewComponents2(QWidget* parent = nullptr);
    ~T_NewComponents2() override;

private:
    // ChatBubble
    ElaChatBubble* _chatBubbleLeft{nullptr};
    ElaChatBubble* _chatBubbleRight{nullptr};
    ElaChatBubble* _chatBubbleSystem{nullptr};
    // DashboardGauge
    ElaDashboardGauge* _gauge{nullptr};
    ElaDashboardGauge* _gaugeSmall{nullptr};
    // TerminalWidget
    ElaTerminalWidget* _terminal{nullptr};
};

#endif // T_NEWCOMPONENTS2_H
