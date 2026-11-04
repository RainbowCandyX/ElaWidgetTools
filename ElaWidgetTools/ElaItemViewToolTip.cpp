#include "ElaItemViewToolTip.h"

#include <QAbstractItemView>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>

#include "ElaText.h"
#include "ElaTheme.h"
#include "private/ElaItemViewToolTipPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaItemViewToolTip, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaItemViewToolTip, int, DisplayMsec)
Q_PROPERTY_CREATE_Q_CPP(ElaItemViewToolTip, int, ShowDelayMsec)
Q_PROPERTY_CREATE_Q_CPP(ElaItemViewToolTip, int, HideDelayMsec)

ElaItemViewToolTip::ElaItemViewToolTip(QAbstractItemView* view)
    : QWidget{view}, d_ptr(new ElaItemViewToolTipPrivate())
{
    Q_D(ElaItemViewToolTip);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    d->_pDisplayMsec = -1;
    d->_pShowDelayMsec = 0;
    d->_pHideDelayMsec = 0;
    d->_pDisplayMsec = 3000;

    setObjectName("ElaItemViewToolTip");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setFocusPolicy(Qt::NoFocus);
    setStyle(nullptr); // 隔绝style

    d->_toolTipText = new ElaText(this);
    d->_toolTipText->setTextPixelSize(17);
    d->_toolTipText->setWordWrap(true);

    d->_mainLayout = new QVBoxLayout(this);
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    d->_mainLayout->setContentsMargins(d->_shadowBorderWidth, d->_shadowBorderWidth, d->_shadowBorderWidth, d->_shadowBorderWidth);
#else
    d->_mainLayout->setContentsMargins(d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2, d->_shadowBorderWidth * 2);
#endif
    d->_mainLayout->setSpacing(0);
    d->_mainLayout->addWidget(d->_toolTipText);


    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [this, d](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });

    if (view != nullptr)
    {
        d->_view = view;
        d->_viewport = view->viewport();
        if (!d->_viewport.isNull())
        {
            d->_viewport->setMouseTracking(true);   // 保证能收到 MouseMove
            d->_viewport->installEventFilter(d);
        }
    }

    hide();
}

ElaItemViewToolTip::~ElaItemViewToolTip()
{
    Q_D(ElaItemViewToolTip);
    if (!d->_viewport.isNull())
    {
        d->_viewport->removeEventFilter(d);
    }
}

void ElaItemViewToolTip::updatePos()
{
    Q_D(ElaItemViewToolTip);
    d->_updatePos();
}

void ElaItemViewToolTip::paintEvent(QPaintEvent* event)
{
    Q_D(ElaItemViewToolTip);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    QRect foregroundRect = rect();
#else
    //阴影
    eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    QRect foregroundRect = rect();
    foregroundRect.adjust(d->_shadowBorderWidth, d->_shadowBorderWidth, -d->_shadowBorderWidth, -d->_shadowBorderWidth);
#endif
    painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
    painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
    QWidget::paintEvent(event);
}

void ElaItemViewToolTip::setToolTip(QString toolTip)
{
    Q_D(ElaItemViewToolTip);
    d->_pToolTip = toolTip;
    ElaText* text = d->_toolTipText;
    text->setText(toolTip);

    // 自适应宽度：单行自然宽，超过上限换行（保留富文本渲染）
    const int shadow = d->_shadowBorderWidth;
    text->setMinimumSize(0, 0);
    text->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    text->setWordWrap(false);
    const int naturalWidth = text->sizeHint().width();
    const int maxContent = qMax(120, 400 - shadow * 4);
    const bool wrapped = naturalWidth > maxContent;
    const int width = qMin(naturalWidth, maxContent);

    text->setWordWrap(wrapped);
    text->setFixedWidth(width);
    int height = 0;
    if (wrapped)
    {
        height = text->heightForWidth(width);
    }
    if (height <= 0)
    {
        height = text->sizeHint().height();
    }
    text->setFixedHeight(height);
    adjustSize();

    Q_EMIT pToolTipChanged();
}

QString ElaItemViewToolTip::getToolTip() const
{
    Q_D(const ElaItemViewToolTip);
    return d->_pToolTip;
}
