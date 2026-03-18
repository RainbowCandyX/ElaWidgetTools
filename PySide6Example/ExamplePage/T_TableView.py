from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout

from ElaWidgetTools import ElaTableView
from ExamplePage.T_BasePage import T_BasePage
from ModelView.T_TableViewModel import T_TableViewModel


class T_TableView(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaTableView")
        self.createCustomWidget("TableView with custom model")

        tableView = ElaTableView(self)
        tableModel = T_TableViewModel(self)
        tableView.setModel(tableModel)
        tableView.setAlternatingRowColors(True)

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaTableView")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(tableView)
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
