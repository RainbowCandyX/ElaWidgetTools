#!/usr/bin/env python3
"""
Auto-generate Shiboken6 typesystem XML and global.h from ElaWidgetTools headers.

Usage:
    python3 scripts/generate_bindings.py
"""

import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEADER_DIR = os.path.join(ROOT, "ElaWidgetTools")
BINDINGS_DIR = os.path.join(ROOT, "bindings")

SKIP_HEADERS = {
    "ElaDef.h",
    "ElaProperty.h",
    "ElaSingleton.h",
    "ElaExponentialBlur.h",
}

PLATFORM_ONLY = {
    "ElaDxgiManager.h": "win32",
    "ElaScreenCaptureManager.h": "darwin",
}

BASE_CLASS_MAP = {
    "QMainWindow": "QMainWindow",
    "QWidget": "QWidget",
    "QDialog": "QDialog",
    "QPushButton": "QPushButton",
    "QAbstractButton": "QAbstractButton",
    "QLabel": "QLabel",
    "QLineEdit": "QLineEdit",
    "QPlainTextEdit": "QPlainTextEdit",
    "QComboBox": "QComboBox",
    "QSpinBox": "QSpinBox",
    "QDoubleSpinBox": "QDoubleSpinBox",
    "QSlider": "QSlider",
    "QProgressBar": "QProgressBar",
    "QCheckBox": "QCheckBox",
    "QRadioButton": "QRadioButton",
    "QTabWidget": "QTabWidget",
    "QTabBar": "QTabBar",
    "QMenuBar": "QMenuBar",
    "QMenu": "QMenu",
    "QToolBar": "QToolBar",
    "QStatusBar": "QStatusBar",
    "QDockWidget": "QDockWidget",
    "QScrollArea": "QScrollArea",
    "QScrollBar": "QScrollBar",
    "QListView": "QListView",
    "QTableView": "QTableView",
    "QTableWidget": "QTableWidget",
    "QTreeView": "QTreeView",
    "QGraphicsScene": "QGraphicsScene",
    "QGraphicsView": "QGraphicsView",
    "QGraphicsItem": "QGraphicsItem",
    "QLayout": "QLayout",
    "QGroupBox": "QGroupBox",
    "QObject": "QObject",
    "QFrame": "QFrame",
    "QAbstractScrollArea": "QAbstractScrollArea",
    "QLCDNumber": "QLCDNumber",
}

ENUM_NAMESPACES = [
    ("ElaApplicationType", ["WindowDisplayMode"]),
    ("ElaThemeType", ["ThemeMode", "ThemeColor"]),
    ("ElaAppBarType", ["ButtonType", "CustomArea", "WMMouseActionType"]),
    ("ElaTextType", ["TextStyle"]),
    ("ElaNavigationType", ["NodeResult", "NavigationDisplayMode", "NavigationNodeType"]),
    ("ElaNavigationRouterType", ["NavigationRouteType", "RouteMode"]),
    ("ElaEventBusType", ["EventBusReturnType"]),
    ("ElaCardPixType", ["PixMode"]),
    ("ElaGraphicsSceneType", ["SceneMode"]),
    ("ElaMessageBarType", ["PositionPolicy", "MessageMode"]),
    ("ElaProgressRingType", ["ValueDisplayMode"]),
    ("ElaWindowType", ["StackSwitchMode", "PaintMode"]),
    ("ElaSpinBoxType", ["ButtonMode"]),
    ("ElaSheetPanelType", ["Direction", "DetentLevel"]),
    ("ElaIconType", ["IconName"]),
]


