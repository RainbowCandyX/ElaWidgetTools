---
name: elawidgettools
description: 用于 Qt/C++ 项目中使用 ElaWidgetTools（FluentUI 风格 Qt Widget 组件库，121 个组件）开发 UI 的 skill。当且仅当当前项目已接入 ElaWidgetTools（判定条件：CMakeLists 出现 ElaWidgetTools target/链接，或源码中存在 #include "Ela*.h"，或目录中存在 ElaWidgetTools/ 子模块）时使用。覆盖窗口、按钮、输入、对话框、消息提示、列表、表格、卡片、导航、菜单、进度、日历、聊天气泡、终端、代码编辑器、二维码、统计图表、Markdown 渲染、PySide6 绑定等所有 UI 场景。当用户要求"加一个按钮 / 弹窗 / 主窗口 / 侧边栏 / 输入框 / 列表 / 表格 / 通知 / 进度条 / 卡片 / 菜单 / 标签 / 主题 / 暗色模式"等 UI 任务时，必须使用此 skill 召回正确的 Ela* 类名而非使用原生 QWidget 替代。包含组件分类索引、选型决策树、典型构造 pattern、CMakeLists 接入样板、Qt 版本兼容性提示，详细 API 在 references/api-reference.md。
---

# ElaWidgetTools

基于 Qt Widget 的 FluentUI 风格组件库，121 个公开组件，命名空间统一为 `Ela*`。本 skill 的核心职责是：当用户在已接入 ElaWidgetTools 的项目里做 UI 时，给出**正确的类名 + 头文件 + 最小可用构造代码**，并在多组件可选时给出选型推荐。

## 触发判定

只在以下任一条件成立时启用本 skill 的建议（避免在裸 Qt 项目里误推 `Ela*` 类）：

1. 项目根/子模块的 CMakeLists.txt 中包含 ElaWidgetTools 目标或 target_link_libraries 链接 ElaWidgetTools
2. 项目源码中已有 `#include "Ela*.h"` 引用
3. 项目目录中存在 `ElaWidgetTools/` 子目录或 git submodule

**反向规则**：在不满足上述条件的纯 Qt 项目里，不要主动建议替换为 `Ela*` 组件——这是越权推荐。可以提一句"该项目可考虑接入 ElaWidgetTools"，但不要直接改代码。

## 启动样板（main.cpp）

接入项目时必须调用 `eApp->init()`，否则主题/Mica/暗色模式都不会生效。

```cpp
#include <QApplication>
#include "ElaApplication.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    eApp->init();              // 必须，初始化主题/显示模式
    MainWindow w;
    w.show();
    return a.exec();
}
```

`eApp` 是 `ElaApplication` 的全局单例宏。继承自 `ElaWindow` 的主窗口可获得无边框 + 侧边导航 + 面包屑路由 + 主题跟随能力。

## CMakeLists 接入

```cmake
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
add_subdirectory(ElaWidgetTools)
target_link_libraries(${PROJECT_NAME} PRIVATE ElaWidgetTools Qt${QT_VERSION_MAJOR}::Widgets)
```

`CMAKE_AUTOMOC` 必须开启（库内大量 `Q_OBJECT`）。Qt 5.15.2 / 6.6.2 / 6.6.3 是推荐版本，**Windows + Qt 6.11.0** 有 Popup 阴影 bug（macOS 不受影响），代码已用 `Q_OS_WIN && QT_VERSION_CHECK(6, 11, 0)` 条件编译适配，但仍建议避开该版本。

## 组件分类索引（121 个）

每行格式：`类名 — 一句话说明`。所有组件头文件即同名 `.h`（如 `ElaToast` → `ElaToast.h`）。

### 基础设施（10）
- `ElaApplication` — 程序初始化（主题/Mica/Acrylic）；用 `eApp->init()`
- `ElaTheme` — 主题管理器，Light/Dark 切换
- `ElaWindow` — 带导航栏的无边框主窗口（侧边导航+堆栈页面+面包屑+路由）
- `ElaWidget` — 无边框模态窗口
- `ElaAppBar` — 窗口顶部标题栏（拖动/最小化/最大化/关闭）
- `ElaNavigationRouter` — 前进/后退历史
- `ElaRouter` — 声明式路由器，类 Vue Router（路由表/守卫/动态/懒加载/嵌套）
- `ElaEventBus` — 事件总线，跨组件解耦
- `ElaIcon` — 3500+ FluentUI 图标
- `ElaLog` — 分级日志

