#ifndef ELACODEEDITORPRIVATE_H
#define ELACODEEDITORPRIVATE_H

#include <QObject>

#include "ElaDef.h"
class ElaCodePlainTextEdit;
class ElaCodeEditor;
class ElaCodeEditorPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaCodeEditor)
    Q_PROPERTY_CREATE_D(QString, Code)
    Q_PROPERTY_CREATE_D(bool, IsReadOnly)
    Q_PROPERTY_CREATE_D(int, TabSize)
public:
    explicit ElaCodeEditorPrivate(QObject* parent = nullptr);
    ~ElaCodeEditorPrivate() override;
    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    friend class ElaCodeEditorLineArea;
    friend class ElaCodeHighlighter;
    ElaThemeType::ThemeMode _themeMode;
    int _language{0};
    ElaCodePlainTextEdit* _editor{nullptr};
    QWidget* _lineArea{nullptr};
    void* _highlighter{nullptr};
};

#endif // ELACODEEDITORPRIVATE_H
