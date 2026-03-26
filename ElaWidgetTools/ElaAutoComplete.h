#ifndef ELAAUTOCOMPLETE_H
#define ELAAUTOCOMPLETE_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaAutoCompletePrivate;
class ELA_EXPORT ElaAutoComplete : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaAutoComplete)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
    enum MatchMode
    {
        Contains = 0,
        StartsWith,
        EndsWith,
        RegExp,
    };
    Q_ENUM(MatchMode)

    explicit ElaAutoComplete(QWidget* parent = nullptr);
    ~ElaAutoComplete() override;

    void setPlaceholderText(const QString& placeholderText);
    QString placeholderText() const;

    void setText(const QString& text);
    QString text() const;

    void setCompletions(const QStringList& completions);
    QStringList completions() const;

    void setMatchMode(MatchMode mode);
    MatchMode matchMode() const;

    void setFixedHeight(int h);

Q_SIGNALS:
    Q_SIGNAL void textChanged(const QString& text);
    Q_SIGNAL void textEdited(const QString& text);
    Q_SIGNAL void completionSelected(const QString& completion);
    Q_SIGNAL void returnPressed(const QString& text);
};

#endif // ELAAUTOCOMPLETE_H
