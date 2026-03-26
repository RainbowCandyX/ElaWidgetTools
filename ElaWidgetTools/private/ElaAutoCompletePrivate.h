#ifndef ELAUTOCOMPLETEPRIVATE_H
#define ELAUTOCOMPLETEPRIVATE_H

#include <QObject>
#include <QStringList>

#include "ElaDef.h"
#include "ElaAutoComplete.h"

class ElaLineEdit;
class ElaBaseListView;
class QVBoxLayout;
class QStringListModel;
class QSortFilterProxyModel;
class ElaAutoCompleteContainer;
class ElaAutoCompletePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaAutoComplete)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit ElaAutoCompletePrivate(QObject* parent = nullptr);
    ~ElaAutoCompletePrivate();

    Q_SLOT void onThemeModeChanged(ElaThemeType::ThemeMode themeMode);
    Q_SLOT void onTextEdited(const QString& text);
    Q_SLOT void onItemClicked(const QModelIndex& index);

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaLineEdit* _lineEdit{nullptr};
    ElaAutoCompleteContainer* _popup{nullptr};
    ElaBaseListView* _listView{nullptr};
    QVBoxLayout* _popupLayout{nullptr};
    QStringListModel* _sourceModel{nullptr};
    QSortFilterProxyModel* _filterModel{nullptr};
    QStringList _completions;
    ElaAutoComplete::MatchMode _matchMode{ElaAutoComplete::Contains};
    bool _isAllowHidePopup{false};
    bool _isPopupVisible{false};

    void _showPopup();
    void _hidePopup();
    void _updateFilter(const QString& text);
};

#endif // ELAUTOCOMPLETEPRIVATE_H
