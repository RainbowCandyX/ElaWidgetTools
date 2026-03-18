from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout

from ElaWidgetTools import (
    ElaBreadcrumbBar,
    ElaPivot,
    ElaScrollPageArea,
    ElaText,
)
from ExamplePage.T_BasePage import T_BasePage


class T_Navigation(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaNavigation")
        self.createCustomWidget("Navigation related components")

        breadcrumbBar = ElaBreadcrumbBar(self)
        breadcrumbBar.appendBreadcrumb("Home")
        breadcrumbBar.appendBreadcrumb("Components")
        breadcrumbBar.appendBreadcrumb("Navigation")
        breadcrumbArea = ElaScrollPageArea(self)
        breadcrumbLayout = QHBoxLayout(breadcrumbArea)
        breadcrumbLayout.addWidget(ElaText("ElaBreadcrumbBar", self))
        breadcrumbLayout.addWidget(breadcrumbBar)
        breadcrumbLayout.addStretch()

        pivot = ElaPivot(self)
        pivot.addPivot("Page1")
        pivot.addPivot("Page2")
        pivot.addPivot("Page3")
        pivotArea = ElaScrollPageArea(self)
        pivotLayout = QHBoxLayout(pivotArea)
        pivotLayout.addWidget(ElaText("ElaPivot", self))
        pivotLayout.addWidget(pivot)
        pivotLayout.addStretch()

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaNavigation")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(breadcrumbArea)
        centerLayout.addWidget(pivotArea)
        centerLayout.addStretch()
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
