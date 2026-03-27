#include "T_NewComponents.h"

#include "ElaCaptcha.h"
#include "ElaCheckBox.h"
#include "ElaComboBox.h"
#include "ElaCommandBar.h"
#include "ElaDivider.h"
#include "ElaDropDownButton.h"
#include "ElaExpander.h"
#include "ElaFlyout.h"
#include "ElaInfoBadge.h"
#include "ElaLineEdit.h"
#include "ElaMarkdownViewer.h"
#include "ElaMenu.h"
#include "ElaNumberBox.h"
#include "ElaPagination.h"
#include "ElaPasswordBox.h"
#include "ElaPersonPicture.h"
#include "ElaPushButton.h"
#include "ElaRadioButton.h"
#include "ElaRatingControl.h"
#include "ElaScrollPageArea.h"
#include "ElaSelectorBar.h"
#include "ElaSkeleton.h"
#include "ElaSlider.h"
#include "ElaSplitButton.h"
#include "ElaStatCard.h"
#include "ElaSteps.h"
#include "ElaTag.h"
#include "ElaTeachingTip.h"
#include "ElaText.h"
#include "ElaTimeline.h"
#include "ElaToast.h"
#include "ElaToggleSwitch.h"
#include "ElaNotificationCenter.h"
#include "ElaWizard.h"
#include "ElaVirtualList.h"
#include "ElaSplashScreen.h"
#include "ElaSheetPanel.h"
#include "ElaTransfer.h"
#include "ElaSpotlight.h"
#include "ElaCountdown.h"
#include "ElaPopconfirm.h"
#include "ElaWatermark.h"
#include "ElaSplitter.h"
#include "ElaSnackbar.h"
#include "ElaQRCode.h"
#include "ElaFloatButton.h"
#include "ElaEmojiPicker.h"
#include "ElaInfoBar.h"
#include "ElaAutoComplete.h"
#include "ElaTreeSelect.h"
#include "ElaUploadArea.h"
#include <QButtonGroup>
#include <QDateTime>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QTimer>
#include <QVBoxLayout>

