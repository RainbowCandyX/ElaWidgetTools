#include "ElaGroupBox.h"

#include <QLayout>

#include "DeveloperComponents/ElaGroupBoxStyle.h"
#include "ElaTheme.h"
#include "private/ElaGroupBoxPrivate.h"

ElaGroupBox::ElaGroupBox(QWidget* parent)
    : QGroupBox(parent), d_ptr(new ElaGroupBoxPrivate())
{
    Q_D(ElaGroupBox);
    d->q_ptr = this;

    setMouseTracking(true);
    setObjectName("ElaGroupBox");

    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);

    setStyle(new ElaGroupBoxStyle(style()));
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaGroupBoxPrivate::onThemeChanged);
}

ElaGroupBox::ElaGroupBox(const QString& title, QWidget* parent)
    : ElaGroupBox(parent)
{
    setTitle(title);
}

ElaGroupBox::~ElaGroupBox()
{
    delete this->style();
}

void ElaGroupBox::paintEvent(QPaintEvent* event)
{
    Q_D(ElaGroupBox);
    if (palette().color(QPalette::WindowText) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QGroupBox::paintEvent(event);
}

Q_PROPERTY_CREATE_Q_CPP(ElaGroupBox, int, BorderRadius)
