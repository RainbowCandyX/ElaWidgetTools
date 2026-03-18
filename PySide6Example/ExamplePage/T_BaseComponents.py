from PySide6.QtCore import Qt
from PySide6.QtGui import QMouseEvent
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout

from ElaWidgetTools import (
    ElaToggleSwitch,
    ElaToggleButton,
    ElaComboBox,
    ElaMultiSelectComboBox,
    ElaMessageButton,
    ElaMessageBarType,
    ElaCheckBox,
    ElaSpinBox,
    ElaSlider,
    ElaRadioButton,
    ElaProgressBar,
    ElaProgressRing,
    ElaProgressRingType,
    ElaPlainTextEdit,
    ElaScrollPageArea,
    ElaText,
)
from ExamplePage.T_BasePage import T_BasePage


class T_BaseComponents(T_BasePage):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("ElaBaseComponents")
        self.createCustomWidget("A collection of common base components")

        toggleSwitch = ElaToggleSwitch(self)
        toggleSwitchArea = ElaScrollPageArea(self)
        toggleSwitchLayout = QHBoxLayout(toggleSwitchArea)
        toggleSwitchLayout.addWidget(ElaText("ElaToggleSwitch", self))
        toggleSwitchLayout.addWidget(toggleSwitch)
        toggleSwitchLayout.addStretch()

        toggleButton = ElaToggleButton("ToggleButton", self)
        toggleButton.setFixedWidth(120)
        toggleButtonArea = ElaScrollPageArea(self)
        toggleButtonLayout = QHBoxLayout(toggleButtonArea)
        toggleButtonLayout.addWidget(ElaText("ElaToggleButton", self))
        toggleButtonLayout.addWidget(toggleButton)
        toggleButtonLayout.addStretch()

        comboBox = ElaComboBox(self)
        comboBox.addItems(["Option A", "Option B", "Option C", "Option D"])
        comboBoxArea = ElaScrollPageArea(self)
        comboBoxLayout = QHBoxLayout(comboBoxArea)
        comboBoxLayout.addWidget(ElaText("ElaComboBox", self))
        comboBoxLayout.addWidget(comboBox)
        comboBoxLayout.addStretch()

        multiComboBox = ElaMultiSelectComboBox(self)
        multiComboBox.addItems(["Alpha", "Beta", "Gamma", "Delta", "Epsilon"])
        multiComboBox.setCurrentSelection(["Alpha", "Gamma"])
        multiComboArea = ElaScrollPageArea(self)
        multiComboLayout = QHBoxLayout(multiComboArea)
        multiComboLayout.addWidget(ElaText("ElaMultiSelectComboBox", self))
        multiComboLayout.addWidget(multiComboBox)
        multiComboLayout.addStretch()

        successBtn = ElaMessageButton("Success", self)
        successBtn.setBarTitle("Success")
        successBtn.setBarText("Operation completed successfully!")

        infoBtn = ElaMessageButton("Info", self)
        infoBtn.setBarTitle("Information")
        infoBtn.setBarText("Here is some information for you.")
        infoBtn.setMessageMode(ElaMessageBarType.MessageMode.Information)
        infoBtn.setPositionPolicy(ElaMessageBarType.PositionPolicy.TopLeft)

        warningBtn = ElaMessageButton("Warning", self)
        warningBtn.setBarTitle("Warning")
        warningBtn.setBarText("Please check your input.")
        warningBtn.setMessageMode(ElaMessageBarType.MessageMode.Warning)
        warningBtn.setPositionPolicy(ElaMessageBarType.PositionPolicy.BottomLeft)

        errorBtn = ElaMessageButton("Error", self)
        errorBtn.setBarTitle("Error")
        errorBtn.setBarText("Something went wrong!")
        errorBtn.setMessageMode(ElaMessageBarType.MessageMode.Error)
        errorBtn.setPositionPolicy(ElaMessageBarType.PositionPolicy.BottomRight)

        msgArea = ElaScrollPageArea(self)
        msgLayout = QHBoxLayout(msgArea)
        msgLayout.addWidget(ElaText("ElaMessageButton", self))
        msgLayout.addWidget(successBtn)
        msgLayout.addWidget(infoBtn)
        msgLayout.addWidget(warningBtn)
        msgLayout.addWidget(errorBtn)
        msgLayout.addStretch()

        checkBox = ElaCheckBox("CheckBox", self)
        checkBoxArea = ElaScrollPageArea(self)
        checkBoxLayout = QHBoxLayout(checkBoxArea)
        checkBoxLayout.addWidget(ElaText("ElaCheckBox", self))
        checkBoxLayout.addWidget(checkBox)
        checkBoxLayout.addStretch()

        spinBox = ElaSpinBox(self)
        spinBoxArea = ElaScrollPageArea(self)
        spinBoxLayout = QHBoxLayout(spinBoxArea)
        spinBoxLayout.addWidget(ElaText("ElaSpinBox", self))
        spinBoxLayout.addWidget(spinBox)
        spinBoxLayout.addStretch()

        slider = ElaSlider(self)
        sliderArea = ElaScrollPageArea(self)
        sliderLayout = QHBoxLayout(sliderArea)
        sliderLayout.addWidget(ElaText("ElaSlider", self))
        sliderLayout.addWidget(slider)
        sliderLayout.addStretch()

        radioButton = ElaRadioButton("RadioButton", self)
        radioArea = ElaScrollPageArea(self)
        radioLayout = QHBoxLayout(radioArea)
        radioLayout.addWidget(ElaText("ElaRadioButton", self))
        radioLayout.addWidget(radioButton)
        radioLayout.addStretch()

        progressBar = ElaProgressBar(self)
        progressBar.setMinimum(0)
        progressBar.setMaximum(0)
        progressBarArea = ElaScrollPageArea(self)
        progressBarLayout = QHBoxLayout(progressBarArea)
        progressBarLayout.addWidget(ElaText("ElaProgressBar", self))
        progressBarLayout.addWidget(progressBar)
        progressBarLayout.addStretch()

        progressRing = ElaProgressRing(self)
        progressRing.setValue(30)
        progressPercentRing = ElaProgressRing(self)
        progressPercentRing.setValue(50)
        progressPercentRing.setValueDisplayMode(
            ElaProgressRingType.ValueDisplayMode.Percent
        )
        progressBusyRing = ElaProgressRing(self)
        progressBusyRing.setIsBusying(True)
        progressRingArea = ElaScrollPageArea(self)
        progressRingArea.setFixedHeight(90)
        progressRingLayout = QHBoxLayout(progressRingArea)
        progressRingLayout.addWidget(ElaText("ElaProgressRing", self))
        progressRingLayout.addWidget(progressRing)
        progressRingLayout.addSpacing(10)
        progressRingLayout.addWidget(progressPercentRing)
        progressRingLayout.addSpacing(10)
        progressRingLayout.addWidget(progressBusyRing)
        progressRingLayout.addStretch()

        edit = ElaPlainTextEdit(self)
        edit.setPlainText("This is an ElaPlainTextEdit")

        centralWidget = QWidget(self)
        centralWidget.setWindowTitle("ElaBaseComponents")
        centerLayout = QVBoxLayout(centralWidget)
        centerLayout.addWidget(toggleSwitchArea)
        centerLayout.addWidget(toggleButtonArea)
        centerLayout.addWidget(comboBoxArea)
        centerLayout.addWidget(multiComboArea)
        centerLayout.addWidget(msgArea)
        centerLayout.addWidget(checkBoxArea)
        centerLayout.addWidget(spinBoxArea)
        centerLayout.addWidget(sliderArea)
        centerLayout.addWidget(radioArea)
        centerLayout.addWidget(progressBarArea)
        centerLayout.addWidget(progressRingArea)
        centerLayout.addWidget(edit)
        centerLayout.addStretch()
        centerLayout.setContentsMargins(0, 0, 0, 0)
        self.addCentralWidget(centralWidget, True, True, 0)
