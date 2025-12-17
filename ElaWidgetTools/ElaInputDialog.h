#ifndef ELAINPUTDIALOG_H
#define ELAINPUTDIALOG_H

#include <QAbstractNativeEventFilter>
#include <QDialog>
#include <QLineEdit>

#include "ElaAppBar.h"
#include "ElaProperty.h"

class ElaInputDialogPrivate;
class ELA_EXPORT ElaInputDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaInputDialog)
    Q_TAKEOVER_NATIVEEVENT_H
    Q_PROPERTY_CREATE_Q_H(QString, TitleText)
    Q_PROPERTY_CREATE_Q_H(QString, SubTitleText)
    Q_PROPERTY_CREATE_Q_H(QString, LabelText)
    Q_PROPERTY_CREATE_Q_H(QString, TextValue)
    Q_PROPERTY_CREATE_Q_H(int, IntValue)
    Q_PROPERTY_CREATE_Q_H(double, DoubleValue)
    Q_PROPERTY_CREATE_Q_H(QString, OkButtonText)
    Q_PROPERTY_CREATE_Q_H(QString, CancelButtonText)
    Q_PROPERTY_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(int, InputMinimumWidth)
    Q_PROPERTY_CREATE_Q_H(int, InputMaximumWidth)

public:
    explicit ElaInputDialog(QWidget* parent = nullptr);
    ~ElaInputDialog() override;

    static QString getText(QWidget* parent, const QString& title,
                          const QString& subtitle, const QString& label,
                          const QString& text = QString(),
                          bool* ok = nullptr, const QString& okButtonText = "确定",
                          const QString& cancelButtonText = "取消",
                          int inputMinWidth = 50, int inputMaxWidth = QWIDGETSIZE_MAX);

    static int getInt(QWidget* parent, const QString& title,
                     const QString& subtitle, const QString& label,
                     int value = 0,
                     int minValue = -2147483647, int maxValue = 2147483647,
                     int step = 1, bool* ok = nullptr,
                     const QString& okButtonText = "确定",
                     const QString& cancelButtonText = "取消",
                     int inputMinWidth = 50, int inputMaxWidth = QWIDGETSIZE_MAX);

    static double getDouble(QWidget* parent, const QString& title,
                           const QString& subtitle, const QString& label,
                           double value = 0.0,
                           double minValue = -2147483647.0, double maxValue = 2147483647.0,
                           int decimals = 2, bool* ok = nullptr,
                           const QString& okButtonText = "确定",
                           const QString& cancelButtonText = "取消",
                           int inputMinWidth = 50, int inputMaxWidth = QWIDGETSIZE_MAX);

    static QString getMultiLineText(QWidget* parent, const QString& title,
                                   const QString& subtitle, const QString& label,
                                   const QString& text = QString(),
                                   bool* ok = nullptr, const QString& okButtonText = "确定",
                                   const QString& cancelButtonText = "取消",
                                   int inputMinWidth = 50, int inputMaxWidth = QWIDGETSIZE_MAX);

    void setTextEchoMode(QLineEdit::EchoMode mode);
    QLineEdit::EchoMode textEchoMode() const;

    void setIntRange(int minValue, int maxValue, int step = 1);
    void setDoubleRange(double minValue, double maxValue, int decimals = 2);
    void setMultiLine(bool multiLine);

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

Q_SIGNALS:
    Q_SIGNAL void textValueChanged(QString text);
    Q_SIGNAL void intValueChanged(int value);
    Q_SIGNAL void doubleValueChanged(double value);

private:
    Q_SLOT void onOkButtonClicked();
    Q_SLOT void onCancelButtonClicked();
    void updateLabels();
};

#endif // ELAINPUTDIALOG_H
