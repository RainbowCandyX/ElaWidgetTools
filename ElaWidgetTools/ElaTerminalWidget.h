#ifndef ELATERMINALWIDGET_H
#define ELATERMINALWIDGET_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaTerminalWidgetPrivate;
class ELA_EXPORT ElaTerminalWidget : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaTerminalWidget)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Prompt)
    Q_PROPERTY_CREATE_Q_H(int, MaxHistorySize)
    Q_PROPERTY_CREATE_Q_H(int, MaxLineCount)
    Q_PROPERTY_CREATE_Q_H(int, FontPixelSize)
public:
    explicit ElaTerminalWidget(QWidget* parent = nullptr);
    ~ElaTerminalWidget() override;

    void appendOutput(const QString& text, const QColor& color = QColor());
    void appendHtml(const QString& html);
    void appendError(const QString& text);
    void appendSuccess(const QString& text);
    void clear();

    QStringList getCommandHistory() const;

Q_SIGNALS:
    Q_SIGNAL void commandSubmitted(const QString& command);
    Q_SIGNAL void tabPressed(const QString& currentInput);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // ELATERMINALWIDGET_H
