from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QPixmap, QIcon
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QMenu, QAction

from ElaWidgetTools import (
    ElaWindow,
    ElaAppBarType,
    ElaIconType,
    ElaNavigationType,
    ElaThemeType,
    ElaTheme,
    ElaContentDialog,
    ElaMenuBar,
    ElaMenu,
    ElaToolBar,
    ElaToolButton,
    ElaDockWidget,
    ElaStatusBar,
    ElaText,
    ElaProgressBar,
)

from ExamplePage.T_BaseComponents import T_BaseComponents
from ExamplePage.T_Card import T_Card
from ExamplePage.T_Icon import T_Icon
from ExamplePage.T_Navigation import T_Navigation
from ExamplePage.T_Popup import T_Popup
from ExamplePage.T_Setting import T_Setting
from ExamplePage.T_About import T_About
from ExamplePage.T_ListView import T_ListView
from ExamplePage.T_TableView import T_TableView


class MainWindow(ElaWindow):
    def __init__(self, parent: QWidget = None):
        super().__init__(parent)
        self._initWindow()
        self._initEdgeLayout()
        self._initContent()

        closeDialog = ElaContentDialog(self)
        closeDialog.rightButtonClicked.connect(self.closeWindow)
        closeDialog.middleButtonClicked.connect(
            lambda: (closeDialog.close(), self.showMinimized())
        )
        self.setIsDefaultClosed(False)
        self.closeButtonClicked.connect(lambda: closeDialog.exec())
        self.moveToCenter()

    def _initWindow(self):
        self.resize(1200, 740)
        self.setUserInfoCardTitle("ElaWidgetTools")
        self.setUserInfoCardSubTitle("PySide6 Example")
        self.setWindowTitle("ElaWidgetTools - PySide6")

    def _initEdgeLayout(self):
        menuBar = ElaMenuBar(self)
        menuBar.setFixedHeight(30)
        customWidget = QWidget(self)
        customLayout = QVBoxLayout(customWidget)
        customLayout.setContentsMargins(0, 0, 0, 0)
        customLayout.addWidget(menuBar)
        customLayout.addStretch()
        self.setCustomWidget(ElaAppBarType.CustomArea.MiddleArea, customWidget)

        menuBar.addElaIconAction(ElaIconType.IconName.AtomSimple, "Action")
        iconMenu = menuBar.addMenu(ElaIconType.IconName.Aperture, "Menu")
        iconMenu.setMenuItemHeight(27)
        iconMenu.addElaIconAction(ElaIconType.IconName.BoxCheck, "Sort")
        iconMenu.addElaIconAction(ElaIconType.IconName.Copy, "Copy")
        iconMenu.addSeparator()
        iconMenu.addElaIconAction(ElaIconType.IconName.ArrowRotateRight, "Refresh")

        toolBar = ElaToolBar("ToolBar", self)
        toolBar.setAllowedAreas(
            Qt.ToolBarArea.TopToolBarArea | Qt.ToolBarArea.BottomToolBarArea
        )
        toolBar.setToolBarSpacing(3)
        toolBar.setToolButtonStyle(Qt.ToolButtonStyle.ToolButtonIconOnly)
        toolBar.setIconSize(QSize(25, 25))

        for icon in [
            ElaIconType.IconName.BadgeCheck,
            ElaIconType.IconName.ChartUser,
            ElaIconType.IconName.Bluetooth,
            ElaIconType.IconName.ChartSimple,
            ElaIconType.IconName.Coins,
        ]:
            btn = ElaToolButton(self)
            btn.setElaIcon(icon)
            toolBar.addWidget(btn)

        progressBar = ElaProgressBar(self)
        progressBar.setMinimum(0)
        progressBar.setMaximum(0)
        progressBar.setFixedWidth(350)
        toolBar.addWidget(progressBar)

        self.addToolBar(Qt.ToolBarArea.TopToolBarArea, toolBar)

        statusBar = ElaStatusBar(self)
        statusText = ElaText("Ready", self)
        statusText.setTextPixelSize(14)
        statusBar.addWidget(statusText)
        self.setStatusBar(statusBar)

    def _initContent(self):
        self._homePage = QWidget()
        self._baseComponentsPage = T_BaseComponents(self)
        self._iconPage = T_Icon(self)
        self._cardPage = T_Card(self)
        self._navigationPage = T_Navigation(self)
        self._popupPage = T_Popup(self)
        self._listViewPage = T_ListView(self)
        self._tableViewPage = T_TableView(self)
        self._settingPage = T_Setting(self)

        self.addPageNode("HOME", self._homePage, ElaIconType.IconName.House)
        self.addPageNode(
            "ElaBaseComponents",
            self._baseComponentsPage,
            ElaIconType.IconName.CabinetFiling,
        )

        _, viewKey = self.addExpanderNode(
            "ElaView", ElaIconType.IconName.CameraViewfinder
        )
        self.addPageNode(
            "ElaListView", self._listViewPage, viewKey, ElaIconType.IconName.List
        )
        self.addPageNode(
            "ElaTableView", self._tableViewPage, viewKey, ElaIconType.IconName.Table
        )
        self.expandNavigationNode(viewKey)

        self.addPageNode("ElaCard", self._cardPage, ElaIconType.IconName.Cards)
        self.addPageNode(
            "ElaNavigation", self._navigationPage, ElaIconType.IconName.LocationArrow
        )
        self.addPageNode("ElaPopup", self._popupPage, ElaIconType.IconName.Envelope)
        self.addPageNode("ElaIcon", self._iconPage, ElaIconType.IconName.FontCase)

        self._aboutPage = T_About()
        self._aboutPage.hide()
        _, aboutKey = self.addFooterNode("About", None, 0, ElaIconType.IconName.User)

        def onNavClicked(nodeType, nodeKey):
            if aboutKey == nodeKey:
                self._aboutPage.setFixedSize(400, 400)
                self._aboutPage.show()

        self.navigationNodeClicked.connect(onNavClicked)

        _, settingKey = self.addFooterNode(
            "Setting", self._settingPage, 0, ElaIconType.IconName.GearComplex
        )
