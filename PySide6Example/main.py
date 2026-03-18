import sys
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

from PySide6.QtCore import Qt
from PySide6.QtWidgets import QApplication
from ElaWidgetTools import ElaApplication

from mainwindow import MainWindow

app = QApplication(sys.argv)
eApp = ElaApplication.getInstance()
eApp.init()
w = MainWindow()
w.show()
sys.exit(app.exec())
