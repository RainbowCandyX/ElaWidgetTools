# ElaWidgetTools API 文档

> 本文档由 `scripts/generate_docs.py` 自动生成，请勿手动编辑。
>
> 共 **106** 个公开组件

## 目录

- [ElaApplication](#elaapplication)
- [ElaTheme](#elatheme)
- [ElaWindow](#elawindow)
- [ElaWidget](#elawidget)
- [ElaAppBar](#elaappbar)
- [ElaNavigationBar](#elanavigationbar)
- [ElaNavigationRouter](#elanavigationrouter)
- [ElaAcrylicUrlCard](#elaacrylicurlcard)
- [ElaBreadcrumbBar](#elabreadcrumbbar)
- [ElaCalendar](#elacalendar)
- [ElaCalendarPicker](#elacalendarpicker)
- [ElaCaptcha](#elacaptcha)
- [ElaCheckBox](#elacheckbox)
- [ElaCodeEditor](#elacodeeditor)
- [ElaColorDialog](#elacolordialog)
- [ElaComboBox](#elacombobox)
- [ElaCommandBar](#elacommandbar)
- [ElaContentDialog](#elacontentdialog)
- [ElaCountdown](#elacountdown)
- [ElaDialog](#eladialog)
- [ElaDivider](#eladivider)
- [ElaDockWidget](#eladockwidget)
- [ElaDoubleSpinBox](#eladoublespinbox)
- [ElaDrawerArea](#eladrawerarea)
- [ElaDropDownButton](#eladropdownbutton)
- [ElaDxgiManager](#eladxgimanager)
- [ElaEvent](#elaevent)
- [ElaExpander](#elaexpander)
- [ElaFlowLayout](#elaflowlayout)
- [ElaFlyout](#elaflyout)
- [ElaGraphicsItem](#elagraphicsitem)
- [ElaGraphicsLineItem](#elagraphicslineitem)
- [ElaGraphicsScene](#elagraphicsscene)
- [ElaGraphicsView](#elagraphicsview)
- [ElaGroupBox](#elagroupbox)
- [ElaIconButton](#elaiconbutton)
- [ElaImageCard](#elaimagecard)
- [ElaInfoBadge](#elainfobadge)
- [ElaInputDialog](#elainputdialog)
- [ElaInteractiveCard](#elainteractivecard)
- [ElaKeyBinder](#elakeybinder)
- [ElaLCDNumber](#elalcdnumber)
- [ElaLineEdit](#elalineedit)
- [ElaListView](#elalistview)
- [ElaLog](#elalog)
- [ElaMarkdownViewer](#elamarkdownviewer)
- [ElaMenu](#elamenu)
- [ElaMenuBar](#elamenubar)
- [ElaMessageBar](#elamessagebar)
- [ElaMessageButton](#elamessagebutton)
- [ElaMessageDialog](#elamessagedialog)
- [ElaMultiSelectComboBox](#elamultiselectcombobox)
- [ElaNotificationCenter](#elanotificationcenter)
- [ElaNumberBox](#elanumberbox)
- [ElaPagination](#elapagination)
- [ElaPasswordBox](#elapasswordbox)
- [ElaPersonPicture](#elapersonpicture)
- [ElaPivot](#elapivot)
- [ElaPlainTextEdit](#elaplaintextedit)
- [ElaPopconfirm](#elapopconfirm)
- [ElaPopularCard](#elapopularcard)
- [ElaProgressBar](#elaprogressbar)
- [ElaProgressRing](#elaprogressring)
- [ElaPromotionCard](#elapromotioncard)
- [ElaPromotionView](#elapromotionview)
- [ElaPushButton](#elapushbutton)
- [ElaRadioButton](#elaradiobutton)
- [ElaRatingControl](#elaratingcontrol)
- [ElaReminderCard](#elaremindercard)
- [ElaRoller](#elaroller)
- [ElaRollerPicker](#elarollerpicker)
- [ElaScreenCaptureManager](#elascreencapturemanager)
- [ElaScrollArea](#elascrollarea)
- [ElaScrollBar](#elascrollbar)
- [ElaScrollPage](#elascrollpage)
- [ElaScrollPageArea](#elascrollpagearea)
- [ElaSelectorBar](#elaselectorbar)
- [ElaSheetPanel](#elasheetpanel)
- [ElaSkeleton](#elaskeleton)
- [ElaSlider](#elaslider)
- [ElaSpinBox](#elaspinbox)
- [ElaSplashScreen](#elasplashscreen)
- [ElaSplitButton](#elasplitbutton)
- [ElaSpotlight](#elaspotlight)
- [ElaStatCard](#elastatcard)
- [ElaStatusBar](#elastatusbar)
- [ElaSteps](#elasteps)
- [ElaSuggestBox](#elasuggestbox)
- [ElaTabBar](#elatabbar)
- [ElaTabWidget](#elatabwidget)
- [ElaTableView](#elatableview)
- [ElaTableWidget](#elatablewidget)
- [ElaTag](#elatag)
- [ElaTeachingTip](#elateachingtip)
- [ElaText](#elatext)
- [ElaTimeline](#elatimeline)
- [ElaToast](#elatoast)
- [ElaToggleButton](#elatogglebutton)
- [ElaToggleSwitch](#elatoggleswitch)
- [ElaToolBar](#elatoolbar)
- [ElaToolButton](#elatoolbutton)
- [ElaToolTip](#elatooltip)
- [ElaTransfer](#elatransfer)
- [ElaTreeView](#elatreeview)
- [ElaVirtualList](#elavirtuallist)
- [ElaWizard](#elawizard)

---

## ElaApplication

**继承**: `QObject` | **头文件**: `ElaApplication.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `ElaApplicationType::WindowDisplayMode` | `WindowDisplayMode` | get/set |
| `QString` | `ElaMicaImagePath` | get/set |

### 方法

- `void init()`
- `void syncWindowDisplayMode(QWidget* widget, bool isSync = true)`
- `static bool containsCursorToItem(QWidget* item)`

---

## ElaTheme

**继承**: `QObject` | **头文件**: `ElaTheme.h`

### 方法

- `void setThemeMode(ElaThemeType::ThemeMode themeMode)`
- `ElaThemeType::ThemeMode getThemeMode()`
- `void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)`
- `void setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, QColor newColor)`
- `const QColor& getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor)`

### 信号

- `themeModeChanged(ElaThemeType::ThemeMode themeMode)`

---

## ElaWindow

**继承**: `QMainWindow` | **头文件**: `ElaWindow.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsStayTop` | get/set |
| `bool` | `IsFixedSize` | get/set |
| `bool` | `IsDefaultClosed` | get/set |
| `int` | `AppBarHeight` | get/set |
| `int` | `ThemeChangeTime` | get/set |
| `bool` | `IsCentralStackedWidgetTransparent` | get/set |
| `bool` | `IsAllowPageOpenInNewWindow` | get/set |
| `bool` | `IsNavigationBarEnable` | get/set |
| `int` | `NavigationBarWidth` | get/set |
| `int` | `CurrentStackIndex` | get/set |
| `ElaNavigationType::NavigationDisplayMode` | `NavigationBarDisplayMode` | get/set |
| `ElaWindowType::StackSwitchMode` | `StackSwitchMode` | get/set |
| `ElaWindowType::PaintMode` | `WindowPaintMode` | get/set |

### 方法

- `void moveToCenter()`
- `void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget, QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "")`
- `QWidget* getCustomWidget(ElaAppBarType::CustomArea customArea)`
- `void setCentralCustomWidget(QWidget* customWidget)`
- `QWidget* getCentralCustomWidget()`
- `void setCustomMenu(QMenu* customMenu)`
- `QMenu* getCustomMenu()`
- `void setUserInfoCardVisible(bool isVisible)`
- `void setUserInfoCardPixmap(QPixmap pix)`
- `void setUserInfoCardTitle(QString title)`
- `void setUserInfoCardSubTitle(QString subTitle)`
- `ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey)`
- `ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)`
- `void addCentralWidget(QWidget* centralWidget)`
- `QWidget* getCentralWidget(int index)`
- `bool getNavigationNodeIsExpanded(QString expanderKey)`
- `void expandNavigationNode(QString expanderKey)`
- `void collapseNavigationNode(QString expanderKey)`
- `void removeNavigationNode(QString nodeKey)`
- `int getPageOpenInNewWindowCount(QString nodeKey)`
- `void backtrackNavigationNode(QString nodeKey)`
- `void setNodeKeyPoints(QString nodeKey, int keyPoints)`
- `int getNodeKeyPoints(QString nodeKey)`
- `void setNavigationNodeTitle(QString nodeKey, QString nodeTitle)`
- `QString getNavigationNodeTitle(QString nodeKey)`
- `void navigation(QString pageKey)`
- `int getCurrentNavigationIndex()`
- `QString getCurrentNavigationPageKey()`
- `void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)`
- `ElaAppBarType::ButtonFlags getWindowButtonFlags()`
- `void setWindowMoviePath(ElaThemeType::ThemeMode themeMode, const QString& moviePath)`
- `QString getWindowMoviePath(ElaThemeType::ThemeMode themeMode)`
- `void setWindowPixmap(ElaThemeType::ThemeMode themeMode, const QPixmap& pixmap)`
- `QPixmap getWindowPixmap(ElaThemeType::ThemeMode themeMode)`
- `void setWindowMovieRate(qreal rate)`
- `qreal getWindowMovieRate()`
- `void closeWindow()`

### 信号

- `userInfoCardClicked()`
- `closeButtonClicked()`
- `navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)`
- `customWidgetChanged()`
- `centralCustomWidgetChanged()`
- `customMenuChanged()`
- `pageOpenInNewWindow(QString nodeKey)`

---

## ElaWidget

**继承**: `QWidget` | **头文件**: `ElaWidget.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsStayTop` | get/set |
| `bool` | `IsFixedSize` | get/set |
| `bool` | `IsDefaultClosed` | get/set |
| `int` | `AppBarHeight` | get/set |

### 方法

- `void moveToCenter()`
- `void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)`
- `ElaAppBarType::ButtonFlags getWindowButtonFlags()`

### 信号

- `routeBackButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`

---

## ElaAppBar

**继承**: `QWidget` | **头文件**: `ElaAppBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsStayTop` | get/set |
| `bool` | `IsFixedSize` | get/set |
| `bool` | `IsDefaultClosed` | get/set |
| `bool` | `IsOnlyAllowMinAndClose` | get/set |
| `int` | `AppBarHeight` | get/set |

### 方法

- `void setCustomWidget(ElaAppBarType::CustomArea customArea, QWidget* customWidget, QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "")`
- `QWidget* getCustomWidget(ElaAppBarType::CustomArea customArea)`
- `void setCustomMenu(QMenu* customMenu)`
- `QMenu* getCustomMenu()`
- `void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)`
- `ElaAppBarType::ButtonFlags getWindowButtonFlags()`
- `void setRouteBackButtonEnable(bool isEnable)`
- `void setRouteForwardButtonEnable(bool isEnable)`
- `void closeWindow()`
- `int takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)`
- `int takeOverNativeEvent(const QByteArray& eventType, void* message, long* result)`

### 信号

- `routeBackButtonClicked()`
- `routeForwardButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`
- `customWidgetChanged()`
- `customMenuChanged()`

---

## ElaNavigationBar

**继承**: `QWidget` | **头文件**: `ElaNavigationBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsTransparent` | get/set |
| `bool` | `IsAllowPageOpenInNewWindow` | get/set |
| `int` | `NavigationBarWidth` | get/set |

### 方法

- `void setUserInfoCardVisible(bool isVisible)`
- `void setUserInfoCardPixmap(QPixmap pix)`
- `void setUserInfoCardTitle(QString title)`
- `void setUserInfoCardSubTitle(QString subTitle)`
- `ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString& expanderKey, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, ElaIconType::IconName awesome = ElaIconType::None)`
- `ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey)`
- `ElaNavigationType::NodeResult addCategoryNode(const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)`
- `bool getNodeIsExpanded(QString expanderKey)`
- `void expandNode(QString expanderKey)`
- `void collapseNode(QString expanderKey)`
- `void removeNode(QString nodeKey)`
- `void setNodeKeyPoints(QString nodeKey, int keyPoints)`
- `int getNodeKeyPoints(QString nodeKey)`
- `void setNodeTitle(QString nodeKey, QString nodeTitle)`
- `QString getNodeTitle(QString nodeKey)`
- `void navigation(QString pageKey, bool isLogClicked = true, bool isRouteBack = false)`
- `void setDisplayMode(ElaNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true)`
- `ElaNavigationType::NavigationDisplayMode getDisplayMode()`
- `int getPageOpenInNewWindowCount(QString nodeKey)`

### 信号

- `pageOpenInNewWindow(QString nodeKey)`
- `userInfoCardClicked()`
- `navigationNodeClicked(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack)`
- `navigationNodeAdded(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)`
- `navigationNodeRemoved(ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)`

---

## ElaNavigationRouter

**继承**: `QObject` | **头文件**: `ElaNavigationRouter.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `MaxRouteCount` | get/set |

### 方法

- `ElaNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection)`
- `void clearNavigationRoute()`
- `void navigationRouteBack()`
- `void navigationRouteForward()`
- `ElaNavigationRouterType::NavigationRouteType navigationRoute(QObject* context, QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection)`
- `void clearNavigationRoute(QObject* context)`
- `void navigationRouteBack(QObject* context)`
- `void navigationRouteForward(QObject* context)`

### 信号

- `navigationRouterStateChanged(ElaNavigationRouterType::RouteMode routeMode)`
- `windowRouterStateChanged(QObject* context, ElaNavigationRouterType::RouteMode routeMode)`

---

## ElaAcrylicUrlCard

**继承**: `QPushButton` | **头文件**: `ElaAcrylicUrlCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `qreal` | `MainOpacity` | get/set |
| `qreal` | `NoiseOpacity` | get/set |
| `int` | `BrushAlpha` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `int` | `TitlePixelSize` | get/set |
| `int` | `SubTitlePixelSize` | get/set |
| `int` | `TitleSpacing` | get/set |
| `int` | `SubTitleSpacing` | get/set |
| `QPixmap` | `CardPixmap` | get/set |
| `QSize` | `CardPixmapSize` | get/set |
| `int` | `CardPixmapBorderRadius` | get/set |
| `ElaCardPixType::PixMode` | `CardPixMode` | get/set |
| `QString` | `Url` | get/set |

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## ElaBreadcrumbBar

**继承**: `QWidget` | **头文件**: `ElaBreadcrumbBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `TextPixelSize` | get/set |
| `bool` | `IsAutoRemove` | get/set |

### 方法

- `void setBreadcrumbList(QStringList breadcrumbList)`
- `QStringList appendBreadcrumb(QString breadcrumb)`
- `QStringList removeBreadcrumb(QString breadcrumb)`
- `int getBreadcrumbListCount()`
- `QStringList getBreadcrumbList()`

### 信号

- `breadcrumbClicked(QString breadcrumb, QStringList lastBreadcrumbList)`

---

## ElaCalendar

**继承**: `QWidget` | **头文件**: `ElaCalendar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRaiuds` | get/set |
| `QDate` | `SelectedDate` | get/set |
| `QDate` | `MinimumDate` | get/set |
| `QDate` | `MaximumDate` | get/set |

### 信号

- `clicked(QDate date)`

---

## ElaCalendarPicker

**继承**: `QPushButton` | **头文件**: `ElaCalendarPicker.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QDate` | `SelectedDate` | get/set |

### 信号

- `selectedDateChanged(QDate date)`

---

## ElaCaptcha

**继承**: `QWidget` | **头文件**: `ElaCaptcha.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `CodeLength` | get/set |
| `int` | `BorderRadius` | get/set |
| `int` | `BoxSize` | get/set |
| `int` | `BoxSpacing` | get/set |

### 枚举

**InputMode**: `DigitOnly`, `AlphaNumeric`

### 方法

- `void setInputMode(InputMode mode)`
- `InputMode getInputMode()`
- `QString getCode()`
- `void clear()`

### 信号

- `codeCompleted(const QString& code)`
- `codeChanged(const QString& code)`

---

## ElaCheckBox

**继承**: `QCheckBox` | **头文件**: `ElaCheckBox.h`

---

## ElaCodeEditor

**继承**: `QWidget` | **头文件**: `ElaCodeEditor.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `Code` | get/set |
| `bool` | `IsReadOnly` | get/set |
| `int` | `TabSize` | get/set |

### 枚举

**Language**: `CPP`, `C`, `CSharp`, `Python`, `JavaScript`, `Lua`, `Rust`, `PHP`

### 方法

- `void setLanguage(Language lang)`
- `Language getLanguage()`

---

## ElaColorDialog

**继承**: `QDialog` | **头文件**: `ElaColorDialog.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QColor` | `CurrentColor` | get/set |

### 方法

- `QColor getCustomColor(int index)`
- `QString getCurrentColorRGB()`

### 信号

- `colorSelected(const QColor& color)`

---

## ElaComboBox

**继承**: `QComboBox` | **头文件**: `ElaComboBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

### 方法

- `void setEditable(bool editable)`

---

## ElaCommandBar

**继承**: `QWidget` | **头文件**: `ElaCommandBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `ButtonSize` | get/set |

### 结构体

**CommandItem**
```cpp
  ElaIconType::IconName icon = ElaIconType::None;
  QString text;
  bool isSeparator = false;
```

### 方法

- `void addItem(const CommandItem& item)`
- `void addSeparator()`
- `void clearItems()`

### 信号

- `itemClicked(int index)`

---

## ElaContentDialog

**继承**: `QDialog` | **头文件**: `ElaContentDialog.h`

### 方法

- `void setCentralWidget(QWidget* centralWidget)`
- `void setLeftButtonText(QString text)`
- `void setMiddleButtonText(QString text)`
- `void setRightButtonText(QString text)`
- `void setLeftButtonVisible(bool visible)`
- `void setMiddleButtonVisible(bool visible)`
- `void setRightButtonVisible(bool visible)`
- `void close()`

### 信号

- `leftButtonClicked()`
- `middleButtonClicked()`
- `rightButtonClicked()`

---

## ElaCountdown

**继承**: `QWidget` | **头文件**: `ElaCountdown.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `IsShowDays` | get/set |
| `bool` | `IsShowHours` | get/set |
| `bool` | `IsShowMinutes` | get/set |
| `bool` | `IsShowSeconds` | get/set |
| `int` | `DigitWidth` | get/set |
| `int` | `DigitHeight` | get/set |
| `int` | `DigitSpacing` | get/set |
| `int` | `FontPixelSize` | get/set |

### 方法

- `void setTargetDateTime(const QDateTime& dateTime)`
- `QDateTime getTargetDateTime()`
- `void setRemainingSeconds(qint64 seconds)`
- `qint64 getRemainingSeconds()`
- `void start()`
- `void pause()`
- `void resume()`
- `void stop()`
- `bool isRunning()`

### 信号

- `timeout()`
- `tick(qint64 remainingSeconds)`

---

## ElaDialog

**继承**: `QDialog` | **头文件**: `ElaDialog.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsStayTop` | get/set |
| `bool` | `IsFixedSize` | get/set |
| `bool` | `IsDefaultClosed` | get/set |
| `int` | `AppBarHeight` | get/set |

### 方法

- `void moveToCenter()`
- `void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true)`
- `void setWindowButtonFlags(ElaAppBarType::ButtonFlags buttonFlags)`
- `ElaAppBarType::ButtonFlags getWindowButtonFlags()`

### 信号

- `routeBackButtonClicked()`
- `navigationButtonClicked()`
- `themeChangeButtonClicked()`
- `closeButtonClicked()`

---

## ElaDivider

**继承**: `QWidget` | **头文件**: `ElaDivider.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `Qt::Orientation` | `Orientation` | get/set |
| `QString` | `Text` | get/set |
| `int` | `ContentPosition` | get/set |

### 枚举

**ContentPositionType**: `Left`, `Center`, `Right`

---

## ElaDockWidget

**继承**: `QDockWidget` | **头文件**: `ElaDockWidget.h`

---

## ElaDoubleSpinBox

**继承**: `QDoubleSpinBox` | **头文件**: `ElaDoubleSpinBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `ElaSpinBoxType::ButtonMode` | `ButtonMode` | get/set |

---

## ElaDrawerArea

**继承**: `QWidget` | **头文件**: `ElaDrawerArea.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `HeaderHeight` | get/set |

### 方法

- `void setDrawerHeader(QWidget* widget)`
- `void addDrawer(QWidget* widget)`
- `void removeDrawer(QWidget* widget)`
- `void expand()`
- `void collapse()`
- `bool getIsExpand()`

### 信号

- `expandStateChanged(bool isExpand)`

---

## ElaDropDownButton

**继承**: `QWidget` | **头文件**: `ElaDropDownButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Text` | get/set |
| `ElaIconType::IconName` | `ElaIcon` | get/set |

### 方法

- `void setMenu(ElaMenu* menu)`
- `ElaMenu* getMenu()`

---

## ElaDxgiManager

**继承**: `QObject` | **头文件**: `ElaDxgiManager.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

### 方法

- `QStringList getDxDeviceList()`
- `QStringList getOutputDeviceList()`
- `QImage grabScreenToImage()`
- `void startGrabScreen()`
- `void stopGrabScreen()`
- `bool getIsGrabScreen()`
- `bool setDxDeviceID(int dxID)`
- `int getDxDeviceID()`
- `bool setOutputDeviceID(int deviceID)`
- `int getOutputDeviceID()`
- `void setGrabArea(int width, int height)`
- `void setGrabArea(int x, int y, int width, int height)`
- `QRect getGrabArea()`
- `void setGrabFrameRate(int frameRateValue)`
- `int getGrabFrameRate()`
- `void setTimeoutMsValue(int timeoutValue)`
- `int getTimeoutMsValue()`
- `explicit ElaDxgiScreen(QWidget* parent = nullptr)`
- `void setIsSyncGrabSize(bool isSyncGrabSize)`
- `bool getIsSyncGrabSize()`

### 信号

- `grabImageUpdate(QImage img)`

---

## ElaEvent

**继承**: `QObject` | **头文件**: `ElaEventBus.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `EventName` | get/set |
| `QString` | `FunctionName` | get/set |
| `Qt::ConnectionType` | `ConnectionType` | get/set |

### 方法

- `ElaEventBusType::EventBusReturnType registerAndInit()`
- `ElaEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {})`
- `QStringList getRegisteredEventsName()`

---

## ElaExpander

**继承**: `QWidget` | **头文件**: `ElaExpander.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `ElaIconType::IconName` | `HeaderIcon` | get/set |
| `int` | `AnimationDuration` | get/set |

### 枚举

**ExpandDirection**: `Down`, `Up`

### 方法

- `void setExpandDirection(ExpandDirection direction)`
- `ExpandDirection getExpandDirection()`
- `void setContentWidget(QWidget* widget)`
- `QWidget* getContentWidget()`
- `void setHeaderWidget(QWidget* widget)`
- `void setIsExpanded(bool expanded)`
- `bool getIsExpanded()`

### 信号

- `expandStateChanged(bool expanded)`

---

## ElaFlowLayout

**继承**: `QLayout` | **头文件**: `ElaFlowLayout.h`

### 方法

- `int horizontalSpacing()`
- `int verticalSpacing()`
- `void setIsAnimation(bool isAnimation)`

---

## ElaFlyout

**继承**: `QWidget` | **头文件**: `ElaFlyout.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `Content` | get/set |
| `bool` | `IsLightDismiss` | get/set |

### 方法

- `void setContentWidget(QWidget* widget)`
- `void showFlyout(QWidget* target)`
- `void closeFlyout()`

### 信号

- `closed()`

---

## ElaGraphicsItem

**继承**: `QGraphicsObject` | **头文件**: `ElaGraphicsItem.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `Width` | get/set |
| `int` | `Height` | get/set |
| `QImage` | `ItemImage` | get/set |
| `QImage` | `ItemSelectedImage` | get/set |
| `QString` | `ItemName` | get/set |
| `QVariantMap` | `DataRoutes` | get/set |
| `int` | `MaxLinkPortCount` | get/set |

### 方法

- `QString getItemUID()`
- `void setLinkPortState(bool isFullLink)`
- `void setLinkPortState(bool isLink, int portIndex)`
- `bool getLinkPortState(int portIndex)`
- `int getUsedLinkPortCount()`
- `int getUnusedLinkPortCount()`

---

## ElaGraphicsLineItem

**继承**: `QGraphicsPathItem` | **头文件**: `ElaGraphicsLineItem.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QPointF` | `StartPoint` | get/set |
| `QPointF` | `EndPoint` | get/set |
| `ElaGraphicsItem*` | `StartItem` | get/set |
| `ElaGraphicsItem*` | `EndItem` | get/set |
| `int` | `StartItemPort` | get/set |
| `int` | `EndItemPort` | get/set |

### 方法

- `bool isTargetLink(ElaGraphicsItem* item)`
- `bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2)`
- `bool isTargetLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1, int port2)`

---

## ElaGraphicsScene

**继承**: `QGraphicsScene` | **头文件**: `ElaGraphicsScene.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsCheckLinkPort` | get/set |
| `QString` | `SerializePath` | get/set |

### 方法

- `void addItem(ElaGraphicsItem* item)`
- `void removeItem(ElaGraphicsItem* item)`
- `void removeSelectedItems()`
- `void clear()`
- `void setSceneMode(ElaGraphicsSceneType::SceneMode mode)`
- `ElaGraphicsSceneType::SceneMode getSceneMode()`
- `void selectAllItems()`
- `bool addItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1 = 0, int port2 = 0)`
- `bool removeItemLink(ElaGraphicsItem* item1)`
- `bool removeItemLink(ElaGraphicsItem* item1, ElaGraphicsItem* item2, int port1 = 0, int port2 = 0)`
- `void serialize()`
- `void deserialize()`

### 信号

- `showItemLink()`
- `mouseLeftClickedItem(ElaGraphicsItem* item)`
- `mouseRightClickedItem(ElaGraphicsItem* item)`
- `mouseDoubleClickedItem(ElaGraphicsItem* item)`

---

## ElaGraphicsView

**继承**: `QGraphicsView` | **头文件**: `ElaGraphicsView.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `qreal` | `MaxTransform` | get/set |
| `qreal` | `MinTransform` | get/set |

---

## ElaGroupBox

**继承**: `QGroupBox` | **头文件**: `ElaGroupBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

---

## ElaIconButton

**继承**: `QPushButton` | **头文件**: `ElaIconButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `qreal` | `Opacity` | get/set |
| `QColor` | `LightHoverColor` | get/set |
| `QColor` | `DarkHoverColor` | get/set |
| `QColor` | `LightIconColor` | get/set |
| `QColor` | `DarkIconColor` | get/set |
| `QColor` | `LightHoverIconColor` | get/set |
| `QColor` | `DarkHoverIconColor` | get/set |
| `bool` | `IsSelected` | get/set |

### 方法

- `void setAwesome(ElaIconType::IconName awesome)`
- `ElaIconType::IconName getAwesome()`
- `void setPixmap(QPixmap pix)`

---

## ElaImageCard

**继承**: `QWidget` | **头文件**: `ElaImageCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QImage` | `CardImage` | get/set |
| `int` | `BorderRadius` | get/set |
| `bool` | `IsPreserveAspectCrop` | get/set |

---

## ElaInfoBadge

**继承**: `QWidget` | **头文件**: `ElaInfoBadge.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `Value` | get/set |
| `ElaIconType::IconName` | `ElaIcon` | get/set |

### 枚举

**BadgeMode**: `Dot`, `Value_`, `Icon`

**Severity**: `Attention`, `Informational`, `Success`, `Caution`, `Critical`

### 方法

- `void setBadgeMode(BadgeMode mode)`
- `BadgeMode getBadgeMode()`
- `void setMaxValue(int maxValue)`
- `int getMaxValue()`
- `void setSeverity(Severity severity)`
- `Severity getSeverity()`
- `void attachTo(QWidget* target)`

---

## ElaInputDialog

**继承**: `QDialog` | **头文件**: `ElaInputDialog.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `TitleText` | get/set |
| `QString` | `SubTitleText` | get/set |
| `QString` | `LabelText` | get/set |
| `QString` | `TextValue` | get/set |
| `int` | `IntValue` | get/set |
| `double` | `DoubleValue` | get/set |
| `QString` | `OkButtonText` | get/set |
| `QString` | `CancelButtonText` | get/set |
| `QString` | `PlaceholderText` | get/set |
| `int` | `InputMinimumWidth` | get/set |
| `int` | `InputMaximumWidth` | get/set |

### 方法

- `void setTextEchoMode(QLineEdit::EchoMode mode)`
- `QLineEdit::EchoMode textEchoMode()`
- `void setIntRange(int minValue, int maxValue, int step = 1)`
- `void setDoubleRange(double minValue, double maxValue, int decimals = 2)`
- `void setMultiLine(bool multiLine)`

### 信号

- `textValueChanged(QString text)`
- `intValueChanged(int value)`
- `doubleValueChanged(double value)`

---

## ElaInteractiveCard

**继承**: `QPushButton` | **头文件**: `ElaInteractiveCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `int` | `TitlePixelSize` | get/set |
| `int` | `SubTitlePixelSize` | get/set |
| `int` | `TitleSpacing` | get/set |
| `QPixmap` | `CardPixmap` | get/set |
| `QSize` | `CardPixmapSize` | get/set |
| `int` | `CardPixmapBorderRadius` | get/set |
| `ElaCardPixType::PixMode` | `CardPixMode` | get/set |

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## ElaKeyBinder

**继承**: `QLabel` | **头文件**: `ElaKeyBinder.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `BinderKeyText` | get/set |
| `quint32` | `NativeVirtualBinderKey` | get/set |

### 信号

- `binderKeyTextChanged(QString binderKeyText)`
- `nativeVirtualBinderKeyChanged(quint32 binderKey)`

---

## ElaLCDNumber

**继承**: `QLCDNumber` | **头文件**: `ElaLCDNumber.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsUseAutoClock` | get/set |
| `QString` | `AutoClockFormat` | get/set |
| `bool` | `IsTransparent` | get/set |

---

## ElaLineEdit

**继承**: `QLineEdit` | **头文件**: `ElaLineEdit.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `IsClearButtonEnable` | get/set |

### 信号

- `focusIn(QString text)`
- `focusOut(QString text)`
- `wmFocusOut(QString text)`

---

## ElaListView

**继承**: `QListView` | **头文件**: `ElaListView.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `ItemHeight` | get/set |
| `bool` | `IsTransparent` | get/set |

---

## ElaLog

**继承**: `QObject` | **头文件**: `ElaLog.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `LogSavePath` | get/set |
| `QString` | `LogFileName` | get/set |
| `bool` | `IsLogFileNameWithTime` | get/set |

### 方法

- `void initMessageLog(bool isEnable)`

### 信号

- `logMessage(QString log)`

---

## ElaMarkdownViewer

**继承**: `QWidget` | **头文件**: `ElaMarkdownViewer.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `Markdown` | get/set |
| `int` | `BorderRadius` | get/set |

---

## ElaMenu

**继承**: `QMenu` | **头文件**: `ElaMenu.h`

### 方法

- `void setMenuItemHeight(int menuItemHeight)`
- `int getMenuItemHeight()`
- `QAction* addMenu(QMenu* menu)`
- `ElaMenu* addMenu(const QString& title)`
- `ElaMenu* addMenu(const QIcon& icon, const QString& title)`
- `ElaMenu* addMenu(ElaIconType::IconName icon, const QString& title)`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text)`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)`
- `bool isHasChildMenu()`
- `bool isHasIcon()`

### 信号

- `menuShow()`

---

## ElaMenuBar

**继承**: `QMenuBar` | **头文件**: `ElaMenuBar.h`

### 方法

- `QAction* addMenu(QMenu* menu)`
- `ElaMenu* addMenu(const QString& title)`
- `ElaMenu* addMenu(const QIcon& icon, const QString& title)`
- `ElaMenu* addMenu(ElaIconType::IconName, const QString& title)`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text)`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)`

---

## ElaMessageBar

**继承**: `QWidget` | **头文件**: `ElaMessageBar.h`

### 方法

- `static void success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr)`
- `static void warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr)`
- `static void information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr)`
- `static void error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr)`

---

## ElaMessageButton

**继承**: `QPushButton` | **头文件**: `ElaMessageButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `BarTitle` | get/set |
| `QString` | `BarText` | get/set |
| `int` | `DisplayMsec` | get/set |
| `QWidget*` | `MessageTargetWidget` | get/set |
| `ElaMessageBarType::MessageMode` | `MessageMode` | get/set |
| `ElaMessageBarType::PositionPolicy` | `PositionPolicy` | get/set |

---

## ElaMessageDialog

**继承**: `QWidget` | **头文件**: `ElaMessageDialog.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `Content` | get/set |
| `int` | `TitlePixelSize` | get/set |
| `int` | `ContentPixelSize` | get/set |

### 信号

- `confirmed()`
- `cancelled()`

---

## ElaMultiSelectComboBox

**继承**: `QComboBox` | **头文件**: `ElaMultiSelectComboBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `ShowCheckBox` | get/set |

### 方法

- `void setCurrentSelection(QString selection)`
- `void setCurrentSelection(QStringList selection)`
- `void setCurrentSelection(int index)`
- `void setCurrentSelection(QList<int> selectionIndex)`
- `QStringList getCurrentSelection()`

### 信号

- `itemSelectionChanged(QVector<bool> itemSelection)`
- `currentTextListChanged(QStringList selectedTextList)`

---

## ElaNotificationCenter

**继承**: `QWidget` | **头文件**: `ElaNotificationCenter.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `PanelWidth` | get/set |

### 结构体

**NotificationItem**
```cpp
  QString title;
  QString content;
  QString timestamp;
  ElaIconType::IconName icon = ElaIconType::None;
```

### 方法

- `void addNotification(const NotificationItem& item)`
- `void clearAll()`
- `int getNotificationCount()`
- `void showPanel(QWidget* anchor)`
- `void hidePanel()`
- `bool isPanelVisible()`

### 信号

- `notificationClicked(int index)`

---

## ElaNumberBox

**继承**: `QWidget` | **头文件**: `ElaNumberBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `double` | `Value` | get/set |
| `double` | `Minimum` | get/set |
| `double` | `Maximum` | get/set |
| `double` | `Step` | get/set |
| `int` | `Decimals` | get/set |
| `bool` | `IsWrapping` | get/set |

### 方法

- `void stepUp()`
- `void stepDown()`

### 信号

- `valueChanged(double value)`

---

## ElaPagination

**继承**: `QWidget` | **头文件**: `ElaPagination.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `CurrentPage` | get/set |
| `int` | `TotalPages` | get/set |
| `int` | `ButtonSize` | get/set |
| `int` | `PagerCount` | get/set |
| `bool` | `JumperVisible` | get/set |

### 信号

- `currentPageChanged(int page)`

---

## ElaPasswordBox

**继承**: `QLineEdit` | **头文件**: `ElaPasswordBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `IsPasswordVisible` | get/set |

### 信号

- `focusIn(QString text)`
- `focusOut(QString text)`
- `wmFocusOut(QString text)`

---

## ElaPersonPicture

**继承**: `QWidget` | **头文件**: `ElaPersonPicture.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `PictureSize` | get/set |
| `QString` | `DisplayName` | get/set |
| `QPixmap` | `Picture` | get/set |

---

## ElaPivot

**继承**: `QWidget` | **头文件**: `ElaPivot.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `TextPixelSize` | get/set |
| `int` | `CurrentIndex` | get/set |
| `int` | `PivotSpacing` | get/set |
| `int` | `MarkWidth` | get/set |

### 方法

- `void appendPivot(QString pivotTitle)`
- `void removePivot(QString pivotTitle)`

### 信号

- `pivotClicked(int index)`
- `pivotDoubleClicked(int index)`

---

## ElaPlainTextEdit

**继承**: `QPlainTextEdit` | **头文件**: `ElaPlainTextEdit.h`

---

## ElaPopconfirm

**继承**: `QWidget` | **头文件**: `ElaPopconfirm.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `Content` | get/set |
| `QString` | `ConfirmButtonText` | get/set |
| `QString` | `CancelButtonText` | get/set |
| `ElaIconType::IconName` | `Icon` | get/set |
| `bool` | `IsLightDismiss` | get/set |

### 方法

- `void showPopconfirm(QWidget* target)`
- `void closePopconfirm()`

### 信号

- `confirmed()`
- `cancelled()`
- `closed()`

---

## ElaPopularCard

**继承**: `QWidget` | **头文件**: `ElaPopularCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QPixmap` | `CardPixmap` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `QString` | `InteractiveTips` | get/set |
| `QString` | `DetailedText` | get/set |
| `QString` | `CardButtonText` | get/set |
| `QWidget*` | `CardFloatArea` | get/set |
| `QPixmap` | `CardFloatPixmap` | get/set |

### 信号

- `popularCardClicked()`
- `popularCardButtonClicked()`

---

## ElaProgressBar

**继承**: `QProgressBar` | **头文件**: `ElaProgressBar.h`

### 方法

- `void setMinimum(int minimum)`
- `void setMaximum(int maximum)`

---

## ElaProgressRing

**继承**: `QWidget` | **头文件**: `ElaProgressRing.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsBusying` | get/set |
| `bool` | `IsTransparent` | get/set |
| `bool` | `IsDisplayValue` | get/set |
| `ElaProgressRingType::ValueDisplayMode` | `ValueDisplayMode` | get/set |
| `int` | `BusyingWidth` | get/set |
| `int` | `BusyingDurationTime` | get/set |
| `int` | `Minimum` | get/set |
| `int` | `Maximum` | get/set |
| `int` | `Value` | get/set |
| `int` | `ValuePixelSize` | get/set |

### 方法

- `void setRange(int min, int max)`

### 信号

- `rangeChanged(int min, int max)`

---

## ElaPromotionCard

**继承**: `QWidget` | **头文件**: `ElaPromotionCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QPixmap` | `CardPixmap` | get/set |
| `QString` | `CardTitle` | get/set |
| `QString` | `PromotionTitle` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `QColor` | `CardTitleColor` | get/set |
| `QColor` | `PromotionTitleColor` | get/set |
| `QColor` | `PromotionTitleBaseColor` | get/set |
| `QColor` | `TitleColor` | get/set |
| `QColor` | `SubTitleColor` | get/set |
| `int` | `CardTitlePixelSize` | get/set |
| `int` | `PromotionTitlePixelSize` | get/set |
| `int` | `TitlePixelSize` | get/set |
| `int` | `SubTitlePixelSize` | get/set |
| `qreal` | `HorizontalCardPixmapRatio` | get/set |
| `qreal` | `VerticalCardPixmapRatio` | get/set |

### 信号

- `promotionCardClicked()`

---

## ElaPromotionView

**继承**: `QWidget` | **头文件**: `ElaPromotionView.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `CardExpandWidth` | get/set |
| `int` | `CardCollapseWidth` | get/set |
| `int` | `CurrentIndex` | get/set |
| `bool` | `IsAutoScroll` | get/set |
| `int` | `AutoScrollInterval` | get/set |

### 方法

- `void appendPromotionCard(ElaPromotionCard* card)`

---

## ElaPushButton

**继承**: `QPushButton` | **头文件**: `ElaPushButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QColor` | `LightDefaultColor` | get/set |
| `QColor` | `DarkDefaultColor` | get/set |
| `QColor` | `LightHoverColor` | get/set |
| `QColor` | `DarkHoverColor` | get/set |
| `QColor` | `LightPressColor` | get/set |
| `QColor` | `DarkPressColor` | get/set |

### 方法

- `void setLightTextColor(QColor color)`
- `QColor getLightTextColor()`
- `void setDarkTextColor(QColor color)`
- `QColor getDarkTextColor()`
- `void setElaIcon(ElaIconType::IconName icon)`
- `void setElaIcon(ElaIconType::IconName icon, int iconSize)`
- `void setHoverEnabled(bool enabled)`
- `bool isHoverEnabled()`

---

## ElaRadioButton

**继承**: `QRadioButton` | **头文件**: `ElaRadioButton.h`

---

## ElaRatingControl

**继承**: `QWidget` | **头文件**: `ElaRatingControl.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `MaxRating` | get/set |
| `int` | `Rating` | get/set |
| `int` | `StarSize` | get/set |
| `int` | `Spacing` | get/set |
| `bool` | `IsReadOnly` | get/set |

### 信号

- `ratingChanged(int rating)`

---

## ElaReminderCard

**继承**: `QPushButton` | **头文件**: `ElaReminderCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `int` | `TitlePixelSize` | get/set |
| `int` | `SubTitlePixelSize` | get/set |
| `int` | `TitleSpacing` | get/set |
| `QPixmap` | `CardPixmap` | get/set |
| `QSize` | `CardPixmapSize` | get/set |
| `int` | `CardPixmapBorderRadius` | get/set |
| `ElaCardPixType::PixMode` | `CardPixMode` | get/set |

### 方法

- `void setCardPixmapSize(int width, int height)`

---

## ElaRoller

**继承**: `QWidget` | **头文件**: `ElaRoller.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QStringList` | `ItemList` | get/set |
| `int` | `ItemHeight` | get/set |
| `int` | `MaxVisibleItems` | get/set |
| `int` | `CurrentIndex` | get/set |
| `bool` | `IsContainer` | get/set |
| `bool` | `IsEnableLoop` | get/set |

### 方法

- `void setCurrentData(const QString& data)`
- `QString getCurrentData()`

### 信号

- `currentDataChanged(const QString& data)`

---

## ElaRollerPicker

**继承**: `QPushButton` | **头文件**: `ElaRollerPicker.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

### 方法

- `void addRoller(const QStringList& itemList, bool isEnableLoop = true)`
- `void removeRoller(int index)`
- `void setRollerItemList(int index, const QStringList& itemList)`
- `void setRollerWidth(int index, int width)`
- `void setCurrentData(int index, const QString& data)`
- `void setCurrentData(const QStringList& dataList)`
- `QString getCurrentData(int index)`
- `QStringList getCurrentData()`
- `void setCurrentIndex(int rollerIndex, int index)`
- `void setCurrentIndex(const QList<int>& indexList)`
- `int getCurrentIndex(int rollerIndex)`

### 信号

- `currentDataChanged(const QStringList& dataList)`
- `currentDataSelectionChanged(const QStringList& dataList)`

---

## ElaScreenCaptureManager

**继承**: `QObject` | **头文件**: `ElaScreenCaptureManager.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

### 方法

- `QStringList getDisplayList()`
- `QImage grabScreenToImage()`
- `void startGrabScreen()`
- `void stopGrabScreen()`
- `bool getIsGrabScreen()`
- `bool setDisplayID(int displayID)`
- `int getDisplayID()`
- `void setGrabArea(int width, int height)`
- `void setGrabArea(int x, int y, int width, int height)`
- `QRect getGrabArea()`
- `void setGrabFrameRate(int frameRateValue)`
- `int getGrabFrameRate()`
- `explicit ElaScreenCaptureScreen(QWidget* parent = nullptr)`
- `void setIsSyncGrabSize(bool isSyncGrabSize)`
- `bool getIsSyncGrabSize()`

### 信号

- `grabImageUpdate(QImage img)`

---

## ElaScrollArea

**继承**: `QScrollArea` | **头文件**: `ElaScrollArea.h`

### 方法

- `void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5)`
- `void setIsOverShoot(Qt::Orientation orientation, bool isEnable)`
- `bool getIsOverShoot(Qt::Orientation orientation)`
- `void setIsAnimation(Qt::Orientation orientation, bool isAnimation)`
- `bool getIsAnimation(Qt::Orientation orientation)`

---

## ElaScrollBar

**继承**: `QScrollBar` | **头文件**: `ElaScrollBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsAnimation` | get/set |
| `qreal` | `SpeedLimit` | get/set |

### 信号

- `rangeAnimationFinished()`

---

## ElaScrollPage

**继承**: `QWidget` | **头文件**: `ElaScrollPage.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QWidget*` | `CustomWidget` | get/set |

### 方法

- `void addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable = true, bool isVerticalGrabGesture = true, qreal mousePressEventDelay = 0.5)`
- `void setPageTitle(const QString& title)`
- `void navigation(int widgetIndex, bool isLogRoute = true)`
- `void setPageTitleSpacing(int spacing)`
- `int getPageTitleSpacing()`
- `void setTitleVisible(bool isVisible)`

---

## ElaScrollPageArea

**继承**: `QWidget` | **头文件**: `ElaScrollPageArea.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |

---

## ElaSelectorBar

**继承**: `QWidget` | **头文件**: `ElaSelectorBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `CurrentIndex` | get/set |
| `int` | `BorderRadius` | get/set |

### 方法

- `void addItem(const QString& text)`
- `void addItem(ElaIconType::IconName icon, const QString& text)`
- `void clearItems()`
- `int getItemCount()`

### 信号

- `currentIndexChanged(int index)`

---

## ElaSheetPanel

**继承**: `QWidget` | **头文件**: `ElaSheetPanel.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `ElaSheetPanelType::Direction` | `Direction` | get/set |
| `qreal` | `PeekRatio` | get/set |
| `qreal` | `HalfRatio` | get/set |
| `qreal` | `FullRatio` | get/set |
| `bool` | `DragHandleVisible` | get/set |
| `bool` | `CloseOnOverlayClick` | get/set |
| `qreal` | `OverlayOpacity` | get/set |

### 方法

- `void setCentralWidget(QWidget* widget)`
- `void open(ElaSheetPanelType::DetentLevel level = ElaSheetPanelType::Half)`
- `void close()`
- `ElaSheetPanelType::DetentLevel currentDetent()`
- `bool isOpened()`

### 信号

- `opened()`
- `closed()`
- `detentChanged(ElaSheetPanelType::DetentLevel level)`

---

## ElaSkeleton

**继承**: `QWidget` | **头文件**: `ElaSkeleton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `IsAnimated` | get/set |

### 枚举

**SkeletonType**: `Text`, `Circle`, `Rectangle`

### 方法

- `void setSkeletonType(SkeletonType type)`
- `SkeletonType getSkeletonType()`

---

## ElaSlider

**继承**: `QSlider` | **头文件**: `ElaSlider.h`

---

## ElaSpinBox

**继承**: `QSpinBox` | **头文件**: `ElaSpinBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `ElaSpinBoxType::ButtonMode` | `ButtonMode` | get/set |

---

## ElaSplashScreen

**继承**: `QWidget` | **头文件**: `ElaSplashScreen.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `Minimum` | get/set |
| `int` | `Maximum` | get/set |
| `int` | `Value` | get/set |
| `bool` | `IsShowProgressBar` | get/set |
| `bool` | `IsShowProgressRing` | get/set |
| `bool` | `IsClosable` | get/set |

### 方法

- `void setLogo(const QPixmap& logo)`
- `void setTitle(const QString& title)`
- `void setSubTitle(const QString& subTitle)`
- `void setStatusText(const QString& text)`
- `void show()`
- `void close()`
- `void finish(QWidget* mainWindow)`

### 信号

- `closed()`

---

## ElaSplitButton

**继承**: `QWidget` | **头文件**: `ElaSplitButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Text` | get/set |
| `ElaIconType::IconName` | `ElaIcon` | get/set |

### 方法

- `void setMenu(ElaMenu* menu)`
- `ElaMenu* getMenu()`

### 信号

- `clicked()`

---

## ElaSpotlight

**继承**: `QWidget` | **头文件**: `ElaSpotlight.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `Padding` | get/set |
| `int` | `OverlayAlpha` | get/set |
| `bool` | `IsCircle` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `Content` | get/set |

### 结构体

**SpotlightStep**
```cpp
  QWidget* target{nullptr;
```

### 方法

- `void showSpotlight(QWidget* target, const QString& buttonText = "知道了")`
- `void setSteps(const QList<SpotlightStep>& steps)`
- `void start()`
- `void next()`
- `void previous()`
- `void finish()`
- `int currentStep()`
- `int stepCount()`

### 信号

- `stepChanged(int step)`
- `finished()`

---

## ElaStatCard

**继承**: `QWidget` | **头文件**: `ElaStatCard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QString` | `Title` | get/set |
| `QString` | `Value` | get/set |
| `QString` | `Description` | get/set |
| `int` | `BorderRadius` | get/set |
| `ElaIconType::IconName` | `CardIcon` | get/set |

### 枚举

**TrendType**: `None`, `Up`, `Down`, `Neutral`

### 方法

- `void setTrend(TrendType trend)`
- `TrendType getTrend()`
- `void setTrendText(const QString& text)`
- `QString getTrendText()`

---

## ElaStatusBar

**继承**: `QStatusBar` | **头文件**: `ElaStatusBar.h`

---

## ElaSteps

**继承**: `QWidget` | **头文件**: `ElaSteps.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `CurrentStep` | get/set |
| `int` | `StepCount` | get/set |

### 方法

- `void setStepTitles(const QStringList& titles)`
- `QStringList getStepTitles()`
- `void next()`
- `void previous()`

### 信号

- `currentStepChanged(int step)`

---

## ElaSuggestBox

**继承**: `QWidget` | **头文件**: `ElaSuggestBox.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `Qt::CaseSensitivity` | `CaseSensitivity` | get/set |

### 方法

- `void setPlaceholderText(const QString& placeholderText)`
- `void setFixedSize(const QSize& size)`
- `void setFixedSize(int w, int h)`
- `void setFixedHeight(int h)`
- `explicit SuggestData()`
- `explicit SuggestData(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {})`
- `QString addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {})`
- `QString addSuggestion(ElaIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {})`
- `QString addSuggestion(const ElaSuggestBox::SuggestData& suggestData)`
- `QStringList addSuggestion(const QList<ElaSuggestBox::SuggestData>& suggestDataList)`
- `void removeSuggestion(const QString& suggestKey)`
- `void removeSuggestion(int index)`
- `void clearSuggestion()`

### 信号

- `suggestionClicked(const ElaSuggestBox::SuggestData& suggestData)`

---

## ElaTabBar

**继承**: `QTabBar` | **头文件**: `ElaTabBar.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `QSize` | `TabSize` | get/set |

### 信号

- `tabDragCreate(QMimeData* mimeData)`
- `tabDragEnter(QMimeData* mimeData)`
- `tabDragLeave(QMimeData* mimeData)`
- `tabDragDrop(QMimeData* mimeData)`

---

## ElaTabWidget

**继承**: `QTabWidget` | **头文件**: `ElaTabWidget.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsTabTransparent` | get/set |
| `bool` | `IsContainerAcceptDrops` | get/set |
| `QSize` | `TabSize` | get/set |

### 方法

- `void setTabPosition(TabPosition position)`

---

## ElaTableView

**继承**: `QTableView` | **头文件**: `ElaTableView.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `HeaderMargin` | get/set |

### 信号

- `tableViewShow()`
- `tableViewHide()`

---

## ElaTableWidget

**继承**: `QTableWidget` | **头文件**: `ElaTableWidget.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `ItemHeight` | get/set |
| `int` | `HeaderMargin` | get/set |
| `bool` | `IsTransparent` | get/set |

### 方法

- `void insertRows(int row, int count)`
- `void removeRows(int row, int count)`
- `void insertColumns(int column, int count)`
- `void removeColumns(int column, int count)`
- `void setItemText(int row, int column, const QString& text)`
- `QString getItemText(int row, int column)`
- `void setRowData(int row, const QStringList& data)`
- `QStringList getRowData(int row)`

### 信号

- `tableWidgetShow()`
- `tableWidgetHide()`

---

## ElaTag

**继承**: `QWidget` | **头文件**: `ElaTag.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `TagText` | get/set |
| `bool` | `IsClosable` | get/set |
| `bool` | `IsCheckable` | get/set |
| `bool` | `IsChecked` | get/set |

### 枚举

**TagColor**: `Default`, `Primary`, `Success`, `Warning`, `Danger`

### 方法

- `void setTagColor(TagColor color)`
- `TagColor getTagColor()`

### 信号

- `closed()`
- `clicked()`
- `checkedChanged(bool checked)`

---

## ElaTeachingTip

**继承**: `QWidget` | **头文件**: `ElaTeachingTip.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Title` | get/set |
| `QString` | `SubTitle` | get/set |
| `QString` | `Content` | get/set |
| `ElaIconType::IconName` | `TipIcon` | get/set |
| `QPixmap` | `HeroImage` | get/set |
| `bool` | `IsLightDismiss` | get/set |

### 枚举

**TailPosition**: `Auto`, `Top`, `Bottom`, `Left`, `Right`

### 方法

- `void setTailPosition(TailPosition position)`
- `TailPosition getTailPosition()`
- `void setTarget(QWidget* target)`
- `QWidget* getTarget()`
- `void setCloseButtonVisible(bool visible)`
- `void clearActions()`
- `void showTip()`
- `void closeTip()`

### 信号

- `closed()`
- `closeButtonClicked()`

---

## ElaText

**继承**: `QLabel` | **头文件**: `ElaText.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `bool` | `IsWrapAnywhere` | get/set |
| `int` | `TextPixelSize` | get/set |
| `int` | `TextPointSize` | get/set |
| `ElaTextType::TextStyle` | `TextStyle` | get/set |
| `ElaIconType::IconName` | `ElaIcon` | get/set |

---

## ElaTimeline

**继承**: `QWidget` | **头文件**: `ElaTimeline.h`

### 结构体

**TimelineItem**
```cpp
  QString title;
  QString content;
  QString timestamp;
  ElaIconType::IconName icon = ElaIconType::None;
```

### 方法

- `void addItem(const TimelineItem& item)`
- `void clearItems()`
- `int getItemCount()`

---

## ElaToast

**继承**: `QWidget` | **头文件**: `ElaToast.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `DisplayMsec` | get/set |

### 枚举

**ToastType**: `Success`, `Info`, `Warning`, `Error`

### 方法

- `static void success(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr)`
- `static void info(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr)`
- `static void warning(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr)`
- `static void error(const QString& text, int displayMsec = 2000, QWidget* parent = nullptr)`

---

## ElaToggleButton

**继承**: `QWidget` | **头文件**: `ElaToggleButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `QString` | `Text` | get/set |

### 方法

- `void setIsToggled(bool isToggled)`
- `bool getIsToggled()`

### 信号

- `toggled(bool checked)`

---

## ElaToggleSwitch

**继承**: `QWidget` | **头文件**: `ElaToggleSwitch.h`

### 方法

- `void setIsToggled(bool isToggled)`
- `bool getIsToggled()`

### 信号

- `toggled(bool checked)`

---

## ElaToolBar

**继承**: `QToolBar` | **头文件**: `ElaToolBar.h`

### 方法

- `void setToolBarSpacing(int spacing)`
- `int getToolBarSpacing()`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text)`
- `QAction* addElaIconAction(ElaIconType::IconName icon, const QString& text, const QKeySequence& shortcut)`

---

## ElaToolButton

**继承**: `QToolButton` | **头文件**: `ElaToolButton.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `bool` | `IsSelected` | get/set |

### 方法

- `void setIsTransparent(bool isTransparent)`
- `bool getIsTransparent()`
- `void setMenu(ElaMenu* menu)`
- `void setElaIcon(ElaIconType::IconName icon)`
- `void setElaIcon(ElaIconType::IconName icon, int rotate)`

---

## ElaToolTip

**继承**: `QWidget` | **头文件**: `ElaToolTip.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `DisplayMsec` | get/set |
| `int` | `ShowDelayMsec` | get/set |
| `int` | `HideDelayMsec` | get/set |
| `QString` | `ToolTip` | get/set |
| `QWidget*` | `CustomWidget` | get/set |

### 方法

- `void updatePos()`

---

## ElaTransfer

**继承**: `QWidget` | **头文件**: `ElaTransfer.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `ItemHeight` | get/set |
| `QString` | `SourceTitle` | get/set |
| `QString` | `TargetTitle` | get/set |
| `bool` | `IsSearchVisible` | get/set |

### 方法

- `void setSourceItems(const QStringList& items)`
- `void addSourceItem(const QString& text)`
- `void addSourceItems(const QStringList& items)`
- `QStringList getSourceItems()`
- `QStringList getTargetItems()`
- `void moveToTarget()`
- `void moveToSource()`
- `void moveAllToTarget()`
- `void moveAllToSource()`

### 信号

- `transferChanged(const QStringList& sourceItems, const QStringList& targetItems)`

---

## ElaTreeView

**继承**: `QTreeView` | **头文件**: `ElaTreeView.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `ItemHeight` | get/set |
| `int` | `HeaderMargin` | get/set |

---

## ElaVirtualList

**继承**: `QListView` | **头文件**: `ElaVirtualList.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `ItemHeight` | get/set |
| `bool` | `IsTransparent` | get/set |
| `bool` | `IsAlternatingRowColors` | get/set |

### 方法

- `void setItemCount(int count)`
- `int getItemCount()`

### 信号

- `itemRequestData(int startIndex, int endIndex)`

---

## ElaWizard

**继承**: `QWidget` | **头文件**: `ElaWizard.h`

### 属性

| 类型 | 名称 | 读写 |
|------|------|------|
| `int` | `BorderRadius` | get/set |
| `int` | `CurrentStep` | get/set |

### 方法

- `void addStep(const QString& title, QWidget* page)`
- `void next()`
- `void previous()`
- `void finish()`
- `int getStepCount()`

### 信号

- `currentStepChanged(int step)`
- `finished()`
- `cancelled()`

---
