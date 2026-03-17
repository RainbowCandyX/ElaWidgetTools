#ifndef ELATAGPRIVATE_H
#define ELATAGPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaTag.h"
class ElaTagPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaTag)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, TagText)
    Q_PROPERTY_CREATE_D(bool, IsClosable)
    Q_PROPERTY_CREATE_D(bool, IsCheckable)
    Q_PROPERTY_CREATE_D(bool, IsChecked)
public:
    explicit ElaTagPrivate(QObject* parent = nullptr);
    ~ElaTagPrivate() override;

private:
    ElaTag::TagColor _tagColor{ElaTag::Default};
    ElaThemeType::ThemeMode _themeMode;
    int _closeButtonWidth{16};
    int _checkIconWidth{16};
    int _padding{8};
    QColor _getBackgroundColor() const;
    QColor _getForegroundColor() const;
};

#endif // ELATAGPRIVATE_H