def parse_header(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        content = f.read()

    class_match = re.search(
        r"class\s+ELA_EXPORT\s+(\w+)\s*:\s*public\s+(\w+)", content
    )
    if not class_match:
        return None

    class_name = class_match.group(1)
    base_class = class_match.group(2)

    is_singleton = "Q_SINGLETON_CREATE_H" in content or "Q_SINGLETON_CREATE(" in content

    has_private_ctor = bool(
        re.search(r"private:\s*\n\s*explicit\s+" + class_name, content)
    )

    signals = re.findall(r"Q_SIGNAL\s+void\s+(\w+)\s*\(", content)

    inner_enums = []
    for m in re.finditer(r"enum\s+(\w+)\s*\{", content):
        enum_name = m.group(1)
        if enum_name not in ("ContentPositionType",):
            inner_enums.append(enum_name)

    return {
        "class_name": class_name,
        "base_class": base_class,
        "is_singleton": is_singleton,
        "has_private_ctor": has_private_ctor,
        "signals": signals,
        "inner_enums": inner_enums,
        "header": os.path.basename(filepath),
    }


def generate_global_h(classes):
    lines = [
        "#ifndef ELAWIDGETTOOLS_GLOBAL_H",
        "#define ELAWIDGETTOOLS_GLOBAL_H",
        "",
        "#include <ElaDef.h>",
        "#include <ElaProperty.h>",
        "#include <ElaSingleton.h>",
        "",
    ]
    for cls in sorted(classes, key=lambda c: c["header"]):
        lines.append(f'#include <{cls["header"]}>')
    lines.extend(["", "#endif // ELAWIDGETTOOLS_GLOBAL_H", ""])
    return "\n".join(lines)


def generate_typesystem(classes):
    lines = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        '<typesystem package="ElaWidgetTools">',
        '    <load-typesystem name="typesystem_widgets.xml" generate="no"/>',
        '    <load-typesystem name="typesystem_core.xml" generate="no"/>',
        '    <load-typesystem name="typesystem_gui.xml" generate="no"/>',
        "",
    ]

    lines.append("    <!-- Enum Namespaces -->")
    for ns_name, enums in ENUM_NAMESPACES:
        lines.append(f'    <namespace-type name="{ns_name}">')
        for enum in enums:
            lines.append(f'        <enum-type name="{enum}"/>')
        lines.append(f"    </namespace-type>")
        lines.append("")

    lines.append("    <!-- Components -->")
    for cls in sorted(classes, key=lambda c: c["class_name"]):
        cn = cls["class_name"]
        bc = cls["base_class"]

        if cls["is_singleton"]:
            lines.append(f'    <object-type name="{cn}">')
            lines.append(
                f'        <modify-function signature="{cn}(QObject*)" remove="all"/>'
            )
            for enum in cls["inner_enums"]:
                lines.append(f'        <enum-type name="{enum}"/>')
            lines.append(f"    </object-type>")
        elif cls["has_private_ctor"]:
            lines.append(f'    <object-type name="{cn}">')
            lines.append(
                f'        <modify-function signature="{cn}(QWidget*)" remove="all"/>'
            )
            for enum in cls["inner_enums"]:
                lines.append(f'        <enum-type name="{enum}"/>')
            lines.append(f"    </object-type>")
        else:
            if cls["inner_enums"]:
                lines.append(f'    <object-type name="{cn}">')
                for enum in cls["inner_enums"]:
                    lines.append(f'        <enum-type name="{enum}"/>')
                lines.append(f"    </object-type>")
            else:
                lines.append(f'    <object-type name="{cn}"/>')
        lines.append("")

    lines.append("</typesystem>")
    lines.append("")
    return "\n".join(lines)


def main():
    os.makedirs(BINDINGS_DIR, exist_ok=True)

    classes = []
    headers = sorted(os.listdir(HEADER_DIR))
    for h in headers:
        if not h.startswith("Ela") or not h.endswith(".h"):
            continue
        if h in SKIP_HEADERS:
            continue

        filepath = os.path.join(HEADER_DIR, h)
        info = parse_header(filepath)
        if info:
            classes.append(info)

    global_h = generate_global_h(classes)
    global_h_path = os.path.join(BINDINGS_DIR, "global.h")
    with open(global_h_path, "w", encoding="utf-8") as f:
        f.write(global_h)
    print(f"Generated {global_h_path} with {len(classes)} headers")

    typesystem = generate_typesystem(classes)
    typesystem_path = os.path.join(BINDINGS_DIR, "bindings.xml")
    with open(typesystem_path, "w", encoding="utf-8") as f:
        f.write(typesystem)
    print(f"Generated {typesystem_path} with {len(classes)} classes")

    print(f"\nEnum namespaces: {len(ENUM_NAMESPACES)}")
    print(f"Total enum types: {sum(len(e) for _, e in ENUM_NAMESPACES)}")


if __name__ == "__main__":
    main()
