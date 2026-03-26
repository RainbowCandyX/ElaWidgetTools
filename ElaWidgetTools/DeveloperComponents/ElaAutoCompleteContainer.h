#ifndef ELAAUTOCOMPLETECONTAINER_H
#define ELAAUTOCOMPLETECONTAINER_H

#include <QWidget>

#include "ElaDef.h"

class ElaLineEdit;
class ElaAutoCompleteContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ElaAutoCompleteContainer(QWidget* parent = nullptr);
    ~ElaAutoCompleteContainer();

    void setForwardTarget(ElaLineEdit* target);

Q_SIGNALS:
    Q_SIGNAL void popupClosed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    ElaThemeType::ThemeMode _themeMode;
    ElaLineEdit* _forwardTarget{nullptr};
};

#endif // ELAAUTOCOMPLETECONTAINER_H
