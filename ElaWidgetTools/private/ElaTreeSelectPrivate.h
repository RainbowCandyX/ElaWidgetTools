#ifndef ELATREESELECTPRIVATE_H
#define ELATREESELECTPRIVATE_H

#include <QModelIndex>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>

#include "ElaDef.h"

class QStandardItemModel;
class ElaTreeSelect;
class ElaTreeView;
class ElaLineEdit;
class QSortFilterProxyModel;

class ElaTreeSelectPopupWidget : public QWidget
{
    Q_OBJECT
public:
    ElaTreeSelectPopupWidget(QWidget* parent = nullptr);
    ElaThemeType::ThemeMode _themeMode;
    int _borderRadius{6};

protected:
    void paintEvent(QPaintEvent* event) override;
};

class ElaTreeSelectPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTreeSelect)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(QString, PlaceholderText)
    Q_PROPERTY_CREATE_D(bool, IsSearchVisible)
    Q_PROPERTY_CREATE_D(bool, IsEditable)

public:
    explicit ElaTreeSelectPrivate(QObject* parent = nullptr);
    ~ElaTreeSelectPrivate() override;

    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);
    Q_SLOT void onItemClicked(const QModelIndex& index);
    Q_SLOT void onSearchTextChanged(const QString& text);
    Q_SLOT void onTreeExpanded(const QModelIndex& index);
    Q_SLOT void onTreeCollapsed(const QModelIndex& index);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void _showPopup();
    void _hidePopup();
    void _updateDisplayText();
    void _updatePopupHeight();
    int _calculatePopupHeight() const;
    int _countVisibleItems(const QModelIndex& parent = QModelIndex()) const;

    bool _isHover{false};
    bool _isPopupVisible{false};
    bool _isPressed{false};
    qreal _expandIconRotate{0};
    QString _currentText;
    QModelIndex _currentIndex;
    QStandardItemModel* _model{nullptr};
    QSortFilterProxyModel* _proxyModel{nullptr};
    ElaTreeView* _treeView{nullptr};
    ElaLineEdit* _searchEdit{nullptr};
    ElaTreeSelectPopupWidget* _popupContainer{nullptr};
    ElaThemeType::ThemeMode _themeMode;

    friend class ElaTreeSelect;
};

#endif // ELATREESELECTPRIVATE_H