### 按钮与输入（28）
- `ElaPushButton` — 标准按钮，支持图标/自定义颜色
- `ElaIconButton` — 纯图标按钮
- `ElaToolButton` — 带下拉菜单的工具按钮
- `ElaSplitButton` — 分裂按钮（左主操作+右下拉，独立 Hover/Press）
- `ElaToggleSwitch` — 滑动开关
- `ElaToggleButton` — 切换按钮（选中/未选中）
- `ElaCheckBox` — 三态勾选框
- `ElaRadioButton` — 单选按钮
- `ElaComboBox` — 下拉框
- `ElaMultiSelectComboBox` — 多选下拉框（标签展示）
- `ElaLineEdit` — 单行输入框
- `ElaPasswordBox` — 密码框（眼睛切换显示）
- `ElaPlainTextEdit` — 多行文本编辑
- `ElaSpinBox` — 整数微调框
- `ElaDoubleSpinBox` — 浮点微调框
- `ElaNumberBox` — 增强数字输入（加减/滚轮/键盘/双击编辑/范围限制）
- `ElaSlider` — 拖动条
- `ElaKeyBinder` — 单按键绑定器（macOS Fn 键支持）
- `ElaCaptcha` — 分格验证码输入
- `ElaRatingControl` — 星级评分
- `ElaSuggestBox` — 输入联想搜索框
- `ElaDropDownButton` — 整体下拉按钮
- `ElaSelectorBar` — 分段选择器（滑动指示条）
- `ElaTransfer` — 穿梭框（双列表互选）
- `ElaAutoComplete` — 自动补全（包含/前缀/后缀/正则匹配）
- `ElaTreeSelect` — 树形选择下拉框
- `ElaUploadArea` — 文件上传区域（拖拽+验证）
- `ElaCopyButton` — 一键复制按钮（图标切换反馈）

### 数据展示（14）
- `ElaText` — 文本（Caption/Body/Title/Display 预设）
- `ElaInfoBadge` — 徽章（Dot/数值/图标，5 种严重等级色）
- `ElaTag` — 标签/胶囊（5 色、可关闭、可选中）
- `ElaPersonPicture` — 头像（图片/首字母/默认图标）
- `ElaStatCard` — 统计卡片（大数字+趋势箭头）
- `ElaProgressBar` — 进度条（进度/忙碌）
- `ElaProgressRing` — 进度环
- `ElaSteps` — 步骤指示器
- `ElaTimeline` — 时间线
- `ElaSkeleton` — 骨架屏（Text/Circle/Rectangle + Shimmer）
- `ElaDivider` — 分隔线（水平/垂直，可带文字）
- `ElaLCDNumber` — 仿液晶屏数字
- `ElaCountdown` — 倒计时（翻牌显示）
- `ElaQRCode` — 二维码生成器

### 卡片（6）
- `ElaAcrylicUrlCard` — 带图片的亚克力交互卡片（URL 跳转）
- `ElaImageCard` — 圆角图片卡片
- `ElaInteractiveCard` — 交互式透明卡片（hover 透明度变化）
- `ElaPopularCard` — 热门推荐卡片
- `ElaPromotionCard` — 促销卡片
- `ElaReminderCard` — 带图片的提醒卡片

### 导航与布局（14）
- `ElaNavigationBar` — 导航栏（展开/折叠/紧凑）
- `ElaBreadcrumbBar` — 面包屑
- `ElaPivot` — Tab 式轴转导航
- `ElaPagination` — 分页（自动省略号）
- `ElaTabBar` — 选项卡（谷歌浏览器风格，可拖拽）
- `ElaTabWidget` — 选项卡页面容器
- `ElaScrollPage` — 自带堆栈+面包屑的滚动页面
- `ElaScrollPageArea` — 圆角背景的滚动区域容器（**最常用的内容包装器**）
- `ElaScrollArea` — 通用滚动区域
- `ElaScrollBar` — 自动隐藏滚动条
- `ElaFlowLayout` — 流式布局（带动画）
- `ElaExpander` — 折叠展开面板
- `ElaGroupBox` — 带标题的分组框
- `ElaSplitter` — 可拖拽分割面板

