#ifndef ELAUTOCOMPLETEDELEGATE_H
#define ELAUTOCOMPLETEDELEGATE_H

#include <QStyledItemDelegate>

#include "ElaDef.h"
#include "ElaAutoComplete.h"

class ElaAutoCompleteDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaAutoCompleteDelegate(QObject* parent = nullptr);
    ~ElaAutoCompleteDelegate();

    void setHighlightText(const QString& text);
    void setMatchMode(ElaAutoComplete::MatchMode mode);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QString _highlightText;
    ElaAutoComplete::MatchMode _matchMode{ElaAutoComplete::Contains};
};

#endif // ELAUTOCOMPLETEDELEGATE_H
