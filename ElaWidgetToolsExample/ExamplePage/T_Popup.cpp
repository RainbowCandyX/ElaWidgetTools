#include "T_Popup.h"

#include "ElaCalendar.h"
#include "ElaCalendarPicker.h"
#include "ElaCheckBox.h"
#include "ElaColorDialog.h"
#include "ElaDrawerArea.h"
#include "ElaInputDialog.h"
#include "ElaKeyBinder.h"
#include "ElaMenu.h"
#include "ElaPushButton.h"
#include "ElaRoller.h"
#include "ElaRollerPicker.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include "ElaToolButton.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaPopup");

    // 顶部元素
    createCustomWidget("一些常用的弹出组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaPopup");

    _toolButton = new ElaToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _toolButton->setText("ElaToolButton");

    ElaMenu* menu = new ElaMenu(this);
    menu->addElaIconAction(ElaIconType::JackOLantern, "JackOLantern");
    menu->addElaIconAction(ElaIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);
    _toolButton->setElaIcon(ElaIconType::Broom);

    ElaScrollPageArea* toolButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* toolButtonLayout = new QHBoxLayout(toolButtonArea);
    ElaText* toolButtonText = new ElaText("ElaToolButton", this);
    toolButtonText->setTextPixelSize(15);
    toolButtonLayout->addWidget(toolButtonText);
    toolButtonLayout->addWidget(_toolButton);
    toolButtonLayout->addStretch();

    _colorDialog = new ElaColorDialog(this);
    ElaText* colorText = new ElaText(_colorDialog->getCurrentColorRGB(), this);
    colorText->setTextPixelSize(15);
    ElaPushButton* colorDialogButton = new ElaPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
    connect(colorDialogButton, &ElaPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        colorDialogButton->setLightDefaultColor(color);
        colorDialogButton->setLightHoverColor(color);
        colorDialogButton->setLightPressColor(color);
        colorDialogButton->setDarkDefaultColor(color);
        colorDialogButton->setDarkHoverColor(color);
        colorDialogButton->setDarkPressColor(color);
        colorText->setText(_colorDialog->getCurrentColorRGB());
    });

    ElaScrollPageArea* colorDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* colorDialogLayout = new QHBoxLayout(colorDialogArea);
    ElaText* colorDialogText = new ElaText("ElaColorDialog", this);
    colorDialogText->setTextPixelSize(15);
    colorDialogLayout->addWidget(colorDialogText);
    colorDialogLayout->addWidget(colorDialogButton);
    colorDialogLayout->addWidget(colorText);
    colorDialogLayout->addStretch();

    // ElaInputDialog - 文本输入示例
    ElaText* inputResultText = new ElaText("未输入", this);
    inputResultText->setTextPixelSize(15);
    ElaPushButton* inputDialogButton = new ElaPushButton("打开输入对话框", this);
    inputDialogButton->setFixedHeight(38);
    connect(inputDialogButton, &ElaPushButton::clicked, this, [=]() {
        bool ok;
        QString text = ElaInputDialog::getText(
            this,
            "输入对话框",            // 标题
            "请输入您的信息",        // 副标题
            "名称:",                 // 标签
            "",                     // 默认文本
            &ok,
            "确定",                 // 确定按钮文本
            "取消"                  // 取消按钮文本
        );
        if (ok && !text.isEmpty())
        {
            inputResultText->setText(QString("您输入的内容: %1").arg(text));
        }
    });

    ElaScrollPageArea* inputDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* inputDialogLayout = new QHBoxLayout(inputDialogArea);
    ElaText* inputDialogText = new ElaText("ElaInputDialog", this);
    inputDialogText->setTextPixelSize(15);
    inputDialogLayout->addWidget(inputDialogText);
    inputDialogLayout->addWidget(inputDialogButton);
    inputDialogLayout->addWidget(inputResultText);
    inputDialogLayout->addStretch();

    // ElaInputDialog - 整数输入示例
    ElaText* intResultText = new ElaText("未输入", this);
    intResultText->setTextPixelSize(15);
    ElaPushButton* intInputButton = new ElaPushButton("打开整数输入对话框", this);
    intInputButton->setFixedHeight(38);
    connect(intInputButton, &ElaPushButton::clicked, this, [=]() {
        bool ok;
        int value = ElaInputDialog::getInt(
            this,
            "输入年龄",              // 标题
            "请提供您的个人信息",    // 副标题
            "年龄:",                 // 标签
            18,                     // 默认值
            0, 150, 1,             // 最小值, 最大值, 步长
            &ok,
            "确定",                 // 确定按钮文本
            "取消"                  // 取消按钮文本
        );
        if (ok)
        {
            intResultText->setText(QString("您输入的年龄: %1").arg(value));
        }
    });

    ElaScrollPageArea* intInputDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* intInputDialogLayout = new QHBoxLayout(intInputDialogArea);
    ElaText* intInputDialogText = new ElaText("ElaInputDialog (Int)", this);
    intInputDialogText->setTextPixelSize(15);
    intInputDialogLayout->addWidget(intInputDialogText);
    intInputDialogLayout->addWidget(intInputButton);
    intInputDialogLayout->addWidget(intResultText);
    intInputDialogLayout->addStretch();

    // ElaInputDialog - 双精度浮点数输入
    ElaText* doubleResultText = new ElaText("未输入", this);
    doubleResultText->setTextPixelSize(15);
    ElaPushButton* doubleInputButton = new ElaPushButton("打开小数输入对话框", this);
    doubleInputButton->setFixedHeight(38);
    connect(doubleInputButton, &ElaPushButton::clicked, this, [=]() {
        bool ok;
        double value = ElaInputDialog::getDouble(
            this,
            "输入价格",              // 标题
            "商品定价系统",          // 副标题
            "价格:",                 // 标签
            99.99,                  // 默认值
            0.0, 9999.99, 2,       // 最小值, 最大值, 小数位数
            &ok,
            "确定",                 // 确定按钮文本
            "取消"                  // 取消按钮文本
        );
        if (ok)
        {
            doubleResultText->setText(QString("您输入的价格: ¥%1").arg(value, 0, 'f', 2));
        }
    });

    ElaScrollPageArea* doubleInputDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* doubleInputDialogLayout = new QHBoxLayout(doubleInputDialogArea);
    ElaText* doubleInputDialogText = new ElaText("ElaInputDialog (Double)", this);
    doubleInputDialogText->setTextPixelSize(15);
    doubleInputDialogLayout->addWidget(doubleInputDialogText);
    doubleInputDialogLayout->addWidget(doubleInputButton);
    doubleInputDialogLayout->addWidget(doubleResultText);
    doubleInputDialogLayout->addStretch();

    // ElaInputDialog - 多行文本输入示例
    ElaText* multiLineResultText = new ElaText("未输入", this);
    multiLineResultText->setTextPixelSize(15);
    multiLineResultText->setWordWrap(true);
    ElaPushButton* multiLineInputButton = new ElaPushButton("打开多行文本对话框", this);
    multiLineInputButton->setFixedHeight(38);
    connect(multiLineInputButton, &ElaPushButton::clicked, this, [=]() {
        bool ok;
        QString text = ElaInputDialog::getMultiLineText(
            this,
            "输入备注",              // 标题
            "请详细描述您的需求",    // 副标题
            "详细描述:",            // 标签
            "",                     // 默认文本
            &ok,
            "提交",                 // 确定按钮文本
            "取消"                  // 取消按钮文本
        );
        if (ok && !text.isEmpty())
        {
            QString displayText = text.length() > 30 ? text.left(30) + "..." : text;
            multiLineResultText->setText(QString("您输入的内容: %1").arg(displayText));
        }
    });

    ElaScrollPageArea* multiLineInputDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* multiLineInputDialogLayout = new QHBoxLayout(multiLineInputDialogArea);
    ElaText* multiLineInputDialogText = new ElaText("ElaInputDialog (MultiLine)", this);
    multiLineInputDialogText->setTextPixelSize(15);
    multiLineInputDialogLayout->addWidget(multiLineInputDialogText);
    multiLineInputDialogLayout->addWidget(multiLineInputButton);
    multiLineInputDialogLayout->addWidget(multiLineResultText);
    multiLineInputDialogLayout->addStretch();

    _calendar = new ElaCalendar(this);

    _calendarPicker = new ElaCalendarPicker(this);
    ElaScrollPageArea* calendarPickerArea = new ElaScrollPageArea(this);
    QHBoxLayout* calendarPickerLayout = new QHBoxLayout(calendarPickerArea);
    ElaText* calendarPickerText = new ElaText("ElaCalendarPicker", this);
    calendarPickerText->setTextPixelSize(15);
    calendarPickerLayout->addWidget(calendarPickerText);
    calendarPickerLayout->addWidget(_calendarPicker);
    calendarPickerLayout->addStretch();

    _keyBinder = new ElaKeyBinder(this);
    ElaScrollPageArea* keyBinderArea = new ElaScrollPageArea(this);
    QHBoxLayout* keyBinderLayout = new QHBoxLayout(keyBinderArea);
    ElaText* keyBinderText = new ElaText("ElaKeyBinder", this);
    keyBinderText->setTextPixelSize(15);
    keyBinderLayout->addWidget(keyBinderText);
    keyBinderLayout->addWidget(_keyBinder);
    keyBinderLayout->addStretch();

    _roller = new ElaRoller(this);
    QStringList rollerItemList;
    for (int i = 0; i < 100; i++)
    {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);
    ElaScrollPageArea* rollerArea = new ElaScrollPageArea(this);
    rollerArea->setFixedHeight(220);
    QHBoxLayout* rollerLayout = new QHBoxLayout(rollerArea);
    ElaText* rollerText = new ElaText("ElaRoller", this);
    rollerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerText);
    rollerLayout->addWidget(_roller);
    rollerLayout->addSpacing(30);

    ElaText* rollerPickerText = new ElaText("ElaRollerPicker", this);
    rollerPickerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerPickerText);

    QTime currentTime = QTime::currentTime();
    QString currentHour = QString("%1").arg(currentTime.hour(), 2, 10, QChar('0'));
    QString currentMinute = QString("%1").arg(currentTime.minute(), 2, 10, QChar('0'));
    _timeRollerPicker = new ElaRollerPicker(this);
    QStringList hourItemList;
    for (int i = 0; i < 24; i++)
    {
        hourItemList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    QStringList minuteList;
    for (int i = 0; i < 61; i++)
    {
        minuteList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    _timeRollerPicker->addRoller(hourItemList);
    _timeRollerPicker->addRoller(minuteList);
    _timeRollerPicker->addRoller({"AM", "PM"}, false);
    _timeRollerPicker->setCurrentData({currentHour,
                                       currentMinute,
                                       currentTime.hour() >= 12 ? "PM" : "AM"});

    _clockRollerPicker = new ElaRollerPicker(this);
    _clockRollerPicker->addRoller(hourItemList);
    _clockRollerPicker->addRoller(minuteList);
    _clockRollerPicker->setRollerWidth(0, 135);
    _clockRollerPicker->setRollerWidth(1, 135);
    _clockRollerPicker->setCurrentData({currentHour,
                                        currentMinute});

    QVBoxLayout* rollerPickerLayout = new QVBoxLayout();
    rollerPickerLayout->addWidget(_timeRollerPicker);
    rollerPickerLayout->addWidget(_clockRollerPicker);
    rollerLayout->addLayout(rollerPickerLayout);
    rollerLayout->addStretch();

    _drawer = new ElaDrawerArea(this);
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    ElaText* drawerIcon = new ElaText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setElaIcon(ElaIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);
    ElaText* drawerText = new ElaText("ElaDrawer", this);
    drawerText->setTextPixelSize(15);

    ElaToggleSwitch* drawerSwitch = new ElaToggleSwitch(this);
    ElaText* drawerSwitchText = new ElaText("关", this);
    drawerSwitchText->setTextPixelSize(15);
    connect(drawerSwitch, &ElaToggleSwitch::toggled, this, [=](bool toggled) {
        if (toggled)
        {
            drawerSwitchText->setText("开");
            _drawer->expand();
        }
        else
        {
            drawerSwitchText->setText("关");
            _drawer->collapse();
        }
    });
    connect(_drawer, &ElaDrawerArea::expandStateChanged, this, [=](bool isExpand) {
        drawerSwitch->setIsToggled(isExpand);
    });

    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);

    _drawer->setDrawerHeader(drawerHeader);
    QWidget* drawerWidget1 = new QWidget(this);
    drawerWidget1->setFixedHeight(75);
    QHBoxLayout* drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
    ElaCheckBox* drawerCheckBox1 = new ElaCheckBox("测试窗口1", this);
    drawerWidget1Layout->addSpacing(60);
    drawerWidget1Layout->addWidget(drawerCheckBox1);

    QWidget* drawerWidget2 = new QWidget(this);
    drawerWidget2->setFixedHeight(75);
    QHBoxLayout* drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
    ElaCheckBox* drawerCheckBox2 = new ElaCheckBox("测试窗口2", this);
    drawerWidget2Layout->addSpacing(60);
    drawerWidget2Layout->addWidget(drawerCheckBox2);

    QWidget* drawerWidget3 = new QWidget(this);
    drawerWidget3->setFixedHeight(75);
    QHBoxLayout* drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
    ElaCheckBox* drawerCheckBox3 = new ElaCheckBox("测试窗口3", this);
    drawerWidget3Layout->addSpacing(60);
    drawerWidget3Layout->addWidget(drawerCheckBox3);

    _drawer->addDrawer(drawerWidget1);
    _drawer->addDrawer(drawerWidget2);
    _drawer->addDrawer(drawerWidget3);

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(inputDialogArea);
    centerVLayout->addWidget(intInputDialogArea);
    centerVLayout->addWidget(doubleInputDialogArea);
    centerVLayout->addWidget(multiLineInputDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(_drawer);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