### 弹出与交互（27）
- `ElaMenu` — 菜单（图标+快捷键+子菜单）
- `ElaMenuBar` — 顶部菜单栏
- `ElaToolBar` — 可停靠工具栏
- `ElaCommandBar` — 命令栏（带溢出菜单，独立 Hover/Press）
- `ElaStatusBar` — 状态栏
- `ElaContentDialog` — **带遮罩**的对话框（全窗口遮罩+居中）
- `ElaDialog` — 标准无边框对话框
- `ElaInputDialog` — 输入对话框
- `ElaMessageDialog` — 确认/取消消息对话框
- `ElaMessageBar` — 弹出式信息栏（**八方向锚定**）
- `ElaMessageButton` — 弹出信息按钮
- `ElaToast` — **轻量自动消失**提示（Success/Info/Warning/Error）
- `ElaSnackbar` — 底部通知条（带操作按钮，自动堆叠）
- `ElaFloatButton` — 圆形悬浮操作按钮（FAB）
- `ElaEmojiPicker` — 表情选择器（Telegram 风格）
- `ElaFlyout` — 锚定目标的轻量弹出面板（Light Dismiss）
- `ElaTeachingTip` — 引导提示气泡（带箭头指向）
- `ElaToolTip` — 悬停工具提示
- `ElaColorDialog` — HSV/RGB 颜色选择器
- `ElaCalendar` — 日历视图
- `ElaCalendarPicker` — 日期选择器
- `ElaSpotlight` — 聚光灯引导（遮罩挖洞高亮）
- `ElaPopconfirm` — 气泡确认框（锚定+Light Dismiss）
- `ElaRoller` — 单列滚轮选择器
- `ElaRollerPicker` — 多列滚轮组合（时间/日期）
- `ElaDrawerArea` — 抽屉区域
- `ElaInfoBar` — **内嵌式持久**信息栏（可加操作按钮）

### 窗口与面板（6）
- `ElaWizard` — 向导窗口（多步骤+步骤指示器）
- `ElaNotificationCenter` — 通知中心（右侧滑出）
- `ElaDockWidget` — 可拖拽停靠浮动面板
- `ElaSplashScreen` — 启动屏（进度条/进度环/Logo）
- `ElaSheetPanel` — 底部滑出面板（三级停靠 Peek/Half/Full）
- `ElaWatermark` — 全局水印覆盖（鼠标穿透）

### 视图（9）
- `ElaListView` — 列表视图（自定义委托）
- `ElaTableView` — 表格视图
- `ElaTableWidget` — 高级表格（悬停行高亮）
- `ElaTreeView` — 树型视图
- `ElaGraphicsScene` / `ElaGraphicsView` / `ElaGraphicsItem` / `ElaGraphicsLineItem` — 高级图形场景
- `ElaPromotionView` — 促销卡片轮播
- `ElaVirtualList` — 虚拟滚动列表（万级数据）

### 编辑器/渲染/聊天/终端/系统（8）
- `ElaCodeEditor` — 代码编辑器（行号+8 语言高亮：C/C++/C#/Python/JS/Lua/Rust/PHP）
- `ElaMarkdownViewer` — Markdown 渲染（基于 QTextBrowser）
- `ElaChatBubble` — 聊天气泡（左右对齐+头像+图片预览）
- `ElaDashboardGauge` — 仪表盘（动画指针+分区变色）
- `ElaTerminalWidget` — 终端模拟器（命令历史+Tab 补全信号）
- `ElaDxgiManager` — Windows DXGI 屏幕采集
- `ElaScreenCaptureManager` — macOS ScreenCaptureKit 采集
- `ElaExponentialBlur` — 高性能指数模糊

## 选型决策树

用户描述需求时优先匹配下表，多选时按"备注"挑选：

### 消息提示（最容易选错）

| 场景 | 推荐 | 备注 |
|---|---|---|
| 操作成功/失败的瞬时反馈，几秒后自动消失 | `ElaToast` | 4 种类型 Success/Info/Warning/Error |
| 在窗口某方向锚定弹出（如顶部居中、右下） | `ElaMessageBar` | 八方向锚定 |
| 持久内嵌（不消失，常驻表单上方） | `ElaInfoBar` | 可加操作按钮 |
| 底部带"撤销"等操作按钮，可堆叠 | `ElaSnackbar` | 自动重排 |
| 引导用户的气泡 + 箭头指向 | `ElaTeachingTip` | 4 方向自动定位 |
| 锚定按钮的"确认删除？"小气泡 | `ElaPopconfirm` | 带图标+确认/取消 |

### 对话框

