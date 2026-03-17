#ifndef ELASHEETPANEL_H
#define ELASHEETPANEL_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaSheetPanelPrivate;
class ELA_EXPORT ElaSheetPanel : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaSheetPanel)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(ElaSheetPanelType::Direction, Direction)
    Q_PROPERTY_CREATE_Q_H(qreal, PeekRatio)
    Q_PROPERTY_CREATE_Q_H(qreal, HalfRatio)
    Q_PROPERTY_CREATE_Q_H(qreal, FullRatio)
    Q_PROPERTY_CREATE_Q_H(bool, DragHandleVisible)
    Q_PROPERTY_CREATE_Q_H(bool, CloseOnOverlayClick)
    Q_PROPERTY_CREATE_Q_H(qreal, OverlayOpacity)
public:
    explicit ElaSheetPanel(QWidget* parent = nullptr);
    ~ElaSheetPanel() override;

    void setCentralWidget(QWidget* widget);

    void open(ElaSheetPanelType::DetentLevel level = ElaSheetPanelType::Half);
    void close();

    ElaSheetPanelType::DetentLevel currentDetent() const;
    bool isOpened() const;

Q_SIGNALS:
    Q_SIGNAL void opened();
    Q_SIGNAL void closed();
    Q_SIGNAL void detentChanged(ElaSheetPanelType::DetentLevel level);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELASHEETPANEL_H
