#!/usr/bin/env python3
"""
ElaWidgetTools API 文档生成器
从公开头文件中提取类信息、属性、方法、信号、枚举，生成 Markdown 文档。

用法: python3 scripts/generate_docs.py
输出: doc/API.md
"""

import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

HEADER_DIR = Path(__file__).resolve().parent.parent / "ElaWidgetTools"
OUTPUT_FILE = Path(__file__).resolve().parent.parent / "doc" / "API.md"

# 跳过的内部文件
SKIP_FILES = {
    "ElaProperty.h", "ElaSingleton.h", "ElaDef.h",
    "ElaExponentialBlur.h", "ElaWidgetTools.h",
}

# Q_PROPERTY_CREATE_Q_H(TYPE, NAME) 和 Q_PROPERTY_REF_CREATE_Q_H(TYPE, NAME)
RE_PROPERTY = re.compile(
    r"Q_PROPERTY(?:_REF)?_CREATE_Q_H\s*\(\s*(.+?)\s*,\s*(\w+)\s*\)"
)
# Q_PRIVATE_CREATE_Q_H / Q_PRIVATE_REF_CREATE_Q_H
RE_PRIVATE_PROP = re.compile(
    r"Q_PRIVATE(?:_REF)?_CREATE_Q_H\s*\(\s*(.+?)\s*,\s*(\w+)\s*\)"
)
# class ELA_EXPORT ClassName : public BaseClass
RE_CLASS = re.compile(
    r"class\s+ELA_EXPORT\s+(\w+)\s*:\s*public\s+(\w+)"
)
# Q_SIGNAL void signalName(args);
RE_SIGNAL = re.compile(
    r"Q_SIGNAL\s+void\s+(\w+)\s*\(([^)]*)\)\s*;"
)
# void/int/bool/etc methodName(args); — public methods (non-signal, non-override)
RE_METHOD = re.compile(
    r"^\s+((?:static\s+)?(?:virtual\s+)?(?:const\s+)?(?:\w+(?:::\w+)*[&*\s]+)+)(\w+)\s*\(([^)]*)\)\s*(?:const)?\s*;",
    re.MULTILINE,
)
# enum EnumName { ... }
RE_ENUM = re.compile(
    r"enum\s+(\w+)\s*\{([^}]+)\}", re.DOTALL
)
# struct StructName { ... }
RE_STRUCT = re.compile(
    r"struct\s+(\w+)\s*\{([^}]+)\}", re.DOTALL
)
# Signals block
RE_SIGNALS_BLOCK = re.compile(
    r"Q_SIGNALS\s*:(.*?)(?=\npublic:|\nprotected:|\nprivate:|\n};)", re.DOTALL
)
# Signal in block: void name(args);
RE_SIGNAL_IN_BLOCK = re.compile(
    r"void\s+(\w+)\s*\(([^)]*)\)\s*;"
)


@dataclass
class PropInfo:
    type: str
    name: str


@dataclass
class MethodInfo:
    return_type: str
    name: str
    args: str


@dataclass
class SignalInfo:
    name: str
    args: str


@dataclass
class EnumInfo:
    name: str
    values: list


@dataclass
class StructInfo:
    name: str
    fields: list


@dataclass
class ClassInfo:
    name: str = ""
    base: str = ""
    file: str = ""
    properties: list = field(default_factory=list)
    methods: list = field(default_factory=list)
    signals: list = field(default_factory=list)
    enums: list = field(default_factory=list)
    structs: list = field(default_factory=list)


