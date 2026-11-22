#include "ElaTextPrivate.h"

#include "ElaText.h"
#include "ElaTheme.h"
#include <QTimer>
ElaTextPrivate::ElaTextPrivate(QObject* parent)
    : QObject{parent}
{
}

ElaTextPrivate::~ElaTextPrivate()
{
}

void ElaTextPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaText);
    _themeMode = themeMode;
    const QColor textColor = _pTextColor.isValid() ? _pTextColor : ElaThemeColor(_themeMode, BasicText);
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, textColor);
    palette.setColor(QPalette::WindowText, textColor);
    q->setPalette(palette);
}
