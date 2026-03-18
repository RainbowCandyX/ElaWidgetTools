from PySide6.QtCore import Qt, QAbstractListModel, QModelIndex


class T_ListViewModel(QAbstractListModel):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._dataList = [
            "ElaWidgetTools - FluentUI Style Components",
            "Supports Light and Dark themes",
            "100+ built-in components",
            "Cross-platform: Windows, macOS, Linux",
            "PySide6 bindings via Shiboken6",
            "Custom property system with Q_PROPERTY macros",
            "3500+ FluentUI icons included",
            "Smooth animations and transitions",
            "HiDPI / Retina display support",
            "MIT License - Free for all projects",
        ]

    def rowCount(self, parent=QModelIndex()):
        return len(self._dataList)

    def data(self, index: QModelIndex, role: int = Qt.ItemDataRole.DisplayRole):
        if role == Qt.ItemDataRole.DisplayRole:
            return self._dataList[index.row()]
        return None