| 场景 | 推荐 |
|---|---|
| 全窗口遮罩 + 居中（推荐用于关闭确认、登录） | `ElaContentDialog` |
| 标准无边框对话框（自定义内容） | `ElaDialog` |
| 输入一行文本拿到结果 | `ElaInputDialog` |
| 确认/取消（YES/NO） | `ElaMessageDialog` |
| 多步骤向导 | `ElaWizard` |
| 颜色选择 | `ElaColorDialog` |

### 主窗口

| 场景 | 推荐 |
|---|---|
| 带侧边导航 + 堆栈页面 + 面包屑路由的应用主窗口 | 继承 `ElaWindow` |
| 仅需无边框 + 标题栏拖动 | 继承 `ElaWidget`（模态）或自己装 `ElaAppBar` |

### 文本输入

| 场景 | 推荐 |
|---|---|
| 单行 | `ElaLineEdit` |
| 密码（带眼睛切换） | `ElaPasswordBox` |
| 多行 | `ElaPlainTextEdit` |
| 数字（整数） | `ElaSpinBox` 或 `ElaNumberBox` |
| 数字（浮点） | `ElaDoubleSpinBox` |
| 输入联想 | `ElaSuggestBox` 或 `ElaAutoComplete` |
| 验证码（分格） | `ElaCaptcha` |

### 选择器

| 场景 | 推荐 |
|---|---|
| 单选下拉 | `ElaComboBox` |
| 多选下拉（标签） | `ElaMultiSelectComboBox` |
| 树形选择 | `ElaTreeSelect` |
| 双列表互选 | `ElaTransfer` |
| 分段（Tab 风） | `ElaSelectorBar` |
| 日期 | `ElaCalendarPicker` |
| 滚轮（时间/日期复合） | `ElaRollerPicker` |
| 颜色 | `ElaColorDialog` |
| 表情 | `ElaEmojiPicker` |

### 列表/表格

| 数据量 | 类型 | 推荐 |
|---|---|---|
| < 1 万行 | 列表 | `ElaListView` |
| > 1 万行 | 列表 | `ElaVirtualList` |
| 表格（M-V 模式） | 表格 | `ElaTableView` |
| 表格（直接填值） | 表格 | `ElaTableWidget` |
| 树 | 树 | `ElaTreeView` |

### 卡片

需求里出现"卡片"时，先确认是否含图片+点击跳转：含 URL → `ElaAcrylicUrlCard`；纯图片 → `ElaImageCard`；hover 透明 → `ElaInteractiveCard`；推荐内容 → `ElaPopularCard`；促销 → `ElaPromotionCard`；提醒 → `ElaReminderCard`；统计数字+趋势 → `ElaStatCard`。

### 进度

| 形态 | 推荐 |
|---|---|
| 条形 | `ElaProgressBar` |
| 环形 | `ElaProgressRing` |
| 多步骤 | `ElaSteps` |
| 时间线 | `ElaTimeline` |
| 骨架屏（加载占位） | `ElaSkeleton` |
| 倒计时 | `ElaCountdown` |
| 仪表盘 | `ElaDashboardGauge` |

## 典型构造 pattern

ElaWidgetTools 的内容页约定俗成的写法：所有逻辑组件被装进 `ElaScrollPageArea`（圆角背景容器），里面用 `QHBoxLayout` / `QVBoxLayout` 组合 `ElaText` 标签 + 实际控件 + `addStretch()`：

```cpp
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include <QHBoxLayout>

auto* area = new ElaScrollPageArea(this);
auto* layout = new QHBoxLayout(area);
auto* label = new ElaText("启用通知", this);
label->setTextPixelSize(15);
auto* sw = new ElaToggleSwitch(this);
layout->addWidget(label);
layout->addStretch();
layout->addWidget(sw);
```

主窗口继承 `ElaWindow` 时的最小骨架：

```cpp
class MainWindow : public ElaWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    void initWindow();      // setWindowTitle / resize / setIsCentralStackedWidgetTransparent 等
    void initEdgeLayout();  // 装配 AppBar 周边（状态栏、悬浮按钮）
    void initContent();     // addPageNode / addExpanderNode 注册侧边导航页
};
```

侧边导航注册页面（基于 `ElaWindow` 内置 `addPageNode` API）：

```cpp
QString homeKey;
addPageNode("HOME", _homePage, homeKey, ElaIconType::House);
QString settingKey;
addFooterNode("Setting", _settingPage, settingKey, 0, ElaIconType::GearComplex);
```

## 主题切换

`eTheme` 是 `ElaTheme` 全局单例宏。所有 `Ela*` 组件自动响应主题变更，无需手动重绘。

