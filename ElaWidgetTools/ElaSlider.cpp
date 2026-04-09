#include "ElaSlider.h"

#include <QEvent>

#include "DeveloperComponents/ElaSliderStyle.h"
ElaSlider::ElaSlider(QWidget* parent)
    : QSlider(parent)
{
#ifdef Q_OS_MACOS
    setAttribute(Qt::WA_Hover);
#endif
    setOrientation(Qt::Horizontal);
    setStyle(new ElaSliderStyle(style()));
}

ElaSlider::ElaSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
#ifdef Q_OS_MACOS
    setAttribute(Qt::WA_Hover);
#endif
    setStyle(new ElaSliderStyle(style()));
}

ElaSlider::~ElaSlider()
{
    delete this->style();
}