T_NewComponents::T_NewComponents(QWidget *parent)
	: T_BasePage(parent)
{
	setWindowTitle("ElaNewComponents");
	createCustomWidget("新增组件的演示页面");

	QWidget *centralWidget = new QWidget(this);
	centralWidget->setWindowTitle("ElaNewComponents");

	// ========== ElaInfoBadge 示例 ==========
	ElaPushButton *dotTargetButton = new ElaPushButton("邮件", this);
	dotTargetButton->setFixedSize(80, 36);
	_dotBadge = new ElaInfoBadge(this);
	_dotBadge->setBadgeMode(ElaInfoBadge::Dot);
	_dotBadge->setSeverity(ElaInfoBadge::Attention);
	_dotBadge->attachTo(dotTargetButton);

	ElaPushButton *valueTargetButton = new ElaPushButton("通知", this);
	valueTargetButton->setFixedSize(80, 36);
	_valueBadge = new ElaInfoBadge(5, this);
	_valueBadge->setSeverity(ElaInfoBadge::Informational);
	_valueBadge->attachTo(valueTargetButton);

	ElaPushButton *iconTargetButton = new ElaPushButton("设置", this);
	iconTargetButton->setFixedSize(80, 36);
	_iconBadge = new ElaInfoBadge(ElaIconType::CircleExclamation, this);
	_iconBadge->setSeverity(ElaInfoBadge::Caution);
	_iconBadge->attachTo(iconTargetButton);

	ElaScrollPageArea *badgeArea = new ElaScrollPageArea(this);
	QHBoxLayout *badgeLayout = new QHBoxLayout(badgeArea);
	ElaText *badgeText = new ElaText("ElaInfoBadge", this);
	badgeText->setTextPixelSize(15);
	badgeLayout->addWidget(badgeText);
	badgeLayout->addSpacing(20);
	badgeLayout->addWidget(dotTargetButton);
	badgeLayout->addSpacing(20);
	badgeLayout->addWidget(valueTargetButton);
	badgeLayout->addSpacing(20);
	badgeLayout->addWidget(iconTargetButton);
	badgeLayout->addStretch();

	// InfoBadge 值控制
	ElaScrollPageArea *badgeControlArea = new ElaScrollPageArea(this);
	QHBoxLayout *badgeControlLayout = new QHBoxLayout(badgeControlArea);
	badgeControlLayout->addWidget(new ElaText("InfoBadge 值控制", 15, this));
	ElaSlider *badgeSlider = new ElaSlider(this);
	badgeSlider->setRange(0, 150);
	badgeSlider->setValue(5);
	badgeSlider->setFixedWidth(200);
	ElaText *badgeValueText = new ElaText("5", this);
	badgeValueText->setTextPixelSize(13);
	connect(badgeSlider, &ElaSlider::valueChanged, this, [=](int v)
	{
		_valueBadge->setValue(v);
		badgeValueText->setText(QString::number(v));
	});
	badgeControlLayout->addWidget(badgeSlider);
	badgeControlLayout->addWidget(badgeValueText);
	badgeControlLayout->addStretch();

	// InfoBadge 严重等级
	ElaScrollPageArea *severityArea = new ElaScrollPageArea(this);
	QHBoxLayout *severityLayout = new QHBoxLayout(severityArea);
	severityLayout->addWidget(new ElaText("InfoBadge 严重等级", 15, this));
	QButtonGroup *severityGroup = new QButtonGroup(this);
	ElaRadioButton *attentionRadio = new ElaRadioButton("Attention", this);
	ElaRadioButton *infoRadio = new ElaRadioButton("Informational", this);
	ElaRadioButton *successRadio = new ElaRadioButton("Success", this);
	ElaRadioButton *cautionRadio = new ElaRadioButton("Caution", this);
	ElaRadioButton *criticalRadio = new ElaRadioButton("Critical", this);
	severityGroup->addButton(attentionRadio, 0);
	severityGroup->addButton(infoRadio, 1);
	severityGroup->addButton(successRadio, 2);
	severityGroup->addButton(cautionRadio, 3);
	severityGroup->addButton(criticalRadio, 4);
	infoRadio->setChecked(true);
	connect(severityGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, [=](int id)
	{
		_valueBadge->setSeverity(static_cast<ElaInfoBadge::Severity>(id));
	});
	severityLayout->addWidget(attentionRadio);
	severityLayout->addWidget(infoRadio);
	severityLayout->addWidget(successRadio);
	severityLayout->addWidget(cautionRadio);
	severityLayout->addWidget(criticalRadio);
	severityLayout->addStretch();

	// ========== ElaExpander 示例 ==========
	ElaText *expanderText = new ElaText("ElaExpander (向下展开)", this);
	expanderText->setTextPixelSize(15);
	_expander = new ElaExpander("网络设置", this);
	_expander->setSubTitle("配置代理和网络参数");
	_expander->setHeaderIcon(ElaIconType::WifiExclamation);
	QWidget *expanderContent = new QWidget(this);
	QVBoxLayout *expanderContentLayout = new QVBoxLayout(expanderContent);
	expanderContentLayout->setContentsMargins(0, 0, 0, 0);
	expanderContentLayout->addWidget(new ElaCheckBox("启用代理", this));
	ElaLineEdit *proxyHost = new ElaLineEdit(this);
	proxyHost->setPlaceholderText("代理地址 (例如: 127.0.0.1)");
	proxyHost->setFixedHeight(35);
	ElaLineEdit *proxyPort = new ElaLineEdit(this);
	proxyPort->setPlaceholderText("端口 (例如: 7890)");
	proxyPort->setFixedHeight(35);
	expanderContentLayout->addWidget(proxyHost);
	expanderContentLayout->addWidget(proxyPort);
	_expander->setContentWidget(expanderContent);

	ElaText *expanderUpText = new ElaText("ElaExpander (向上展开)", this);
	expanderUpText->setTextPixelSize(15);
	_expanderUp = new ElaExpander("高级设置", this);
	_expanderUp->setExpandDirection(ElaExpander::Up);
	_expanderUp->setHeaderIcon(ElaIconType::GearComplex);
	QWidget *expanderUpContent = new QWidget(this);
	QVBoxLayout *expanderUpContentLayout = new QVBoxLayout(expanderUpContent);
	expanderUpContentLayout->setContentsMargins(0, 0, 0, 0);
	QHBoxLayout *dbgLay = new QHBoxLayout();
	dbgLay->addWidget(new ElaText("调试模式", 13, this));
	dbgLay->addWidget(new ElaToggleSwitch(this));
	dbgLay->addStretch();
	QHBoxLayout *logLay = new QHBoxLayout();
	logLay->addWidget(new ElaText("日志输出", 13, this));
	logLay->addWidget(new ElaToggleSwitch(this));
	logLay->addStretch();
	expanderUpContentLayout->addLayout(dbgLay);
	expanderUpContentLayout->addLayout(logLay);
	_expanderUp->setContentWidget(expanderUpContent);

	// ========== ElaTeachingTip 示例 ==========
	_teachingTip = new ElaTeachingTip(this);
	_teachingTip->setTitle("截图功能升级");
	_teachingTip->setSubTitle("版本 2.0");
	_teachingTip->setContent("现在支持区域截图、延时截图和 OCR 文字识别。");
	_teachingTip->setTipIcon(ElaIconType::CameraRetro);
	_teachingTip->setTailPosition(ElaTeachingTip::Bottom);
	_teachingTip->addAction("跳过", [=]() { _teachingTip->closeTip(); });
	_teachingTip->addAction("了解更多", [=]() { _teachingTip->closeTip(); });

	_teachingTipTarget = new ElaPushButton("显示 TeachingTip", this);
	_teachingTipTarget->setFixedSize(160, 38);
	_teachingTip->setTarget(_teachingTipTarget);
	connect(_teachingTipTarget, &ElaPushButton::clicked, this, [=]() { _teachingTip->showTip(); });

	ElaScrollPageArea *teachingTipArea = new ElaScrollPageArea(this);
	QHBoxLayout *tipLayout = new QHBoxLayout(teachingTipArea);
	tipLayout->addWidget(new ElaText("ElaTeachingTip", 15, this));
	tipLayout->addWidget(_teachingTipTarget);
	tipLayout->addStretch();

	// TeachingTip 方向切换
	ElaScrollPageArea *tipPosArea = new ElaScrollPageArea(this);
	QHBoxLayout *tipPosLayout = new QHBoxLayout(tipPosArea);
	tipPosLayout->addWidget(new ElaText("TeachingTip 箭头方向", 15, this));
	QButtonGroup *tipPosGroup = new QButtonGroup(this);
	ElaRadioButton *autoRadio = new ElaRadioButton("Auto", this);
	ElaRadioButton *topRadio = new ElaRadioButton("Top", this);
	ElaRadioButton *bottomRadio = new ElaRadioButton("Bottom", this);
	ElaRadioButton *leftRadio = new ElaRadioButton("Left", this);
	ElaRadioButton *rightRadio = new ElaRadioButton("Right", this);
	tipPosGroup->addButton(autoRadio, 0);
	tipPosGroup->addButton(topRadio, 1);
	tipPosGroup->addButton(bottomRadio, 2);
	tipPosGroup->addButton(leftRadio, 3);
	tipPosGroup->addButton(rightRadio, 4);
	bottomRadio->setChecked(true);
	connect(tipPosGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, [=](int id)
	{
		_teachingTip->setTailPosition(static_cast<ElaTeachingTip::TailPosition>(id));
	});
	tipPosLayout->addWidget(autoRadio);
	tipPosLayout->addWidget(topRadio);
	tipPosLayout->addWidget(bottomRadio);
	tipPosLayout->addWidget(leftRadio);
	tipPosLayout->addWidget(rightRadio);
	tipPosLayout->addStretch();

	// ========== ElaPasswordBox 示例 ==========
	_passwordBox = new ElaPasswordBox(this);
	_passwordBox->setFixedSize(280, 38);
	_passwordBox->setPlaceholderText("请输入密码...");

	ElaScrollPageArea *passwordArea = new ElaScrollPageArea(this);
	QHBoxLayout *pwdLayout = new QHBoxLayout(passwordArea);
	pwdLayout->addWidget(new ElaText("ElaPasswordBox", 15, this));
	pwdLayout->addWidget(_passwordBox);
	pwdLayout->addStretch();

	// ========== ElaNumberBox 示例 ==========
	_numberBox = new ElaNumberBox(this);
	_numberBox->setMinimum(-50);
	_numberBox->setMaximum(200);
	_numberBox->setStep(5);
	_numberBox->setValue(25);

	ElaNumberBox *decimalBox = new ElaNumberBox(this);
	decimalBox->setMinimum(0);
	decimalBox->setMaximum(10);
	decimalBox->setStep(0.1);
	decimalBox->setDecimals(2);
	decimalBox->setValue(3.14);

	ElaText *numberValue = new ElaText("25", this);
	numberValue->setTextPixelSize(13);
	connect(_numberBox, &ElaNumberBox::valueChanged, this, [=](double v)
	{
		numberValue->setText(QString::number(v, 'f', 0));
	});

	ElaScrollPageArea *numberArea = new ElaScrollPageArea(this);
	QHBoxLayout *numberLayout = new QHBoxLayout(numberArea);
	numberLayout->addWidget(new ElaText("ElaNumberBox", 15, this));
	numberLayout->addWidget(_numberBox);
	numberLayout->addWidget(numberValue);
	numberLayout->addSpacing(15);
	numberLayout->addWidget(new ElaText("小数:", 13, this));
	numberLayout->addWidget(decimalBox);
	numberLayout->addStretch();

	// ========== ElaTag 示例 ==========
	ElaTag *defaultTag = new ElaTag(this);
	defaultTag->setTagText("Default");
	ElaTag *primaryTag = new ElaTag(this);
	primaryTag->setTagText("Primary");
	primaryTag->setTagColor(ElaTag::Primary);
	ElaTag *successTag = new ElaTag(this);
	successTag->setTagText("Success");
	successTag->setTagColor(ElaTag::Success);
	ElaTag *warningTag = new ElaTag(this);
	warningTag->setTagText("Warning");
	warningTag->setTagColor(ElaTag::Warning);
	ElaTag *dangerTag = new ElaTag(this);
	dangerTag->setTagText("Danger");
	dangerTag->setTagColor(ElaTag::Danger);

	ElaScrollPageArea *tagArea = new ElaScrollPageArea(this);
	QHBoxLayout *tagLayout = new QHBoxLayout(tagArea);
	tagLayout->addWidget(new ElaText("ElaTag", 15, this));
	tagLayout->addSpacing(10);
	tagLayout->addWidget(defaultTag);
	tagLayout->addWidget(primaryTag);
	tagLayout->addWidget(successTag);
	tagLayout->addWidget(warningTag);
	tagLayout->addWidget(dangerTag);
	tagLayout->addStretch();

	// Tag 可关闭
	_closableTag = new ElaTag(this);
	_closableTag->setTagText("可关闭");
	_closableTag->setIsClosable(true);
	_closableTag->setTagColor(ElaTag::Primary);
	ElaTag *closableTag2 = new ElaTag(this);
	closableTag2->setTagText("北京");
	closableTag2->setIsClosable(true);
	ElaTag *closableTag3 = new ElaTag(this);
	closableTag3->setTagText("上海");
	closableTag3->setIsClosable(true);
	connect(_closableTag, &ElaTag::closed, this, [=]() { _closableTag->hide(); });
	connect(closableTag2, &ElaTag::closed, this, [=]() { closableTag2->hide(); });
	connect(closableTag3, &ElaTag::closed, this, [=]() { closableTag3->hide(); });

	ElaScrollPageArea *tagCloseArea = new ElaScrollPageArea(this);
	QHBoxLayout *tagCloseLayout = new QHBoxLayout(tagCloseArea);
	tagCloseLayout->addWidget(new ElaText("ElaTag 可关闭", 15, this));
	tagCloseLayout->addSpacing(10);
	tagCloseLayout->addWidget(_closableTag);
	tagCloseLayout->addWidget(closableTag2);
	tagCloseLayout->addWidget(closableTag3);
	tagCloseLayout->addStretch();

	// Tag 可选中
	_checkableTag = new ElaTag(this);
	_checkableTag->setTagText("周一");
	_checkableTag->setIsCheckable(true);
	ElaTag *chkTag2 = new ElaTag(this);
	chkTag2->setTagText("周二");
	chkTag2->setIsCheckable(true);
	chkTag2->setIsChecked(true);
	ElaTag *chkTag3 = new ElaTag(this);
	chkTag3->setTagText("周三");
	chkTag3->setIsCheckable(true);

	ElaScrollPageArea *tagCheckArea = new ElaScrollPageArea(this);
	QHBoxLayout *tagCheckLayout = new QHBoxLayout(tagCheckArea);
	tagCheckLayout->addWidget(new ElaText("ElaTag 可选中", 15, this));
	tagCheckLayout->addSpacing(10);
	tagCheckLayout->addWidget(_checkableTag);
	tagCheckLayout->addWidget(chkTag2);
	tagCheckLayout->addWidget(chkTag3);
	tagCheckLayout->addStretch();

	// ========== ElaSplitButton 示例 ==========
	_splitButton = new ElaSplitButton(this);
	_splitButton->setText("发送邮件");
	_splitButton->setElaIcon(ElaIconType::Envelope);
	ElaMenu *splitMenu = new ElaMenu(this);
	splitMenu->addElaIconAction(ElaIconType::Reply, "回复");
	splitMenu->addElaIconAction(ElaIconType::ReplyAll, "全部回复");
	splitMenu->addElaIconAction(ElaIconType::Share, "转发");
	_splitButton->setMenu(splitMenu);

	ElaScrollPageArea *splitArea = new ElaScrollPageArea(this);
	QHBoxLayout *splitLayout = new QHBoxLayout(splitArea);
	splitLayout->addWidget(new ElaText("ElaSplitButton", 15, this));
	splitLayout->addWidget(_splitButton);
	splitLayout->addStretch();

	// ========== ElaPersonPicture 示例 ==========
	_personPicture = new ElaPersonPicture(this);
	_personPicture->setPicture(QPixmap(":/Resource/Image/Cirno.jpg"));
	_personPictureInitials = new ElaPersonPicture(this);
	_personPictureInitials->setDisplayName("Ela");
	_personPictureDefault = new ElaPersonPicture(this);

	ElaScrollPageArea *personArea = new ElaScrollPageArea(this);
	QHBoxLayout *personLayout = new QHBoxLayout(personArea);
	personLayout->addWidget(new ElaText("ElaPersonPicture", 15, this));
	personLayout->addSpacing(10);
	personLayout->addWidget(_personPicture);
	personLayout->addSpacing(10);
	personLayout->addWidget(_personPictureInitials);
	personLayout->addSpacing(10);
	personLayout->addWidget(_personPictureDefault);
	personLayout->addStretch();

	// ========== ElaRatingControl 示例 ==========
	_ratingControl = new ElaRatingControl(this);
	_ratingControl->setRating(5);
	ElaText *ratingValue = new ElaText("5", this);
	ratingValue->setTextPixelSize(15);
	connect(_ratingControl, &ElaRatingControl::ratingChanged, this, [=](int r)
	{
		ratingValue->setText(QString::number(r));
	});

	ElaScrollPageArea *ratingArea = new ElaScrollPageArea(this);
	QHBoxLayout *ratingLayout = new QHBoxLayout(ratingArea);
	ratingLayout->addWidget(new ElaText("ElaRatingControl", 15, this));
	ratingLayout->addWidget(_ratingControl);
	ratingLayout->addWidget(ratingValue);
	ratingLayout->addStretch();

	// ========== ElaDivider 示例 ==========
	ElaDivider *divider1 = new ElaDivider(this);
	ElaDivider *divider2 = new ElaDivider("分隔文字", this);

	// ========== ElaFlyout 示例 ==========
	_flyout = new ElaFlyout(this);
	_flyout->setTitle("提示");
	_flyout->setContent("轻量级弹出面板，点击外部自动关闭。");
	ElaPushButton *flyoutBtn = new ElaPushButton("显示 Flyout", this);
	flyoutBtn->setFixedSize(120, 38);
	connect(flyoutBtn, &ElaPushButton::clicked, this, [=]() { _flyout->showFlyout(flyoutBtn); });

	// 自定义 Widget Flyout
	ElaFlyout *customFlyout = new ElaFlyout(this);
	QWidget *customContent = new QWidget();
	QVBoxLayout *customFlyoutLay = new QVBoxLayout(customContent);
	customFlyoutLay->setContentsMargins(0, 0, 0, 0);
	customFlyoutLay->addWidget(new ElaText("请为本次体验评分:", 13, customContent));
	customFlyoutLay->addWidget(new ElaRatingControl(customContent));
	customFlyout->setContentWidget(customContent);

	ElaPushButton *customFlyoutBtn = new ElaPushButton("自定义 Flyout", this);
	customFlyoutBtn->setFixedSize(130, 38);
	connect(customFlyoutBtn, &ElaPushButton::clicked, this, [=]() { customFlyout->showFlyout(customFlyoutBtn); });

	ElaScrollPageArea *flyoutArea = new ElaScrollPageArea(this);
	QHBoxLayout *flyoutLayout = new QHBoxLayout(flyoutArea);
	flyoutLayout->addWidget(new ElaText("ElaFlyout", 15, this));
	flyoutLayout->addWidget(flyoutBtn);
	flyoutLayout->addSpacing(10);
	flyoutLayout->addWidget(customFlyoutBtn);
	flyoutLayout->addStretch();

	// ========== ElaSkeleton 示例 ==========
	ElaSkeleton *skelCircle = new ElaSkeleton(this);
	skelCircle->setSkeletonType(ElaSkeleton::Circle);
	skelCircle->setFixedSize(40, 40);
	ElaSkeleton *skelText1 = new ElaSkeleton(this);
	skelText1->setSkeletonType(ElaSkeleton::Text);
	skelText1->setFixedSize(250, 16);
	ElaSkeleton *skelRect = new ElaSkeleton(this);
	skelRect->setSkeletonType(ElaSkeleton::Rectangle);
	skelRect->setFixedSize(200, 80);
	skelRect->setBorderRadius(8);

	ElaScrollPageArea *skelArea = new ElaScrollPageArea(this);
	QHBoxLayout *skelLayout = new QHBoxLayout(skelArea);
	skelLayout->addWidget(new ElaText("ElaSkeleton", 15, this));
	skelLayout->addSpacing(10);
	skelLayout->addWidget(skelCircle);
	skelLayout->addSpacing(10);
	skelLayout->addWidget(skelText1);
	skelLayout->addSpacing(10);
	skelLayout->addWidget(skelRect);
	skelLayout->addStretch();

	// ========== ElaSteps 示例 ==========
	_steps = new ElaSteps(this);
	_steps->setStepCount(4);
	_steps->setStepTitles({"选择商品", "确认订单", "支付", "完成"});
	_steps->setCurrentStep(1);
	ElaPushButton *prevBtn = new ElaPushButton("上一步", this);
	prevBtn->setFixedSize(80, 32);
	ElaPushButton *nextBtn = new ElaPushButton("下一步", this);
	nextBtn->setFixedSize(80, 32);
	connect(prevBtn, &ElaPushButton::clicked, _steps, &ElaSteps::previous);
	connect(nextBtn, &ElaPushButton::clicked, _steps, &ElaSteps::next);

	QHBoxLayout *stepsCtrl = new QHBoxLayout();
	stepsCtrl->addWidget(new ElaText("ElaSteps", 15, this));
	stepsCtrl->addStretch();
	stepsCtrl->addWidget(prevBtn);
	stepsCtrl->addWidget(nextBtn);

	// ========== ElaCommandBar 示例 ==========
	_commandBar = new ElaCommandBar(this);
	_commandBar->addItem({ElaIconType::Copy, "复制"});
	_commandBar->addItem({ElaIconType::Paste, "粘贴"});
	_commandBar->addItem({ElaIconType::Scissors, "剪切"});
	_commandBar->addSeparator();
	_commandBar->addItem({ElaIconType::ArrowRotateLeft, "撤销"});
	_commandBar->addItem({ElaIconType::ArrowRotateRight, "重做"});
	_commandBar->addSeparator();
	_commandBar->addItem({ElaIconType::FloppyDisk, "保存"});

	ElaScrollPageArea *cmdArea = new ElaScrollPageArea(this);
	QHBoxLayout *cmdLayout = new QHBoxLayout(cmdArea);
	cmdLayout->addWidget(new ElaText("ElaCommandBar", 15, this));
	cmdLayout->addWidget(_commandBar);
	cmdLayout->addStretch();

	// ========== ElaPagination 示例 ==========
	QStackedWidget *pageStack = new QStackedWidget(this);
	pageStack->setFixedHeight(160);
	int totalPages = 50;
	for (int p = 1; p <= totalPages; p++)
	{
		QWidget *page = new QWidget(this);
		QVBoxLayout *pLay = new QVBoxLayout(page);
		pLay->setContentsMargins(15, 10, 15, 10);
		pLay->addWidget(new ElaText(QString("第 %1 页").arg(p), 16, this));
		for (int i = 1; i <= 5; i++)
			pLay->addWidget(new ElaText(QString("  数据项 #%1").arg((p - 1) * 4 + i), 13, this));
		pLay->addStretch();
		pageStack->addWidget(page);
	}
	_pagination = new ElaPagination(this);
	_pagination->setTotalPages(totalPages);
	_pagination->setCurrentPage(1);
	_pagination->setJumperVisible(true);
	ElaText *pageInfo = new ElaText(QString("第 1 / %1 页").arg(totalPages), this);
	pageInfo->setTextPixelSize(13);
	connect(_pagination, &ElaPagination::currentPageChanged, this, [=](int pg)
	{
		pageStack->setCurrentIndex(pg - 1);
		pageInfo->setText(QString("第 %1 / %2 页").arg(pg).arg(totalPages));
	});

	ElaScrollPageArea *pgArea = new ElaScrollPageArea(this);
	QHBoxLayout *pgLayout = new QHBoxLayout(pgArea);
	pgLayout->addWidget(new ElaText("ElaPagination", 15, this));
	pgLayout->addWidget(_pagination);
	pgLayout->addWidget(pageInfo);
	pgLayout->addStretch();

	// ========== ElaTimeline 示例 ==========
	_timeline = new ElaTimeline(this);
	_timeline->addItem({"项目启动", "完成需求分析和技术选型", "2026-01-15", ElaIconType::RocketLaunch});
	_timeline->addItem({"开发阶段", "完成核心模块开发", "2026-02-20", ElaIconType::Code});
	_timeline->addItem({"测试阶段", "集成测试和验收测试", "2026-03-01"});
	_timeline->addItem({"发布上线", "部署到生产环境", "2026-03-15"});

	// ========== ElaToast 示例 ==========
	ElaPushButton *toastSuccessBtn = new ElaPushButton("成功", this);
	toastSuccessBtn->setFixedSize(70, 32);
	connect(toastSuccessBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaToast::success("操作成功完成!", 2000, window());
	});
	ElaPushButton *toastInfoBtn = new ElaPushButton("信息", this);
	toastInfoBtn->setFixedSize(70, 32);
	connect(toastInfoBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaToast::info("这是一条提示信息", 2000, window());
	});
	ElaPushButton *toastWarnBtn = new ElaPushButton("警告", this);
	toastWarnBtn->setFixedSize(70, 32);
	connect(toastWarnBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaToast::warning("请注意检查输入", 2000, window());
	});
	ElaPushButton *toastErrorBtn = new ElaPushButton("错误", this);
	toastErrorBtn->setFixedSize(70, 32);
	connect(toastErrorBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaToast::error("操作失败，请重试", 2000, window());
	});

	ElaScrollPageArea *toastArea = new ElaScrollPageArea(this);
	QHBoxLayout *toastLayout = new QHBoxLayout(toastArea);
	toastLayout->addWidget(new ElaText("ElaToast", 15, this));
	toastLayout->addWidget(toastSuccessBtn);
	toastLayout->addWidget(toastInfoBtn);
	toastLayout->addWidget(toastWarnBtn);
	toastLayout->addWidget(toastErrorBtn);
	toastLayout->addStretch();

	// ========== ElaWizard 示例 ==========
	_wizard = new ElaWizard(this);
	QWidget *wizPage1 = new QWidget();
	QVBoxLayout *wl1 = new QVBoxLayout(wizPage1);
	wl1->addWidget(new ElaText("欢迎使用安装向导", 16, wizPage1));
	wl1->addWidget(new ElaText("本向导将引导你完成软件安装过程。", 13, wizPage1));
	wl1->addStretch();
	_wizard->addStep("欢迎", wizPage1);

	QWidget *wizPage2 = new QWidget();
	QVBoxLayout *wl2 = new QVBoxLayout(wizPage2);
	wl2->addWidget(new ElaText("请选择安装路径", 16, wizPage2));
	ElaLineEdit *pathEdit = new ElaLineEdit(wizPage2);
	pathEdit->setText("/usr/local/ElaWidgetTools");
	pathEdit->setFixedHeight(35);
	wl2->addWidget(pathEdit);
	wl2->addStretch();
	_wizard->addStep("路径", wizPage2);

	QWidget *wizPage3 = new QWidget();
	QVBoxLayout *wl3 = new QVBoxLayout(wizPage3);
	wl3->addWidget(new ElaText("安装完成！", 16, wizPage3));
	wl3->addWidget(new ElaText("所有组件已成功安装。", 13, wizPage3));
	wl3->addStretch();
	_wizard->addStep("完成", wizPage3);

	ElaPushButton *wizardBtn = new ElaPushButton("打开向导", this);
	wizardBtn->setFixedSize(100, 38);
	connect(wizardBtn, &ElaPushButton::clicked, this, [=]() { _wizard->show(); });

	ElaScrollPageArea *wizardArea = new ElaScrollPageArea(this);
	QHBoxLayout *wizardLayout = new QHBoxLayout(wizardArea);
	wizardLayout->addWidget(new ElaText("ElaWizard", 15, this));
	wizardLayout->addWidget(wizardBtn);
	wizardLayout->addStretch();

	// ========== ElaNotificationCenter 示例 ==========
	_notificationCenter = new ElaNotificationCenter(this);
	_notificationCenter->addNotification({"系统更新", "ElaWidgetTools 已更新到 v2.1", "10:30"});
	_notificationCenter->addNotification({"构建完成", "项目编译成功，耗时 12 秒", "10:25", ElaIconType::Check});
	_notificationCenter->addNotification({"新消息", "收到来自 Qt 社区的新回复", "09:45", ElaIconType::Envelope});

	ElaPushButton *notifBtn = new ElaPushButton("通知中心", this);
	notifBtn->setFixedSize(100, 38);
	connect(notifBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (_notificationCenter->isPanelVisible())
			_notificationCenter->hidePanel();
		else
			_notificationCenter->showPanel(window());
	});

	ElaScrollPageArea *notifArea = new ElaScrollPageArea(this);
	QHBoxLayout *notifLayout = new QHBoxLayout(notifArea);
	notifLayout->addWidget(new ElaText("ElaNotificationCenter", 15, this));
	notifLayout->addWidget(notifBtn);
	notifLayout->addStretch();

	// ========== ElaMarkdownViewer 示例 ==========
	_markdownViewer = new ElaMarkdownViewer(this);
	_markdownViewer->setFixedHeight(200);
	_markdownViewer->setMarkdown(
		"# ElaWidgetTools\n\n"
		"一个 **FluentUI** 风格的 Qt 组件库。\n\n"
		"## 特性\n\n"
		"- 支持 `Light` / `Dark` 主题切换\n"
		"- **90+** 个组件\n"
		"- 跨平台: Windows / macOS / Linux\n\n"
		"## 代码示例\n\n"
		"```cpp\nElaToast::success(\"Hello World!\");\n```\n\n"
		"> 更多详情请访问 [GitHub](https://github.com/RainbowCandyX/ElaWidgetTools)\n"
	);

	// ========== ElaCaptcha 示例 ==========
	_captcha = new ElaCaptcha(this);
	_captcha->setInputMode(ElaCaptcha::DigitOnly);
	ElaText *captchaResult = new ElaText("", this);
	_captcha->setCodeLength(4);
	captchaResult->setTextPixelSize(13);
	connect(_captcha, &ElaCaptcha::codeCompleted, this, [=](const QString &code)
	{
		captchaResult->setText("输入完成: " + code);
	});
	connect(_captcha, &ElaCaptcha::codeChanged, this, [=](const QString &code)
	{
		if (code.length() < _captcha->getCodeLength())
			captchaResult->setText("");
	});

	ElaPushButton *captchaClearBtn = new ElaPushButton("清除", this);
	captchaClearBtn->setFixedSize(60, 32);
	connect(captchaClearBtn, &ElaPushButton::clicked, _captcha, &ElaCaptcha::clear);

	ElaScrollPageArea *captchaArea = new ElaScrollPageArea(this);
	QHBoxLayout *captchaLayout = new QHBoxLayout(captchaArea);
	captchaLayout->addWidget(new ElaText("ElaCaptcha", 15, this));
	captchaLayout->addWidget(_captcha);
	captchaLayout->addWidget(captchaClearBtn);
	captchaLayout->addWidget(captchaResult);
	captchaLayout->addStretch();

	// ========== ElaStatCard 示例 ==========
	ElaStatCard *statCard1 = new ElaStatCard(this);
	statCard1->setTitle("今日销售额");
	statCard1->setValue("¥86,400");
	statCard1->setCardIcon(ElaIconType::CartShopping);
	statCard1->setTrend(ElaStatCard::Up);
	statCard1->setTrendText("+15.3%");
	statCard1->setDescription("较昨日");

	ElaStatCard *statCard2 = new ElaStatCard(this);
	statCard2->setTitle("CPU 使用率");
	statCard2->setValue("62%");
	statCard2->setCardIcon(ElaIconType::Gauge);
	statCard2->setTrend(ElaStatCard::Down);
	statCard2->setTrendText("-8.1%");
	statCard2->setDescription("较上小时");

	ElaStatCard *statCard3 = new ElaStatCard(this);
	statCard3->setTitle("在线设备");
	statCard3->setValue("1,024");
	statCard3->setCardIcon(ElaIconType::WifiExclamation);
	statCard3->setTrend(ElaStatCard::Neutral);
	statCard3->setTrendText("稳定");
	statCard3->setDescription("当前在线");

	QHBoxLayout *statLayout = new QHBoxLayout();
	statLayout->addWidget(new ElaText("ElaStatCard", 15, this));
	statLayout->addSpacing(10);
	statLayout->addWidget(statCard1);
	statLayout->addWidget(statCard2);
	statLayout->addWidget(statCard3);
	statLayout->addStretch();

	// ========== ElaDropDownButton 示例 ==========
	_dropDownButton = new ElaDropDownButton(this);
	_dropDownButton->setText("排序方式");
	_dropDownButton->setElaIcon(ElaIconType::BarsSort);
	ElaMenu *dropMenu = new ElaMenu(this);
	dropMenu->addElaIconAction(ElaIconType::ArrowDownAZ, "按名称");
	dropMenu->addElaIconAction(ElaIconType::Calendar, "按日期");
	dropMenu->addElaIconAction(ElaIconType::ArrowDown19, "按大小");
	_dropDownButton->setMenu(dropMenu);

	ElaDropDownButton *dropBtn2 = new ElaDropDownButton(this);
	dropBtn2->setText("导出");
	ElaMenu *dropMenu2 = new ElaMenu(this);
	dropMenu2->addAction("PDF");
	dropMenu2->addAction("Excel");
	dropMenu2->addAction("CSV");
	dropBtn2->setMenu(dropMenu2);

	ElaScrollPageArea *dropArea = new ElaScrollPageArea(this);
	QHBoxLayout *dropLayout = new QHBoxLayout(dropArea);
	dropLayout->addWidget(new ElaText("ElaDropDownButton", 15, this));
	dropLayout->addWidget(_dropDownButton);
	dropLayout->addSpacing(10);
	dropLayout->addWidget(dropBtn2);
	dropLayout->addStretch();

	// ========== ElaSelectorBar 示例 ==========
	_selectorBar = new ElaSelectorBar(this);
	_selectorBar->addItem("全部");
	_selectorBar->addItem("未读");
	_selectorBar->addItem("已标记");
	_selectorBar->addItem("归档");

	ElaSelectorBar *viewSelector = new ElaSelectorBar(this);
	viewSelector->addItem(ElaIconType::List, "列表");
	viewSelector->addItem(ElaIconType::Grid2, "网格");
	viewSelector->addItem(ElaIconType::TableCells, "详情");

	ElaText *selectorInfo = new ElaText("选中: 全部", this);
	selectorInfo->setTextPixelSize(13);
	connect(_selectorBar, &ElaSelectorBar::currentIndexChanged, this, [=](int idx)
	{
		QStringList names = {"全部", "未读", "已标记", "归档"};
		selectorInfo->setText("选中: " + names.value(idx));
	});

	ElaScrollPageArea *selectorArea = new ElaScrollPageArea(this);
	QHBoxLayout *selectorLayout = new QHBoxLayout(selectorArea);
	selectorLayout->addWidget(new ElaText("ElaSelectorBar", 15, this));
	selectorLayout->addWidget(_selectorBar);
	selectorLayout->addWidget(selectorInfo);
	selectorLayout->addSpacing(10);
	selectorLayout->addWidget(viewSelector);
	selectorLayout->addStretch();

	// ========== ElaVirtualList 示例 ==========
	_virtualList = new ElaVirtualList(this);
	QStandardItemModel *virtualModel = new QStandardItemModel(this);
	for (int i = 0; i < 10000; ++i)
	{
		QStandardItem *item = new QStandardItem(QString("虚拟列表项 #%1 - 支持万级数据量").arg(i + 1));
		virtualModel->appendRow(item);
	}
	_virtualList->setModel(virtualModel);

	ElaText *virtualListInfo = new ElaText("已加载 10,000 条数据", this);
	virtualListInfo->setTextPixelSize(13);
	virtualListInfo->setMinimumWidth(200);

	ElaScrollPageArea *virtualListArea = new ElaScrollPageArea(this);
	virtualListArea->setFixedHeight(200);
	QVBoxLayout *virtualListLayout = new QVBoxLayout(virtualListArea);
	QHBoxLayout *virtualListHeader = new QHBoxLayout();
	virtualListHeader->addWidget(new ElaText("ElaVirtualList", 15, this));
	virtualListHeader->addWidget(virtualListInfo);
	virtualListHeader->addStretch();
	virtualListLayout->addLayout(virtualListHeader);
	virtualListLayout->addWidget(_virtualList);

	// ========== ElaSplashScreen 示例 ==========
	_splashScreen = new ElaSplashScreen(this);
	_splashScreen->setTitle("ElaWidgetTools");
	_splashScreen->setSubTitle("FluentUI 风格 Qt 组件库");
	_splashScreen->setStatusText("正在初始化...");
	_splashScreen->setIsShowProgressRing(true);

	ElaPushButton *splashBtn = new ElaPushButton("显示启动屏", this);
	splashBtn->setFixedSize(120, 38);
	connect(splashBtn, &ElaPushButton::clicked, this, [=]()
	{
		_splashScreen->setValue(0);
		_splashScreen->setStatusText("正在初始化...");
		_splashScreen->show();
		// 模拟加载过程
		QTimer *timer = new QTimer(this);
		int *progress = new int(0);
		connect(timer, &QTimer::timeout, this, [=]()
		{
			*progress += 5;
			_splashScreen->setValue(*progress);
			if (*progress >= 30)
				_splashScreen->setStatusText("正在加载组件...");
			if (*progress >= 60)
				_splashScreen->setStatusText("正在准备界面...");
			if (*progress >= 100)
			{
				timer->stop();
				timer->deleteLater();
				_splashScreen->setStatusText("加载完成!");
				QTimer::singleShot(500, this, [=]()
				{
					_splashScreen->close();
					delete progress;
				});
			}
		});
		timer->start(80);
	});

	ElaScrollPageArea *splashArea = new ElaScrollPageArea(this);
	QHBoxLayout *splashLayout = new QHBoxLayout(splashArea);
	splashLayout->addWidget(new ElaText("ElaSplashScreen", 15, this));
	splashLayout->addWidget(splashBtn);
	splashLayout->addStretch();

	// ========== ElaSheetPanel 示例 ==========
	_sheetPanel = nullptr; // 延迟创建，构造时 window() 可能还未就绪

	ElaPushButton *sheetPeekBtn = new ElaPushButton("Peek", this);
	sheetPeekBtn->setFixedSize(70, 32);
	connect(sheetPeekBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!_sheetPanel)
		{
			_sheetPanel = new ElaSheetPanel(window());
			_sheetPanel->setDragHandleVisible(true);
			_sheetPanel->setCloseOnOverlayClick(true);
			QWidget *sheetContent = new QWidget();
			QVBoxLayout *sheetContentLayout = new QVBoxLayout(sheetContent);
			sheetContentLayout->setContentsMargins(20, 10, 20, 10);
			sheetContentLayout->addWidget(new ElaText("Sheet 面板内容", 16, sheetContent));
			sheetContentLayout->addWidget(new ElaText("这是一个从底部滑出的半模态面板，支持拖拽调整高度。", 13, sheetContent));
			sheetContentLayout->addWidget(new ElaText("可以放置表单、详情或任意内容。", 13, sheetContent));
			ElaLineEdit *sheetInput = new ElaLineEdit(sheetContent);
			sheetInput->setPlaceholderText("在 Sheet 中输入内容...");
			sheetInput->setFixedHeight(35);
			sheetContentLayout->addWidget(sheetInput);
			sheetContentLayout->addStretch();
			_sheetPanel->setCentralWidget(sheetContent);
		}
		_sheetPanel->open(ElaSheetPanelType::Peek);
	});
	ElaPushButton *sheetHalfBtn = new ElaPushButton("Half", this);
	sheetHalfBtn->setFixedSize(70, 32);
	connect(sheetHalfBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!_sheetPanel)
		{
			sheetPeekBtn->click();
			return;
		}
		_sheetPanel->open(ElaSheetPanelType::Half);
	});
	ElaPushButton *sheetFullBtn = new ElaPushButton("Full", this);
	sheetFullBtn->setFixedSize(70, 32);
	connect(sheetFullBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!_sheetPanel)
		{
			sheetPeekBtn->click();
			return;
		}
		_sheetPanel->open(ElaSheetPanelType::Full);
	});

	ElaScrollPageArea *sheetArea = new ElaScrollPageArea(this);
	QHBoxLayout *sheetLayout = new QHBoxLayout(sheetArea);
	sheetLayout->addWidget(new ElaText("ElaSheetPanel", 15, this));
	sheetLayout->addWidget(sheetPeekBtn);
	sheetLayout->addWidget(sheetHalfBtn);
	sheetLayout->addWidget(sheetFullBtn);
	sheetLayout->addStretch();


	// ========== ElaTransfer 示例 ==========
	_transfer = new ElaTransfer(this);
	_transfer->setFixedHeight(280);
	_transfer->setSourceTitle("可选项目");
	_transfer->setTargetTitle("已选项目");
	QStringList transferItems;
	for (int i = 1; i <= 20; ++i)
	{
		transferItems.append(QString("项目 %1").arg(i));
	}
	_transfer->setSourceItems(transferItems);

	ElaText *transferResult = new ElaText("已选: 0 项", this);
	transferResult->setTextPixelSize(13);
	connect(_transfer, &ElaTransfer::transferChanged, this, [=](const QStringList &, const QStringList &target)
	{
		transferResult->setText(QString("已选: %1 项").arg(target.size()));
	});

	QHBoxLayout *transferHeader = new QHBoxLayout();
	transferHeader->addWidget(new ElaText("ElaTransfer", 15, this));
	transferHeader->addWidget(transferResult);
	transferHeader->addStretch();

	// ========== ElaSpotlight 示例 ==========
	_spotlight = nullptr; // 延迟创建

	ElaPushButton *spotlightSingleBtn = new ElaPushButton("单目标聚光", this);
	spotlightSingleBtn->setFixedSize(110, 38);
	connect(spotlightSingleBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!_spotlight)
		{
			_spotlight = new ElaSpotlight(window());
		}
		_spotlight->setTitle("ElaSplashScreen");
		_spotlight->setContent("点击此按钮可以展示启动屏效果。");
		_spotlight->showSpotlight(splashBtn, "我知道了");
	});

	ElaPushButton *spotlightTourBtn = new ElaPushButton("多步骤引导", this);
	spotlightTourBtn->setFixedSize(110, 38);
	connect(spotlightTourBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (!_spotlight)
		{
			_spotlight = new ElaSpotlight(window());
		}
		QList<ElaSpotlight::SpotlightStep> steps;
		steps.append({splashBtn, "启动屏", "展示应用启动画面效果", false});
		steps.append({sheetPeekBtn, "底部面板", "从底部滑出半模态面板", false});
		steps.append({spotlightSingleBtn, "聚光灯", "高亮任意目标区域", false});
		_spotlight->setSteps(steps);
		_spotlight->start();
	});

	ElaScrollPageArea *spotlightArea = new ElaScrollPageArea(this);
	QHBoxLayout *spotlightLayout = new QHBoxLayout(spotlightArea);
	spotlightLayout->addWidget(new ElaText("ElaSpotlight", 15, this));
	spotlightLayout->addWidget(spotlightSingleBtn);
	spotlightLayout->addWidget(spotlightTourBtn);
	spotlightLayout->addStretch();


	// ========== ElaCountdown 示例 ==========
	_countdown = new ElaCountdown(this);
	_countdown->setRemainingSeconds(86400 + 3661);
	_countdown->setIsShowDays(true);

	ElaPushButton *countdownStartBtn = new ElaPushButton("开始", this);
	countdownStartBtn->setFixedSize(60, 32);
	ElaPushButton *countdownPauseBtn = new ElaPushButton("暂停", this);
	countdownPauseBtn->setFixedSize(60, 32);
	ElaPushButton *countdownResetBtn = new ElaPushButton("重置", this);
	countdownResetBtn->setFixedSize(60, 32);
	connect(countdownStartBtn, &ElaPushButton::clicked, this, [=]()
	{
		if (_countdown->isRunning())
			_countdown->resume();
		else
			_countdown->start();
	});
	connect(countdownPauseBtn, &ElaPushButton::clicked, _countdown, &ElaCountdown::pause);
	connect(countdownResetBtn, &ElaPushButton::clicked, this, [=]()
	{
		_countdown->stop();
		_countdown->setRemainingSeconds(86400 + 3661);
	});

	ElaCountdown *countdownTarget = new ElaCountdown(this);
	countdownTarget->setIsShowDays(false);
	countdownTarget->setTargetDateTime(QDateTime::currentDateTime().addSecs(7200));
	countdownTarget->start();

	ElaScrollPageArea *countdownArea = new ElaScrollPageArea(this);
	countdownArea->setFixedHeight(130);
	QVBoxLayout *countdownMainLayout = new QVBoxLayout(countdownArea);
	QHBoxLayout *countdownHeader = new QHBoxLayout();
	countdownHeader->addWidget(new ElaText("ElaCountdown", 15, this));
	countdownHeader->addWidget(countdownStartBtn);
	countdownHeader->addWidget(countdownPauseBtn);
	countdownHeader->addWidget(countdownResetBtn);
	countdownHeader->addStretch();
	countdownMainLayout->addLayout(countdownHeader);
	QHBoxLayout *countdownBody = new QHBoxLayout();
	countdownBody->addWidget(_countdown);
	countdownBody->addSpacing(20);
	countdownBody->addWidget(new ElaText("目标时间 (2小时后):", 13, this));
	countdownBody->addWidget(countdownTarget);
	countdownBody->addStretch();
	countdownMainLayout->addLayout(countdownBody);

	// ========== ElaPopconfirm 示例 ==========
	_popconfirm = new ElaPopconfirm(this);
	_popconfirm->setTitle("确认删除");
	_popconfirm->setContent("删除后将无法恢复，是否继续？");

	ElaPushButton *popconfirmBtn = new ElaPushButton("删除项目", this);
	popconfirmBtn->setFixedSize(100, 38);
	connect(popconfirmBtn, &ElaPushButton::clicked, this, [=]()
	{
		_popconfirm->showPopconfirm(popconfirmBtn);
	});

	ElaText *popconfirmResult = new ElaText("", this);
	popconfirmResult->setTextPixelSize(13);
	connect(_popconfirm, &ElaPopconfirm::confirmed, this, [=]()
	{
		popconfirmResult->setText("已确认删除");
	});
	connect(_popconfirm, &ElaPopconfirm::cancelled, this, [=]()
	{
		popconfirmResult->setText("已取消");
	});

	ElaPopconfirm *customPopconfirm = new ElaPopconfirm(this);
	customPopconfirm->setTitle("提交变更");
	customPopconfirm->setContent("确定要提交到远程仓库吗？");
	customPopconfirm->setConfirmButtonText("提交");
	customPopconfirm->setCancelButtonText("再想想");
	customPopconfirm->setIcon(ElaIconType::CloudArrowUp);

	ElaPushButton *customPopconfirmBtn = new ElaPushButton("提交代码", this);
	customPopconfirmBtn->setFixedSize(100, 38);
	connect(customPopconfirmBtn, &ElaPushButton::clicked, this, [=]()
	{
		customPopconfirm->showPopconfirm(customPopconfirmBtn);
	});

	ElaScrollPageArea *popconfirmArea = new ElaScrollPageArea(this);
	QHBoxLayout *popconfirmLayout = new QHBoxLayout(popconfirmArea);
	popconfirmLayout->addWidget(new ElaText("ElaPopconfirm", 15, this));
	popconfirmLayout->addWidget(popconfirmBtn);
	popconfirmLayout->addWidget(popconfirmResult);
	popconfirmLayout->addSpacing(10);
	popconfirmLayout->addWidget(customPopconfirmBtn);
	popconfirmLayout->addStretch();

	// ========== ElaWatermark 示例 ==========
	_watermark = new ElaWatermark("ElaWidgetTools", window());
	_watermark->setVisible(false);

	ElaPushButton *watermarkShowBtn = new ElaPushButton("显示", this);
	watermarkShowBtn->setFixedSize(60, 36);
	ElaPushButton *watermarkHideBtn = new ElaPushButton("隐藏", this);
	watermarkHideBtn->setFixedSize(60, 36);
	connect(watermarkShowBtn, &ElaPushButton::clicked, this, [=]()
	{
		_watermark->setVisible(true);
	});
	connect(watermarkHideBtn, &ElaPushButton::clicked, this, [=]()
	{
		_watermark->setVisible(false);
	});

	ElaLineEdit *watermarkTextEdit = new ElaLineEdit(this);
	watermarkTextEdit->setText("ElaWidgetTools");
	watermarkTextEdit->setFixedWidth(140);
	connect(watermarkTextEdit, &ElaLineEdit::textChanged, this, [=](const QString &text)
	{
		_watermark->setText(text);
	});

	ElaSlider *watermarkOpacitySlider = new ElaSlider(this);
	watermarkOpacitySlider->setRange(1, 100);
	watermarkOpacitySlider->setValue(50);
	watermarkOpacitySlider->setFixedWidth(120);
	connect(watermarkOpacitySlider, &ElaSlider::valueChanged, this, [=](int v)
	{
		_watermark->setOpacity(v / 100.0);
	});

	ElaSlider *watermarkRotationSlider = new ElaSlider(this);
	watermarkRotationSlider->setRange(-90, 90);
	watermarkRotationSlider->setValue(-22);
	watermarkRotationSlider->setFixedWidth(120);
	connect(watermarkRotationSlider, &ElaSlider::valueChanged, this, [=](int v)
	{
		_watermark->setRotation(v);
	});

	ElaScrollPageArea *watermarkArea = new ElaScrollPageArea(this);
	QHBoxLayout *watermarkLayout = new QHBoxLayout(watermarkArea);
	watermarkLayout->addWidget(new ElaText("ElaWatermark", 15, this));
	watermarkLayout->addSpacing(20);
	watermarkLayout->addWidget(watermarkShowBtn);
	watermarkLayout->addWidget(watermarkHideBtn);
	watermarkLayout->addSpacing(10);
	watermarkLayout->addWidget(watermarkTextEdit);
	watermarkLayout->addSpacing(10);
	watermarkLayout->addWidget(new ElaText("透明度", 13, this));
	watermarkLayout->addWidget(watermarkOpacitySlider);
	watermarkLayout->addSpacing(10);
	watermarkLayout->addWidget(new ElaText("角度", 13, this));
	watermarkLayout->addWidget(watermarkRotationSlider);
	watermarkLayout->addStretch();

	// ========== ElaSnackbar 示例 ==========
	ElaPushButton *snackSuccessBtn = new ElaPushButton("成功", this);
	snackSuccessBtn->setFixedSize(70, 32);
	connect(snackSuccessBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaSnackbar::success("文件保存成功!", "", 3000, window());
	});

	ElaPushButton *snackUndoBtn = new ElaPushButton("撤销操作", this);
	snackUndoBtn->setFixedSize(90, 32);
	connect(snackUndoBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaSnackbar *snackbar = ElaSnackbar::info("已删除 3 个项目", "撤销", 5000, window());
		connect(snackbar, &ElaSnackbar::actionClicked, this, [=]()
		{
			ElaToast::success("已撤销删除操作", 2000, window());
		});
	});

	ElaPushButton *snackWarnBtn = new ElaPushButton("警告", this);
	snackWarnBtn->setFixedSize(70, 32);
	connect(snackWarnBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaSnackbar::warning("磁盘空间不足", "查看详情", 4000, window());
	});

	ElaPushButton *snackErrorBtn = new ElaPushButton("错误", this);
	snackErrorBtn->setFixedSize(70, 32);
	connect(snackErrorBtn, &ElaPushButton::clicked, this, [=]()
	{
		ElaSnackbar::error("网络连接失败", "重试", 4000, window());
	});

	ElaScrollPageArea *snackbarArea = new ElaScrollPageArea(this);
	QHBoxLayout *snackbarLayout = new QHBoxLayout(snackbarArea);
	snackbarLayout->addWidget(new ElaText("ElaSnackbar", 15, this));
	snackbarLayout->addWidget(snackSuccessBtn);
	snackbarLayout->addWidget(snackUndoBtn);
	snackbarLayout->addWidget(snackWarnBtn);
	snackbarLayout->addWidget(snackErrorBtn);
	snackbarLayout->addStretch();

	// ========== ElaSplitter 示例 ==========
	_splitter = new ElaSplitter(Qt::Horizontal, this);
	_splitter->setFixedHeight(200);

	QWidget *splitterLeft = new QWidget(this);
	splitterLeft->setMinimumWidth(120);
	QVBoxLayout *leftLayout = new QVBoxLayout(splitterLeft);
	leftLayout->setContentsMargins(10, 10, 10, 10);
	ElaText *leftTitle = new ElaText("左侧面板", this);
	leftTitle->setTextPixelSize(14);
	leftLayout->addWidget(leftTitle);
	for (int i = 1; i <= 5; ++i)
	{
		leftLayout->addWidget(new ElaText(QString("  列表项 #%1").arg(i), 13, this));
	}
	leftLayout->addStretch();

	QWidget *splitterRight = new QWidget(this);
	splitterRight->setMinimumWidth(120);
	QVBoxLayout *rightLayout = new QVBoxLayout(splitterRight);
	rightLayout->setContentsMargins(10, 10, 10, 10);
	ElaText *rightTitle = new ElaText("右侧面板", this);
	rightTitle->setTextPixelSize(14);
	rightLayout->addWidget(rightTitle);
	rightLayout->addWidget(new ElaText("拖拽中间手柄调整面板大小", 13, this));
	rightLayout->addStretch();

	_splitter->addWidget(splitterLeft);
	_splitter->addWidget(splitterRight);
	_splitter->setSizes({300, 300});

	ElaSplitter *vSplitter = new ElaSplitter(Qt::Vertical, this);
	vSplitter->setFixedHeight(200);

	QWidget *splitterTop = new QWidget(this);
	splitterTop->setMinimumHeight(40);
	QVBoxLayout *topLayout = new QVBoxLayout(splitterTop);
	topLayout->setContentsMargins(10, 10, 10, 10);
	topLayout->addWidget(new ElaText("上方面板", 14, this));
	topLayout->addStretch();

	QWidget *splitterBottom = new QWidget(this);
	splitterBottom->setMinimumHeight(40);
	QVBoxLayout *bottomLayout = new QVBoxLayout(splitterBottom);
	bottomLayout->setContentsMargins(10, 10, 10, 10);
	bottomLayout->addWidget(new ElaText("下方面板", 14, this));
	bottomLayout->addStretch();

	vSplitter->addWidget(splitterTop);
	vSplitter->addWidget(splitterBottom);
	vSplitter->setSizes({100, 100});

	ElaScrollPageArea *splitterArea = new ElaScrollPageArea(this);
	splitterArea->setFixedHeight(460);
	QVBoxLayout *splitterMainLayout = new QVBoxLayout(splitterArea);
	splitterMainLayout->addWidget(new ElaText("ElaSplitter", 15, this));
	splitterMainLayout->addSpacing(5);
	splitterMainLayout->addWidget(new ElaText("水平分割", 13, this));
	splitterMainLayout->addWidget(_splitter);
	splitterMainLayout->addSpacing(5);
	splitterMainLayout->addWidget(new ElaText("垂直分割", 13, this));
	splitterMainLayout->addWidget(vSplitter);

	// ========== ElaInfoBar 示例 ==========
	_infoBarInfo = new ElaInfoBar(ElaInfoBarType::Informational, this);
	_infoBarInfo->setTitle("提示");
	_infoBarInfo->setMessage("这是一条信息提示，用于展示一般性的通知内容。");

	_infoBarSuccess = new ElaInfoBar(ElaInfoBarType::Success, this);
	_infoBarSuccess->setTitle("成功");
	_infoBarSuccess->setMessage("文件已成功保存到指定路径。");
	_infoBarSuccess->addAction("查看", [=]() { qDebug() << "查看按钮被点击"; });

	_infoBarWarning = new ElaInfoBar(ElaInfoBarType::Warning, this);
	_infoBarWarning->setTitle("警告");
	_infoBarWarning->setMessage("你的许可证将在 7 天后过期。");
	_infoBarWarning->addAction("续费", [=]() { qDebug() << "续费按钮被点击"; });
	_infoBarWarning->addAction("稍后提醒", [=]() { qDebug() << "稍后提醒按钮被点击"; });

	_infoBarError = new ElaInfoBar(ElaInfoBarType::Error, this);
	_infoBarError->setTitle("错误");
	_infoBarError->setMessage("无法连接到服务器，请检查网络设置。");
	_infoBarError->setIsClosable(false);
	_infoBarError->addAction("重试", [=]() { qDebug() << "重试按钮被点击"; });

	// ========== 中心布局 ==========
	QVBoxLayout *c = new QVBoxLayout(centralWidget);
	c->setContentsMargins(0, 0, 0, 0);
	c->addWidget(badgeArea);
	c->addWidget(badgeControlArea);
	c->addWidget(severityArea);
	c->addWidget(passwordArea);
	c->addWidget(numberArea);
	c->addWidget(tagArea);
	c->addWidget(tagCloseArea);
	c->addWidget(tagCheckArea);
	c->addWidget(splitArea);
	c->addWidget(personArea);
	c->addWidget(ratingArea);
	c->addWidget(flyoutArea);
	c->addWidget(teachingTipArea);
	c->addWidget(tipPosArea);
	c->addWidget(divider1);
	c->addWidget(skelArea);
	c->addWidget(divider2);
	c->addLayout(stepsCtrl);
	c->addWidget(_steps);
	c->addWidget(cmdArea);
	c->addWidget(pgArea);
	c->addWidget(pageStack);
	c->addWidget(expanderText);
	c->addWidget(_expander);
	c->addWidget(expanderUpText);
	c->addWidget(_expanderUp);
	c->addWidget(captchaArea);
	c->addLayout(statLayout);
	c->addWidget(dropArea);
	c->addWidget(selectorArea);
	c->addWidget(toastArea);
	c->addWidget(wizardArea);
	c->addWidget(notifArea);
	c->addSpacing(5);
	c->addWidget(new ElaText("ElaMarkdownViewer", 15, this));
	c->addWidget(_markdownViewer);
	c->addSpacing(5);
	c->addWidget(new ElaText("ElaTimeline", 15, this));
	c->addWidget(_timeline);
	c->addSpacing(10);
	c->addWidget(virtualListArea);
	c->addWidget(splashArea);
	c->addWidget(sheetArea);
	c->addWidget(spotlightArea);
	c->addWidget(countdownArea);
	c->addWidget(popconfirmArea);
	c->addWidget(watermarkArea);
	c->addWidget(snackbarArea);
	c->addWidget(splitterArea);

	// ========== ElaQRCode ==========
	_qrCode = new ElaQRCode("https://github.com/RainbowCandyX/ElaWidgetTools", this);
	_qrCode->setFixedSize(180, 180);

	ElaLineEdit *qrInput = new ElaLineEdit(this);
	qrInput->setText("https://github.com/RainbowCandyX/ElaWidgetTools");
	qrInput->setFixedHeight(35);
	qrInput->setMinimumWidth(300);
	connect(qrInput, &ElaLineEdit::textChanged, this, [=](const QString &text)
	{
		_qrCode->setText(text);
	});

	ElaComboBox *qrEcLevel = new ElaComboBox(this);
	qrEcLevel->addItems({"低 (7%)", "中 (15%)", "较高 (25%)", "高 (30%)"});
	qrEcLevel->setCurrentIndex(1);
	connect(qrEcLevel, &ElaComboBox::currentIndexChanged, this, [=](int index)
	{
		_qrCode->setErrorCorrectionLevel(static_cast<ElaQRCode::ErrorCorrectionLevel>(index));
	});

	ElaScrollPageArea *qrArea = new ElaScrollPageArea(this);
	qrArea->setFixedHeight(240);
	QVBoxLayout *qrMainLayout = new QVBoxLayout(qrArea);
	QHBoxLayout *qrHeader = new QHBoxLayout();
	qrHeader->addWidget(new ElaText("ElaQRCode", 15, this));
	qrHeader->addWidget(qrInput);
	qrHeader->addSpacing(10);
	qrHeader->addWidget(new ElaText("纠错:", 13, this));
	qrHeader->addWidget(qrEcLevel);
	qrHeader->addStretch();
	qrMainLayout->addLayout(qrHeader);
	QHBoxLayout *qrBody = new QHBoxLayout();
	qrBody->addWidget(_qrCode);
	qrBody->addStretch();
	qrMainLayout->addLayout(qrBody);
	c->addWidget(qrArea);

	_emojiPicker = new ElaEmojiPicker(this);

	ElaLineEdit *emojiInput = new ElaLineEdit(this);
	emojiInput->setPlaceholderText(QString::fromUtf8("点击表情按钮插入..."));
	emojiInput->setFixedHeight(35);
	emojiInput->setMinimumWidth(300);

	ElaPushButton *emojiBtn = new ElaPushButton(QString::fromUtf8("😀"), this);
	emojiBtn->setFixedSize(40, 35);
	connect(emojiBtn, &ElaPushButton::clicked, this, [=]()
	{
		_emojiPicker->popup(emojiBtn);
	});
	connect(_emojiPicker, &ElaEmojiPicker::emojiSelected, this, [=](const QString &emoji)
	{
		emojiInput->setText(emojiInput->text() + emoji);
	});

	ElaScrollPageArea *emojiArea = new ElaScrollPageArea(this);
	QHBoxLayout *emojiLayout = new QHBoxLayout(emojiArea);
	emojiLayout->addWidget(new ElaText("ElaEmojiPicker", 15, this));
	emojiLayout->addSpacing(10);
	emojiLayout->addWidget(emojiInput);
	emojiLayout->addWidget(emojiBtn);
	emojiLayout->addStretch();
	c->addWidget(emojiArea);

	_floatButton = new ElaFloatButton(ElaIconType::Plus, this);
	ElaMenu *floatMenu = new ElaMenu(this);
	floatMenu->addElaIconAction(ElaIconType::Pen, "新建笔记");
	floatMenu->addElaIconAction(ElaIconType::Upload, "上传文件");
	floatMenu->addElaIconAction(ElaIconType::Share, "分享");
	_floatButton->setMenu(floatMenu);

	ElaScrollPageArea *floatBtnArea = new ElaScrollPageArea(this);
	QHBoxLayout *floatBtnLayout = new QHBoxLayout(floatBtnArea);
	floatBtnLayout->addWidget(new ElaText("ElaFloatButton", 15, this));
	floatBtnLayout->addSpacing(10);
	floatBtnLayout->addWidget(new ElaText("位置:", 13, this));
	ElaComboBox *floatPosCombo = new ElaComboBox(this);
	floatPosCombo->addItems({"右下", "左下", "右上", "左上"});
	connect(floatPosCombo, &ElaComboBox::currentIndexChanged, this, [=](int index)
	{
		_floatButton->setPosition(static_cast<ElaFloatButton::Position>(index));
	});
	floatBtnLayout->addWidget(floatPosCombo);
	floatBtnLayout->addSpacing(10);
	floatBtnLayout->addWidget(new ElaText("边距:", 13, this));
	ElaSlider *floatMarginSlider = new ElaSlider(this);
	floatMarginSlider->setRange(10, 100);
	floatMarginSlider->setValue(30);
	floatMarginSlider->setFixedWidth(150);
	connect(floatMarginSlider, &ElaSlider::valueChanged, this, [=](int value)
	{
		_floatButton->setMargin(value);
	});
	floatBtnLayout->addWidget(floatMarginSlider);
	floatBtnLayout->addStretch();
	c->addWidget(floatBtnArea);

	c->addSpacing(5);
	c->addWidget(new ElaText("ElaInfoBar", 15, this));
	c->addWidget(_infoBarInfo);
	c->addWidget(_infoBarSuccess);
	c->addWidget(_infoBarWarning);
	c->addWidget(_infoBarError);

	// ========== ElaAutoComplete 示例 ==========
	_autoComplete = new ElaAutoComplete(this);
	_autoComplete->setPlaceholderText("输入城市名称...");
	_autoComplete->setCompletions({
		"北京", "北京大兴机场", "北京西站", "上海", "上海虹桥", "上海浦东",
		"广州", "广州南站", "深圳", "深圳北站", "杭州", "杭州西湖",
		"成都", "成都天府", "武汉", "南京", "南京南站", "西安",
		"重庆", "天津", "苏州", "长沙",
		"Singapore", "Hong Kong", "Taiwan", "Tokyo", "Seoul",
		"Bangkok", "New York", "London", "Paris", "Sydney",
		"Los Angeles", "San Francisco", "Chicago", "Toronto", "Vancouver",
		"Berlin", "Amsterdam", "Dubai", "Mumbai", "Melbourne",
		"Oslo", "Stockholm", "Helsinki", "Lisbon", "Barcelona"
	});

	ElaText *autoCompleteResult = new ElaText("", this);
	autoCompleteResult->setTextPixelSize(13);
	connect(_autoComplete, &ElaAutoComplete::completionSelected, this, [=](const QString &text)
	{
		autoCompleteResult->setText(QString("已选择: %1").arg(text));
	});

	ElaComboBox *matchModeCombo = new ElaComboBox(this);
	matchModeCombo->addItems({"包含匹配", "前缀匹配", "后缀匹配", "正则表达式"});
	connect(matchModeCombo, &ElaComboBox::currentIndexChanged, this, [=](int index)
	{
		_autoComplete->setMatchMode(static_cast<ElaAutoComplete::MatchMode>(index));
	});

	ElaScrollPageArea *autoCompleteArea = new ElaScrollPageArea(this);
	QHBoxLayout *autoCompleteLayout = new QHBoxLayout(autoCompleteArea);
	autoCompleteLayout->addWidget(new ElaText("ElaAutoComplete", 15, this));
	autoCompleteLayout->addSpacing(10);
	autoCompleteLayout->addWidget(_autoComplete);
	autoCompleteLayout->addSpacing(10);
	autoCompleteLayout->addWidget(matchModeCombo);
	autoCompleteLayout->addSpacing(10);
	autoCompleteLayout->addWidget(autoCompleteResult);
	autoCompleteLayout->addStretch();
	c->addWidget(autoCompleteArea);

	// ========== ElaTreeSelect 示例 ==========
	_treeSelect = new ElaTreeSelect(this);
	_treeSelect->setPlaceholderText("请选择分类...");
	_treeSelect->setMinimumWidth(280);

	QStandardItemModel *treeModel = new QStandardItemModel(this);
	QStandardItem *china = new QStandardItem("中国");
	QStandardItem *beijing = new QStandardItem("北京");
	beijing->appendRow(new QStandardItem("朝阳区"));
	beijing->appendRow(new QStandardItem("海淀区"));
	beijing->appendRow(new QStandardItem("西城区"));
	china->appendRow(beijing);
	QStandardItem *shanghai = new QStandardItem("上海");
	shanghai->appendRow(new QStandardItem("浦东新区"));
	shanghai->appendRow(new QStandardItem("黄浦区"));
	china->appendRow(shanghai);
	QStandardItem *guangdong = new QStandardItem("广东");
	guangdong->appendRow(new QStandardItem("深圳"));
	guangdong->appendRow(new QStandardItem("广州"));
	china->appendRow(guangdong);
	treeModel->appendRow(china);

	QStandardItem *usa = new QStandardItem("美国");
	QStandardItem *california = new QStandardItem("加利福尼亚");
	california->appendRow(new QStandardItem("洛杉矶"));
	california->appendRow(new QStandardItem("旧金山"));
	usa->appendRow(california);
	QStandardItem *newYork = new QStandardItem("纽约州");
	newYork->appendRow(new QStandardItem("纽约市"));
	usa->appendRow(newYork);
	treeModel->appendRow(usa);

	_treeSelect->setModel(treeModel);

	ElaText *treeSelectResult = new ElaText("", this);
	treeSelectResult->setTextPixelSize(13);
	connect(_treeSelect, &ElaTreeSelect::currentTextChanged, this, [=](const QString &text)
	{
		treeSelectResult->setText(QString("已选择: %1").arg(text));
	});

	ElaScrollPageArea *treeSelectArea = new ElaScrollPageArea(this);
	QHBoxLayout *treeSelectLayout = new QHBoxLayout(treeSelectArea);
	treeSelectLayout->addWidget(new ElaText("ElaTreeSelect", 15, this));
	treeSelectLayout->addSpacing(10);
	treeSelectLayout->addWidget(_treeSelect);
	treeSelectLayout->addSpacing(10);
	treeSelectLayout->addWidget(treeSelectResult);
	treeSelectLayout->addStretch();
	c->addWidget(treeSelectArea);

	// ========== ElaUploadArea 示例 ==========
	_uploadArea = new ElaUploadArea(this);
	_uploadArea->setFixedHeight(200);
	_uploadArea->setAcceptedSuffixes({"png", "jpg", "jpeg", "gif", "bmp", "pdf", "txt"});
	_uploadArea->setMaxFileCount(5);
	_uploadArea->setTitle("拖拽文件到此处");
	_uploadArea->setSubTitle("支持 png, jpg, gif, bmp, pdf, txt (最多5个)");
	_uploadArea->setDialogTitle("选择要上传的文件");

	ElaText *uploadInfo = new ElaText("已选择 0 个文件", this);
	uploadInfo->setTextPixelSize(13);
	connect(_uploadArea, &ElaUploadArea::filesSelected, this, [=](const QStringList &files)
	{
		uploadInfo->setText(QString("已选择 %1 个文件").arg(files.size()));
	});
	connect(_uploadArea, &ElaUploadArea::fileRejected, this, [=](const QString &path, const QString &reason)
	{
		Q_UNUSED(path)
		ElaToast::warning(reason, 2000, window());
	});

	ElaPushButton *clearUploadBtn = new ElaPushButton("清空文件", this);
	clearUploadBtn->setFixedSize(100, 35);
	connect(clearUploadBtn, &ElaPushButton::clicked, this, [=]()
	{
		_uploadArea->clearFiles();
		uploadInfo->setText("已选择 0 个文件");
	});

	ElaScrollPageArea *uploadAreaContainer = new ElaScrollPageArea(this);
	uploadAreaContainer->setFixedHeight(280);
	QVBoxLayout *uploadMainLayout = new QVBoxLayout(uploadAreaContainer);
	QHBoxLayout *uploadHeader = new QHBoxLayout();
	uploadHeader->addWidget(new ElaText("ElaUploadArea", 15, this));
	uploadHeader->addSpacing(10);
	uploadHeader->addWidget(uploadInfo);
	uploadHeader->addSpacing(10);
	uploadHeader->addWidget(clearUploadBtn);
	uploadHeader->addStretch();
	uploadMainLayout->addLayout(uploadHeader);
	uploadMainLayout->addWidget(_uploadArea);
	c->addWidget(uploadAreaContainer);

	c->addSpacing(5);
	c->addLayout(transferHeader);
	c->addWidget(_transfer);
	c->addStretch();
	addCentralWidget(centralWidget, true, false, 0);
}

T_NewComponents::~T_NewComponents()
{
}