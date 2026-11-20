#include "ElaLoadingMask.h"

#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QWheelEvent>

#include "ElaLoadingMaskPrivate.h"
#include "ElaProgressRing.h"
#include "ElaText.h"
#include "ElaTheme.h"

ElaLoadingMask::ElaLoadingMask(QWidget* parent)
    : QWidget(parent), d_ptr(new ElaLoadingMaskPrivate())
{
    Q_D(ElaLoadingMask);
    d->q_ptr = this;
    d->_target = parent ? parent->window() : nullptr;
    d->_pText = "加载中...";
    d->_pMaskAlpha = 90;
    d->_pAnimationDuration = 250;

    setObjectName("ElaLoadingMask");
    setAttribute(Qt::WA_StyledBackground, false);
    setFocusPolicy(Qt::NoFocus);
    setParent(d->_target);

    d->_progressRing = new ElaProgressRing(this);
    d->_progressRing->setIsTransparent(true);
    d->_progressRing->setIsDisplayValue(false);

    d->_textLabel = new ElaText(d->_pText, this);
    d->_textLabel->setTextPixelSize(14);

    d->_contentWidget = new QWidget(this);
    d->_contentWidget->setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout* contentLayout = new QVBoxLayout(d->_contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(12);
    contentLayout->addWidget(d->_progressRing, 0, Qt::AlignHCenter);
    contentLayout->addWidget(d->_textLabel, 0, Qt::AlignHCenter);

    d->_contentOpacityEffect = new QGraphicsOpacityEffect(d->_contentWidget);
    d->_contentOpacityEffect->setOpacity(0);
    d->_contentWidget->setGraphicsEffect(d->_contentOpacityEffect);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addWidget(d->_contentWidget, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    d->_maskAlphaAnimation = new QPropertyAnimation(d, "pCurrentMaskAlpha", d);
    d->_maskAlphaAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->_maskAlphaAnimation, &QPropertyAnimation::valueChanged, this, [this, d]() {
        const qreal progress = d->_pMaskAlpha > 0
                                   ? qBound(0.0, qreal(d->_pCurrentMaskAlpha) / d->_pMaskAlpha, 1.0)
                                   : 0.0;
        d->_contentOpacityEffect->setOpacity(progress);
        update();
    });
    connect(d->_maskAlphaAnimation, &QPropertyAnimation::finished, this, [this, d]() {
        if (!d->_isFinishing)
        {
            return;
        }
        d->_isFinishing = false;
        d->_isRunning = false;
        hide();
        d->_progressRing->setIsBusying(false);
        deleteLater();
    });

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this, d](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });

    if (d->_target)
    {
        d->_target->installEventFilter(this);
        setGeometry(d->_target->rect());
    }
    hide();
}

ElaLoadingMask::~ElaLoadingMask()
{
    Q_D(ElaLoadingMask);
    if (d->_target)
    {
        d->_target->removeEventFilter(this);
    }
}

void ElaLoadingMask::setText(QString text)
{
    Q_D(ElaLoadingMask);
    d->_pText = std::move(text);
    if (d->_textLabel)
    {
        d->_textLabel->setText(d->_pText);
    }
    Q_EMIT pTextChanged();
}

QString ElaLoadingMask::getText() const
{
    Q_D(const ElaLoadingMask);
    return d->_pText;
}

void ElaLoadingMask::setMaskAlpha(int maskAlpha)
{
    Q_D(ElaLoadingMask);
    d->_pMaskAlpha = qBound(0, maskAlpha, 255);
    Q_EMIT pMaskAlphaChanged();
}

int ElaLoadingMask::getMaskAlpha() const
{
    Q_D(const ElaLoadingMask);
    return d->_pMaskAlpha;
}

void ElaLoadingMask::setAnimationDuration(int durationMs)
{
    Q_D(ElaLoadingMask);
    d->_pAnimationDuration = qMax(0, durationMs);
    d->_maskAlphaAnimation->setDuration(d->_pAnimationDuration);
    Q_EMIT pAnimationDurationChanged();
}

int ElaLoadingMask::getAnimationDuration() const
{
    Q_D(const ElaLoadingMask);
    return d->_pAnimationDuration;
}

void ElaLoadingMask::start()
{
    Q_D(ElaLoadingMask);
    if (d->_isRunning)
    {
        return;
    }
    if (d->_target)
    {
        setGeometry(d->_target->rect());
    }
    d->_isRunning = true;
    d->_isFinishing = false;
    show();
    raise();
    d->_progressRing->setIsBusying(true);
    d->_fadeTo(d->_pMaskAlpha);
}

void ElaLoadingMask::finish()
{
    Q_D(ElaLoadingMask);
    if (!d->_isRunning || d->_isFinishing)
    {
        return;
    }
    d->_isFinishing = true;
    d->_fadeTo(0);
}

bool ElaLoadingMask::isRunning() const
{
    Q_D(const ElaLoadingMask);
    return d->_isRunning;
}

void ElaLoadingMask::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(ElaLoadingMask);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // 遮罩层
    painter.setBrush(QColor(0, 0, 0, d->_pCurrentMaskAlpha));
    painter.drawRect(rect());

    // ring + label 背后的主题背景，透明度与遮罩进度同步
    const qreal progress = d->_pMaskAlpha > 0
                               ? qBound(0.0, qreal(d->_pCurrentMaskAlpha) / d->_pMaskAlpha, 1.0)
                               : 0.0;
    if (d->_contentWidget && progress > 0)
    {
        QColor bgColor = ElaThemeColor(d->_themeMode, BasicBaseAlpha);
        bgColor.setAlphaF(bgColor.alphaF() * progress);
        painter.setBrush(bgColor);
        const QRect bgRect = d->_contentWidget->geometry().adjusted(-20, -16, 20, 16);
        painter.drawRoundedRect(bgRect, 8, 8);
    }
}

bool ElaLoadingMask::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaLoadingMask);
    if (watched == d->_target && event->type() == QEvent::Resize)
    {
        setGeometry(d->_target->rect());
    }
    return QWidget::eventFilter(watched, event);
}

void ElaLoadingMask::mousePressEvent(QMouseEvent* event)
{
    event->accept();
}

void ElaLoadingMask::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();
}

void ElaLoadingMask::wheelEvent(QWheelEvent* event)
{
    event->accept();
}
