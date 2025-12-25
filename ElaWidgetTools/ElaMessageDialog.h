#ifndef ELAMESSAGEDIALOG_H
#define ELAMESSAGEDIALOG_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaMessageDialogPrivate;
class ELA_EXPORT ElaMessageDialog : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaMessageDialog)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(int, TitlePixelSize)
    Q_PROPERTY_CREATE_Q_H(int, ContentPixelSize)

public:
    explicit ElaMessageDialog(QWidget* parent = nullptr);
    ~ElaMessageDialog();

Q_SIGNALS:
    void confirmed();
    void cancelled();

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // ELAMESSAGEDIALOG_H
