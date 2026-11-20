#ifndef ELAMESSAGEBOX_H
#define ELAMESSAGEBOX_H

#include <QColor>
#include <QDialog>
#include <QStringList>

#include "ElaAppBar.h"
#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMessageBoxPrivate;
class ELA_EXPORT ElaMessageBox : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageBox)
    Q_TAKEOVER_NATIVEEVENT_H
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Text)
    Q_PROPERTY_CREATE_Q_H(QString, InformationText)
    Q_PROPERTY_CREATE_Q_H(QColor, IconColor)
public:
    explicit ElaMessageBox(QWidget* parent = nullptr);
    ~ElaMessageBox() override;

    void setIcon(ElaIconType::IconName icon);
    ElaIconType::IconName getIcon() const;

    void setButtons(const QStringList& buttons);
    QStringList getButtons() const;

    void setDefaultButtonIndex(int index);
    int getDefaultButtonIndex() const;

    int getClickedButtonIndex() const;

    static int information(QWidget* parent, const QString& title, const QString& text,
                           const QStringList& buttons = {QStringLiteral("确定")},
                           int defaultButtonIndex = -1);
    static int warning(QWidget* parent, const QString& title, const QString& text,
                       const QStringList& buttons = {QStringLiteral("确定")},
                       int defaultButtonIndex = -1);
    static int critical(QWidget* parent, const QString& title, const QString& text,
                        const QStringList& buttons = {QStringLiteral("确定")},
                        int defaultButtonIndex = -1);
    static int question(QWidget* parent, const QString& title, const QString& text,
                        const QStringList& buttons = {QStringLiteral("确定")},
                        int defaultButtonIndex = -1);
    static void about(QWidget* parent, const QString& title, const QString& text);

Q_SIGNALS:
    Q_SIGNAL void buttonClicked(int index);

public Q_SLOTS:
    void reject() override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
};

#endif // ELAMESSAGEBOX_H