def parse_header(filepath: Path) -> ClassInfo | None:
    content = filepath.read_text(encoding="utf-8", errors="ignore")

    # Find main exported class
    m = RE_CLASS.search(content)
    if not m:
        return None

    info = ClassInfo()
    info.name = m.group(1)
    info.base = m.group(2)
    info.file = filepath.name

    # Properties
    for pm in RE_PROPERTY.finditer(content):
        info.properties.append(PropInfo(type=pm.group(1).strip(), name=pm.group(2)))
    for pm in RE_PRIVATE_PROP.finditer(content):
        info.properties.append(PropInfo(type=pm.group(1).strip(), name=pm.group(2)))

    # Enums (inside the class)
    for em in RE_ENUM.finditer(content):
        enum_name = em.group(1)
        values = [v.strip().split("=")[0].strip() for v in em.group(2).split(",") if v.strip()]
        values = [v for v in values if v and not v.startswith("//")]
        info.enums.append(EnumInfo(name=enum_name, values=values))

    # Structs
    for sm in RE_STRUCT.finditer(content):
        struct_name = sm.group(1)
        fields_raw = sm.group(2).strip().split(";")
        fields = []
        for f in fields_raw:
            f = f.strip()
            if f and not f.startswith("//"):
                fields.append(f + ";")
        info.structs.append(StructInfo(name=struct_name, fields=fields))

    # Signals — from Q_SIGNAL macro
    for sm in RE_SIGNAL.finditer(content):
        sig_name = sm.group(1)
        if sig_name.startswith("p") and sig_name.endswith("Changed"):
            continue  # skip property change signals
        info.signals.append(SignalInfo(name=sig_name, args=sm.group(2).strip()))

    # Signals — from Q_SIGNALS block
    sig_block = RE_SIGNALS_BLOCK.search(content)
    if sig_block:
        block = sig_block.group(1)
        for sm in RE_SIGNAL_IN_BLOCK.finditer(block):
            sig_name = sm.group(1)
            if sig_name.startswith("p") and sig_name.endswith("Changed"):
                continue
            # avoid duplicates
            if not any(s.name == sig_name for s in info.signals):
                info.signals.append(SignalInfo(name=sig_name, args=sm.group(2).strip()))

    # Public methods — extract from "public:" section, skip property macros, signals, overrides
    public_sections = re.findall(
        r"\npublic:\s*\n(.*?)(?=\nQ_SIGNALS|\nprotected:|\nprivate:|\n};)",
        content, re.DOTALL
    )

    skip_patterns = {"set", "get", "override", "Q_PROPERTY", "Q_SIGNAL", "Q_INVOKABLE", "explicit", "~"}
    for section in public_sections:
        for line in section.split("\n"):
            line = line.strip()
            if not line or line.startswith("//") or line.startswith("Q_"):
                continue
            if any(sp in line for sp in ["Q_PROPERTY", "Q_PRIVATE", "Q_ENUM", "Q_Q_CREATE"]):
                continue
            if "override" in line:
                continue
            # Match method declarations
            mm = re.match(
                r"((?:static\s+)?(?:(?:const\s+)?[\w:]+[&*\s]+)+)(\w+)\s*\(([^)]*)\)\s*(const)?\s*;",
                line,
            )
            if mm:
                ret = mm.group(1).strip()
                name = mm.group(2)
                args = mm.group(3).strip()
                # Skip destructors, constructors, property getters/setters that are auto-generated
                if name.startswith("~") or name == info.name:
                    continue
                info.methods.append(MethodInfo(return_type=ret, name=name, args=args))

    return info


def generate_markdown(classes: list[ClassInfo]) -> str:
    lines = []
    lines.append("# ElaWidgetTools API 文档")
    lines.append("")
    lines.append("> 本文档由 `scripts/generate_docs.py` 自动生成，请勿手动编辑。")
    lines.append(">")
    lines.append(f"> 共 **{len(classes)}** 个公开组件")
    lines.append("")

    # TOC
    lines.append("## 目录")
    lines.append("")
    for cls in classes:
        anchor = cls.name.lower()
        lines.append(f"- [{cls.name}](#{anchor})")
    lines.append("")
    lines.append("---")
    lines.append("")

    for cls in classes:
        lines.append(f"## {cls.name}")
        lines.append("")
        lines.append(f"**继承**: `{cls.base}` | **头文件**: `{cls.file}`")
        lines.append("")

        # Properties
        if cls.properties:
            lines.append("### 属性")
            lines.append("")
            lines.append("| 类型 | 名称 | 读写 |")
            lines.append("|------|------|------|")
            for p in cls.properties:
                lines.append(f"| `{p.type}` | `{p.name}` | get/set |")
            lines.append("")

        # Enums
        if cls.enums:
            lines.append("### 枚举")
            lines.append("")
            for e in cls.enums:
                lines.append(f"**{e.name}**: `{'`, `'.join(e.values)}`")
                lines.append("")

        # Structs
        if cls.structs:
            lines.append("### 结构体")
            lines.append("")
            for s in cls.structs:
                lines.append(f"**{s.name}**")
                lines.append("```cpp")
                for f in s.fields:
                    lines.append(f"  {f}")
                lines.append("```")
                lines.append("")

        # Methods
        if cls.methods:
            lines.append("### 方法")
            lines.append("")
            for m in cls.methods:
                args_str = m.args if m.args else ""
                lines.append(f"- `{m.return_type} {m.name}({args_str})`")
            lines.append("")

        # Signals
        if cls.signals:
            lines.append("### 信号")
            lines.append("")
            for s in cls.signals:
                args_str = s.args if s.args else ""
                lines.append(f"- `{s.name}({args_str})`")
            lines.append("")

        lines.append("---")
        lines.append("")

    return "\n".join(lines)


def main():
    headers = sorted(HEADER_DIR.glob("*.h"))
    classes = []

    for h in headers:
        if h.name in SKIP_FILES:
            continue
        if h.name.startswith("Ela") and h.suffix == ".h":
            info = parse_header(h)
            if info:
                classes.append(info)

    # Sort: common infrastructure first, then alphabetical
    priority = [
        "ElaApplication", "ElaTheme", "ElaWindow", "ElaWidget",
        "ElaAppBar", "ElaNavigationBar", "ElaNavigationRouter",
    ]
    def sort_key(c):
        if c.name in priority:
            return (0, priority.index(c.name))
        return (1, c.name)

    classes.sort(key=sort_key)

    md = generate_markdown(classes)
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT_FILE.write_text(md, encoding="utf-8")
    print(f"Generated {OUTPUT_FILE} with {len(classes)} components")


if __name__ == "__main__":
    main()
