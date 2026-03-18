from PySide6.QtCore import Qt, QAbstractTableModel, QModelIndex


class T_TableViewModel(QAbstractTableModel):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._header = ["Component", "Category", "Description"]
        self._dataList = [
            ["ElaWindow", "Infrastructure", "Frameless window with navigation"],
            ["ElaPushButton", "Button", "Standard button with icon support"],
            ["ElaToggleSwitch", "Button", "Slide toggle switch"],
            ["ElaComboBox", "Input", "Dropdown selection box"],
            ["ElaLineEdit", "Input", "Text input with focus animation"],
            ["ElaProgressBar", "Display", "Progress bar with busy mode"],
            ["ElaListView", "View", "List view with FluentUI style"],
            ["ElaTableView", "View", "Table view with custom header"],
            ["ElaMenu", "Popup", "Context menu with icons"],
            ["ElaContentDialog", "Popup", "Modal dialog with mask"],
            ["ElaWatermark", "Overlay", "Text/image watermark overlay"],
            ["ElaCodeEditor", "Editor", "Syntax highlighting editor"],
        ]

    def rowCount(self, parent=QModelIndex()):
        return len(self._dataList)

    def columnCount(self, parent=QModelIndex()):
        return len(self._header)

    def data(self, index: QModelIndex, role: int = Qt.ItemDataRole.DisplayRole):
        if role == Qt.ItemDataRole.DisplayRole:
            return self._dataList[index.row()][index.column()]
        if role == Qt.ItemDataRole.TextAlignmentRole and index.column() == 1:
            return Qt.AlignmentFlag.AlignCenter
        return None

    def headerData(self, section, orientation, role):
        if (
            orientation == Qt.Orientation.Horizontal
            and role == Qt.ItemDataRole.DisplayRole
        ):
            return self._header[section]
        return None
