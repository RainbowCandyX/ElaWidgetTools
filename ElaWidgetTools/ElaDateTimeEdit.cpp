#include "ElaDateTimeEdit.h"

#include "DeveloperComponents/ElaDateTimeEditStyle.h"
#include "ElaDateTimeEditPrivate.h"
#include "ElaMenu.h"
#include "ElaTheme.h"

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>
#include <QPropertyAnimation>
ElaDateTimeEdit::ElaDateTimeEdit(QWidget* parent)
    : QDateTimeEdit(parent), d_ptr(new ElaDateTimeEditPrivate())
{
    Q_D(ElaDateTimeEdit);
    d->q_ptr = this;
    d->_pExpandMarkWidth = 0;
    setFixedHeight(35);
    setMinimumWidth(210);
#ifdef Q_OS_MACOS
    setAttribute(Qt::WA_Hover);
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    setStyle(new ElaDateTimeEditStyle(style()));
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
#ifdef Q_OS_MACOS
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;border:none;outline:none;");
#else
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
#endif
    d->onThemeChanged(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaDateTimeEditPrivate::onThemeChanged);
}

ElaDateTimeEdit::ElaDateTimeEdit(const QDateTime& dateTime, QWidget* parent)
    : ElaDateTimeEdit(parent)
{
    setDateTime(dateTime);
}

ElaDateTimeEdit::~ElaDateTimeEdit()
{
}

void ElaDateTimeEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(ElaDateTimeEdit);
    if (event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QDateTimeEdit::focusInEvent(event);
}

void ElaDateTimeEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(ElaDateTimeEdit);
    if (event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QDateTimeEdit::focusOutEvent(event);
}

void ElaDateTimeEdit::paintEvent(QPaintEvent* event)
{
    Q_D(ElaDateTimeEdit);
    if (palette().color(QPalette::Text) != ElaThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QDateTimeEdit::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
    painter.restore();
}

void ElaDateTimeEdit::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(ElaDateTimeEdit);
    ElaMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addElaIconAction(ElaIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addElaIconAction(ElaIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
        ? event->globalPos()
        : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
