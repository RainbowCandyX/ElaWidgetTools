#ifndef ELAMULTISELECTCOMBOBOXDELEGATE_H
#define ELAMULTISELECTCOMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

#include "ElaDef.h"

class ElaMultiSelectComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ElaMultiSelectComboBoxDelegate(QObject* parent = nullptr);
    ~ElaMultiSelectComboBoxDelegate();

    void setItemSelection(const QVector<bool>& selection);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QVector<bool> _itemSelection;
};

#endif // ELAMULTISELECTCOMBOBOXDELEGATE_H
