#include "ElaItemViewToolTipPrivate.h"

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QCursor>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTimer>

#include "ElaItemViewToolTip.h"
#include "ElaText.h"

ElaItemViewToolTipPrivate::ElaItemViewToolTipPrivate(QObject* parent)
    : QObject{parent}
{
    _showTimer = new QTimer(this);
    _showTimer->setSingleShot(true);
    _displayTimer = new QTimer(this);
    _displayTimer->setSingleShot(true);
    _hideTimer = new QTimer(this);
    _hideTimer->setSingleShot(true);

    connect(_showTimer, &QTimer::timeout, this, [this]() {
        if (_viewport.isNull() || !_viewport->underMouse())
        {
            return;
        }
        _doShowAnimation();
        if (_pDisplayMsec > -1)
        {
            _displayTimer->start(_pDisplayMsec);
        }
        });
    connect(_displayTimer, &QTimer::timeout, this, [this]() {
        Q_Q(ElaItemViewToolTip);
        _suppressed = true;   // 到点自动隐藏后，光标不换行就不再重弹
        q->hide();
        });
    connect(_hideTimer, &QTimer::timeout, this, [this]() {
        Q_Q(ElaItemViewToolTip);
        q->hide();
        });
}

ElaItemViewToolTipPrivate::~ElaItemViewToolTipPrivate()
{
}

bool ElaItemViewToolTipPrivate::eventFilter(QObject* watched, QEvent* event)
{
    Q_Q(ElaItemViewToolTip);
    if (_viewport.isNull() || watched != _viewport.data())
    {
        return QObject::eventFilter(watched, event);
    }

    switch (event->type())
    {
    case QEvent::ToolTip:
        return true;   // 吞掉原生 tooltip，改由 MouseMove 驱动
    case QEvent::MouseMove:
    {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        const QModelIndex index = _view->indexAt(mouseEvent->pos());
        const QString text = (index.isValid() && index.model() != nullptr)
            ? index.model()->data(index, Qt::ToolTipRole).toString()
            : QString();

        if (index != _hoverIndex)
        {
            _hoverIndex = index;
            _suppressed = false;
            _showTimer->stop();   // 换行后重新计时
        }

        if (text.isEmpty())
        {
            _showTimer->stop();
            if (q->isVisible())
            {
                _hideTimer->start(qMax(0, _pHideDelayMsec));
            }
            break;
        }

        _hideTimer->stop();   // 回到有文字的行，取消待隐藏

        if (q->isVisible())
        {
            // 已显示：文字变化则更新内容（不重播淡入），并跟随光标更新位置
            if (text != _pToolTip)
            {
                q->setToolTip(text);
            }
            // 移动后重新计时
            if (_pDisplayMsec > -1)
            {
                _displayTimer->start(_pDisplayMsec);
            }
            _updatePos();
        }
        else if (!_suppressed && !_showTimer->isActive())
        {
            q->setToolTip(text);
            _showTimer->start(qMax(0, _pShowDelayMsec));
        }
        break;
    }
    case QEvent::Leave:
        _showTimer->stop();
        _displayTimer->stop();
        if (q->isVisible())
        {
           q->hide();
        }
        break;
    case QEvent::Hide:
        q->hide();
        break;
    default:
        break;
    }
    return QObject::eventFilter(watched, event);
}

void ElaItemViewToolTipPrivate::_doShowAnimation()
{
    Q_Q(ElaItemViewToolTip);
    QPoint cursorPoint = QCursor::pos();
    q->move(cursorPoint.x() + 10, cursorPoint.y());
    q->setWindowOpacity(0.0);
    q->show();

    auto* showAnimation = new QPropertyAnimation(q, "windowOpacity", q);
    showAnimation->setEasingCurve(QEasingCurve::InOutSine);
    showAnimation->setDuration(250);
    showAnimation->setStartValue(0.0);
    showAnimation->setEndValue(1.0);
    showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ElaItemViewToolTipPrivate::_updatePos()
{
    Q_Q(ElaItemViewToolTip);
    if (q->isVisible())
    {
        QPoint cursorPoint = QCursor::pos();
        q->move(cursorPoint.x() + 10, cursorPoint.y());
    }
}
