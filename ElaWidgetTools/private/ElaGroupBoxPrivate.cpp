#include "ElaGroupBoxPrivate.h"

#include "ElaGroupBox.h"
#include "ElaTheme.h"

ElaGroupBoxPrivate::ElaGroupBoxPrivate(QObject* parent)
    : QObject(parent)
{
    _pBorderRadius = 6;
}

ElaGroupBoxPrivate::~ElaGroupBoxPrivate()
{
}

void ElaGroupBoxPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
    Q_Q(ElaGroupBox);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, ElaThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}
