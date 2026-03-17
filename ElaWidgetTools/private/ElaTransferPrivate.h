#ifndef ELATRANSFERPRIVATE_H
#define ELATRANSFERPRIVATE_H

#include <QObject>
#include <QStringList>

#include "ElaDef.h"
class QHBoxLayout;
class QVBoxLayout;
class ElaCheckBox;
class ElaLineEdit;
class ElaIconButton;
class ElaText;
class ElaBaseListView;
class QStandardItemModel;
class ElaTransfer;
class ElaTransferPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTransfer)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(QString, SourceTitle)
    Q_PROPERTY_CREATE_D(QString, TargetTitle)
    Q_PROPERTY_CREATE_D(bool, IsSearchVisible)
public:
    explicit ElaTransferPrivate(QObject* parent = nullptr);
    ~ElaTransferPrivate() override;

    Q_SLOT void onMoveToTarget();
    Q_SLOT void onMoveToSource();
    Q_SLOT void onSourceSearchTextChanged(const QString& text);
    Q_SLOT void onTargetSearchTextChanged(const QString& text);
    Q_SLOT void onSourceSelectAllChanged(int state);
    Q_SLOT void onTargetSelectAllChanged(int state);

    void _refreshCountText();

private:
    ElaThemeType::ThemeMode _themeMode;

    ElaCheckBox* _sourceSelectAll{nullptr};
    ElaText* _sourceTitleText{nullptr};
    ElaText* _sourceCountText{nullptr};
    ElaLineEdit* _sourceSearchEdit{nullptr};
    ElaBaseListView* _sourceListView{nullptr};
    QStandardItemModel* _sourceModel{nullptr};

    ElaCheckBox* _targetSelectAll{nullptr};
    ElaText* _targetTitleText{nullptr};
    ElaText* _targetCountText{nullptr};
    ElaLineEdit* _targetSearchEdit{nullptr};
    ElaBaseListView* _targetListView{nullptr};
    QStandardItemModel* _targetModel{nullptr};

    ElaIconButton* _moveToTargetButton{nullptr};
    ElaIconButton* _moveToSourceButton{nullptr};

    friend class ElaTransfer;
};

#endif // ELATRANSFERPRIVATE_H
