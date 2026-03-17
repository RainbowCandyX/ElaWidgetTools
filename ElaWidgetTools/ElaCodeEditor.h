#ifndef ELACODEEDITOR_H
#define ELACODEEDITOR_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"
class ElaCodeEditorPrivate;
class ELA_EXPORT ElaCodeEditor : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaCodeEditor)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Code)
    Q_PROPERTY_CREATE_Q_H(bool, IsReadOnly)
    Q_PROPERTY_CREATE_Q_H(int, TabSize)
public:
    enum Language
    {
        CPP = 0,
        C,
        CSharp,
        Python,
        JavaScript,
        Lua,
        Rust,
        PHP
    };
    Q_ENUM(Language)

    explicit ElaCodeEditor(QWidget* parent = nullptr);
    ~ElaCodeEditor() override;

    void setLanguage(Language lang);
    Language getLanguage() const;
};

#endif // ELACODEEDITOR_H
