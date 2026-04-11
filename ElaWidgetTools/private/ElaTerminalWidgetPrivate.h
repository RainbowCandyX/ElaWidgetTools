#ifndef ELATERMINALWIDGETPRIVATE_H
#define ELATERMINALWIDGETPRIVATE_H

#include <QObject>
#include <QStringList>

#include "ElaDef.h"
#include "ElaProperty.h"

class QPlainTextEdit;
class QLineEdit;
class QLabel;
class ElaTerminalWidget;
class ElaTerminalWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTerminalWidget)
    Q_PROPERTY_CREATE_D(QString, Prompt)
    Q_PROPERTY_CREATE_D(int, MaxHistorySize)
    Q_PROPERTY_CREATE_D(int, MaxLineCount)
    Q_PROPERTY_CREATE_D(int, FontPixelSize)
public:
    explicit ElaTerminalWidgetPrivate(QObject* parent = nullptr);
    ~ElaTerminalWidgetPrivate() override;

    void submitCommand();
    void navigateHistory(int direction);
    void updateFont();
    void trimOutput();

private:
    ElaThemeType::ThemeMode _themeMode;
    QPlainTextEdit* _outputArea{nullptr};
    QLineEdit* _inputLine{nullptr};
    QLabel* _promptLabel{nullptr};
    QStringList _commandHistory;
    int _historyIndex{-1};
    QString _savedInput;
};

#endif // ELATERMINALWIDGETPRIVATE_H
