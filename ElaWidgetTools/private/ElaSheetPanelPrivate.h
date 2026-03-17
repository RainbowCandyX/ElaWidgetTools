#ifndef ELASHEETPANELPRIVATE_H
#define ELASHEETPANELPRIVATE_H

#include <QObject>
#include <QPoint>

#include "ElaDef.h"
class QVBoxLayout;
class QPropertyAnimation;
class ElaMaskWidget;
class ElaSheetPanel;
class ElaSheetPanelPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaSheetPanel)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(ElaSheetPanelType::Direction, Direction)
    Q_PROPERTY_CREATE_D(qreal, PeekRatio)
    Q_PROPERTY_CREATE_D(qreal, HalfRatio)
    Q_PROPERTY_CREATE_D(qreal, FullRatio)
    Q_PROPERTY_CREATE_D(bool, DragHandleVisible)
    Q_PROPERTY_CREATE_D(bool, CloseOnOverlayClick)
    Q_PROPERTY_CREATE_D(qreal, OverlayOpacity)
public:
    explicit ElaSheetPanelPrivate(QObject* parent = nullptr);
    ~ElaSheetPanelPrivate() override;

    void _doOpenAnimation(ElaSheetPanelType::DetentLevel level);
    void _doCloseAnimation();
    void _updatePanelGeometry(int targetSize, bool animate = true);
    int _getTargetSize(ElaSheetPanelType::DetentLevel level) const;
    ElaSheetPanelType::DetentLevel _getNearestDetent(int currentSize) const;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaMaskWidget* _maskWidget{nullptr};
    QWidget* _panelWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QWidget* _dragHandle{nullptr};
    QVBoxLayout* _panelLayout{nullptr};
    ElaSheetPanelType::DetentLevel _currentDetent{ElaSheetPanelType::Half};
    bool _isOpened{false};
    bool _isDragging{false};
    QPoint _dragStartPos;
    int _dragStartSize{0};

    friend class ElaSheetPanel;
};

#endif // ELASHEETPANELPRIVATE_H