**ThemeMode 枚举只有两个值**：`ElaThemeType::Light` / `ElaThemeType::Dark`。**没有 `FollowSystem` 这个枚举值**——跟随系统是一个独立的正交开关。

### 三种用法

**(1) 手动切换**（应用自管，不理系统）：

```cpp
#include "ElaTheme.h"
eTheme->setThemeMode(ElaThemeType::Dark);   // 或 Light
```

**(2) 跟随系统**（推荐，默认建议开启）：

```cpp
eTheme->setIsFollowSystemTheme(true);
```

打开后由库内部监听系统颜色方案变化，自动调用 `setThemeMode` 切换到对应模式：
- Qt 6.5+：通过 `QStyleHints::colorSchemeChanged` 信号响应
- Qt < 6.5：通过 `installEventFilter` 监听 `QEvent::ApplicationPaletteChange`

打开瞬间会立即应用一次当前系统主题。再次调用 `setIsFollowSystemTheme(false)` 关闭跟随，之后保持当前模式直到下次手动 `setThemeMode`。

**(3) 用户在设置页给"跟随系统"提供单独开关**（典型应用场景）：

把"跟随系统"和"Light/Dark 二选一"做成两组联动控件——开了跟随就禁用 Light/Dark 选择器；关了跟随才允许手选。

```cpp
#include "ElaToggleSwitch.h"
auto* followSwitch = new ElaToggleSwitch(this);
followSwitch->setIsToggled(eTheme->getIsFollowSystemTheme());
connect(followSwitch, &ElaToggleSwitch::toggled, this, [](bool on) {
    eTheme->setIsFollowSystemTheme(on);
});

// 监听跟随开关变化（如同步禁用/启用 Light/Dark 选择器）
connect(eTheme, &ElaTheme::pIsFollowSystemThemeChanged, this, [this](bool isFollow) {
    _modeSelector->setEnabled(!isFollow);
});

// 监听最终生效的主题（系统切换或手动切换都会发）
connect(eTheme, &ElaTheme::themeModeChanged, this, [this](ElaThemeType::ThemeMode mode) {
    // 自定义重绘逻辑（一般不需要）
});
```

### 自定义主题色

```cpp
eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryNormal, QColor("#3498db"));
```

色键见 [ElaDef.h](../ElaWidgetTools/ElaDef.h) 的 `ElaThemeType::ThemeColor` 枚举（含 `ScrollBarHandle` / `ToggleSwitchNoToggledCenter` / `PrimaryNormal` 等数十种）。Light 和 Dark 各自一套独立色板，两边都要 set 才能两个模式都生效。

### 信号速查

| 信号 | 触发时机 |
|---|---|
| `themeModeChanged(ThemeMode)` | 模式实际生效时（手动切或跟随系统切） |
| `pIsFollowSystemThemeChanged(bool)` | "跟随系统"开关被切换时 |

## PySide6 绑定

项目带 Shiboken6 绑定基础设施。Python 侧用法与 C++ 完全对齐（类名相同），见 `PySide6Example/` 与 `bindings/`：

```bash
pip install PySide6 shiboken6 shiboken6-generator
python3 scripts/generate_bindings.py
cmake -B build -DELAWIDGETTOOLS_BUILD_STATIC_LIB=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build --target ElaWidgetTools
```

## 完整 API 查询

写代码前若需要确认某个组件的具体方法签名、属性、信号，**用 grep 定位 `references/api-reference.md`**，例如查 `ElaToast` 的 30 行片段：

```
grep -nA 30 "^## ElaToast" references/api-reference.md
```

api-reference.md 共 3031 行，121 个组件按字母序排列，每个组件给出：继承类、头文件、属性表、方法列表、信号列表。**不要把整个文件读进上下文**，只 grep 需要的段落。

## 注意事项

1. **`eApp->init()` 必须调用**——否则主题/Mica/暗色模式失效
2. **`CMAKE_AUTOMOC` 必须开启**——大量 `Q_OBJECT`
3. **C++17 起步**（`CMAKE_CXX_STANDARD 17`）
4. **Windows + Qt 6.11.0** Popup 阴影 bug，已条件编译适配但建议避开
5. **Debug/MinSizeRel 构建被禁止**（CMakeLists 主动 fatal），只允许 Release / RelWithDebInfo
6. 修改/新增 Qt/C++ 组件后必须运行 `cmake --build build` 验证编译，遵循项目 CLAUDE.md 中的"C++/Qt 开发"规范
