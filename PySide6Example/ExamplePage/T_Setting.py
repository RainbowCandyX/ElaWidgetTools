from PySide6.QtCore import Qt
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout

from ElaWidgetTools import (
    ElaTheme,
    ElaThemeType,
    ElaToggleSwitch,
    ElaComboBox,
    ElaScrollPageArea,
    ElaText,
)
from ExamplePage.T_BasePage import T_BasePage


class T_Setting(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaSetting")
        self.createCustomWidget("Application settings")

        eTheme = ElaTheme.getInstance()

        themeSwitch = ElaToggleSwitch(self)
        themeSwitch.setIsToggled(
            eTheme.getThemeMode() == ElaThemeType.ThemeMode.Dark
        )
        themeSwitch.toggled.connect(
            lambda checked: eTheme.setThemeMode(
                ElaThemeType.ThemeMode.Dark
                if checked
                else ElaThemeType.ThemeMode.Light
            )
        )
        themeArea = ElaScrollPageArea(self)
        themeLayout = QHBoxLayout(themeArea)
        themeLayout.addWidget(ElaText("Dark Mode", self))
        themeLayout.addStretch()
        themeLayout.addWidget(themeSwitch)

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaSetting")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(themeArea)
        centerLayout.addStretch()
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
