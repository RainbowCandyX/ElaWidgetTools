from PySide6.QtCore import Qt
from PySide6.QtWidgets import QVBoxLayout

from ElaWidgetTools import ElaWidget, ElaText


class T_About(ElaWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("About")

        layout = QVBoxLayout(self)
        title = ElaText("ElaWidgetTools", self)
        title.setTextPixelSize(24)
        title.setAlignment(Qt.AlignmentFlag.AlignCenter)

        subtitle = ElaText("PySide6 Bindings Example", self)
        subtitle.setTextPixelSize(14)
        subtitle.setAlignment(Qt.AlignmentFlag.AlignCenter)

        repo = ElaText("https://github.com/RainbowCandyX/ElaWidgetTools", self)
        repo.setTextPixelSize(11)
        repo.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        repo.setAlignment(Qt.AlignmentFlag.AlignCenter)

        layout.addStretch()
        layout.addWidget(title)
        layout.addSpacing(10)
        layout.addWidget(subtitle)
        layout.addSpacing(5)
        layout.addWidget(repo)
        layout.addStretch()
