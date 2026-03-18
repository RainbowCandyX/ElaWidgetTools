from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout

from ElaWidgetTools import ElaListView
from ExamplePage.T_BasePage import T_BasePage
from ModelView.T_ListViewModel import T_ListViewModel


class T_ListView(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaListView")
        self.createCustomWidget("ListView with custom model")

        listView = ElaListView(self)
        listModel = T_ListViewModel(self)
        listView.setModel(listModel)

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaListView")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(listView)
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
