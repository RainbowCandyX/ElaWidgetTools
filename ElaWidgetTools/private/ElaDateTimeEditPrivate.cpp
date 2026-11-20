#include "ElaDateTimeEditPrivate.h"

#include "ElaDateTimeEdit.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QLineEdit>
ElaDateTimeEditPrivate::ElaDateTimeEditPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaDateTimeEditPrivate::~ElaDateTimeEditPrivate()
{
}

void ElaDateTimeEditPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaDateTimeEdit);
    _themeMode = themeMode;
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::transparent);
    palette.setColor(QPalette::Text, ElaThemeColor(_themeMode, BasicText));
    q->lineEdit()->setPalette(palette);
}

ElaMenu* ElaDateTimeEditPrivate::_createStandardContextMenu()
{
    Q_Q(ElaDateTimeEdit);
    QLineEdit* lineEdit = q->lineEdit();
    ElaMenu* menu = new ElaMenu(q);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(lineEdit->isUndoAvailable());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

        action = menu->addElaIconAction(ElaIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(lineEdit->isRedoAvailable());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
    }

    action = menu->addElaIconAction(ElaIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

    if (!lineEdit->isReadOnly())
    {
        action = menu->addElaIconAction(ElaIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
    }
#endif
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!lineEdit->text().isEmpty() && !(lineEdit->selectedText() == lineEdit->text()));
    connect(action, &QAction::triggered, q, &ElaDateTimeEdit::selectAll);
    return menu;
}
