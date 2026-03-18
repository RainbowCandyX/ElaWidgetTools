from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout

from ElaWidgetTools import (
    ElaLCDNumber,
    ElaPromotionCard,
    ElaPromotionView,
    ElaScrollPageArea,
    ElaText,
)
from ExamplePage.T_BasePage import T_BasePage


class T_Card(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaCard")
        self.createCustomWidget("A collection of card components")

        lcdNumber = ElaLCDNumber(self)
        lcdNumber.setIsUseAutoClock(True)
        lcdNumber.setIsTransparent(False)
        lcdNumber.setFixedHeight(100)

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaCard")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(ElaText("ElaLCDNumber", self))
        centerLayout.addWidget(lcdNumber)
        centerLayout.addStretch()
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
