#ifndef T_NEWCOMPONENTS_H
#define T_NEWCOMPONENTS_H

#include "T_BasePage.h"

class ElaInfoBadge;
class ElaExpander;
class ElaTeachingTip;
class ElaPushButton;
class ElaPasswordBox;
class ElaTag;
class ElaSplitButton;
class ElaPersonPicture;
class ElaPagination;
class ElaFlyout;
class ElaDivider;
class ElaSkeleton;
class ElaSteps;
class ElaTimeline;
class ElaCommandBar;
class ElaRatingControl;
class ElaNumberBox;
class ElaWizard;
class ElaNotificationCenter;
class ElaMarkdownViewer;
class ElaCaptcha;
class ElaStatCard;
class ElaDropDownButton;
class ElaSelectorBar;
class ElaVirtualList;
class ElaSplashScreen;
class ElaSheetPanel;
class ElaTransfer;
class ElaSpotlight;
class ElaCountdown;
class ElaPopconfirm;
class ElaWatermark;
class ElaSplitter;
class ElaQRCode;
class ElaFloatButton;
class ElaEmojiPicker;
class ElaInfoBar;
class T_NewComponents : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_NewComponents(QWidget* parent = nullptr);
    ~T_NewComponents() override;

private:
    // InfoBadge
    ElaInfoBadge* _dotBadge{nullptr};
    ElaInfoBadge* _valueBadge{nullptr};
    ElaInfoBadge* _iconBadge{nullptr};
    // Expander
    ElaExpander* _expander{nullptr};
    ElaExpander* _expanderUp{nullptr};
    // TeachingTip
    ElaTeachingTip* _teachingTip{nullptr};
    ElaPushButton* _teachingTipTarget{nullptr};
    // PasswordBox
    ElaPasswordBox* _passwordBox{nullptr};
    // Tag
    ElaTag* _closableTag{nullptr};
    ElaTag* _checkableTag{nullptr};
    // SplitButton
    ElaSplitButton* _splitButton{nullptr};
    // PersonPicture
    ElaPersonPicture* _personPicture{nullptr};
    ElaPersonPicture* _personPictureInitials{nullptr};
    ElaPersonPicture* _personPictureDefault{nullptr};
    // Pagination
    ElaPagination* _pagination{nullptr};
    // Flyout
    ElaFlyout* _flyout{nullptr};
    // Steps
    ElaSteps* _steps{nullptr};
    // Timeline
    ElaTimeline* _timeline{nullptr};
    // CommandBar
    ElaCommandBar* _commandBar{nullptr};
    // RatingControl
    ElaRatingControl* _ratingControl{nullptr};
    // NumberBox
    ElaNumberBox* _numberBox{nullptr};
    // Wizard
    ElaWizard* _wizard{nullptr};
    // NotificationCenter
    ElaNotificationCenter* _notificationCenter{nullptr};
    // MarkdownViewer
    ElaMarkdownViewer* _markdownViewer{nullptr};
    // Captcha
    ElaCaptcha* _captcha{nullptr};
    // DropDownButton
    ElaDropDownButton* _dropDownButton{nullptr};
    // SelectorBar
    ElaSelectorBar* _selectorBar{nullptr};
    // VirtualList
    ElaVirtualList* _virtualList{nullptr};
    // SplashScreen
    ElaSplashScreen* _splashScreen{nullptr};
    // SheetPanel
    ElaSheetPanel* _sheetPanel{nullptr};
    // Transfer
    ElaTransfer* _transfer{nullptr};
    // Spotlight
    ElaSpotlight* _spotlight{nullptr};
    ElaCountdown* _countdown{nullptr};
    ElaPopconfirm* _popconfirm{nullptr};
    // Watermark
    ElaWatermark* _watermark{nullptr};
    // Splitter
    ElaSplitter* _splitter{nullptr};
    // QRCode
    ElaQRCode* _qrCode{nullptr};
    // FloatButton
    ElaFloatButton* _floatButton{nullptr};
    ElaEmojiPicker* _emojiPicker{nullptr};
    // InfoBar
    ElaInfoBar* _infoBarInfo{nullptr};
    ElaInfoBar* _infoBarSuccess{nullptr};
    ElaInfoBar* _infoBarWarning{nullptr};
    ElaInfoBar* _infoBarError{nullptr};
};

#endif // T_NEWCOMPONENTS_H
