from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout, QListView

from ElaWidgetTools import ElaListView, ElaLineEdit
from ExamplePage.T_BasePage import T_BasePage


class T_Icon(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaIcon")
        self.createCustomWidget("A collection of icons, click to copy enum name")

        centralWidget = QWidget(self)
        centerVLayout = QVBoxLayout(centralWidget)
        centerVLayout.setContentsMargins(0, 0, 5, 0)
        centralWidget.setWindowTitle("ElaIcon")

        iconView = ElaListView(self)
        iconView.setIsTransparent(True)
        iconView.setFlow(QListView.Flow.LeftToRight)
        iconView.setViewMode(QListView.ViewMode.IconMode)
        iconView.setResizeMode(QListView.ResizeMode.Adjust)

        searchEdit = ElaLineEdit(self)
        searchEdit.setPlaceholderText("Search icons")
        searchEdit.setFixedSize(300, 35)

        centerVLayout.addSpacing(13)
        centerVLayout.addWidget(searchEdit)
        centerVLayout.addWidget(iconView)
        self.addCentralWidget(centralWidget, True, True, 0)
