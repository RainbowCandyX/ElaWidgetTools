#include "ElaInteractiveCard.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaInteractiveCardPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, Title);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QString, SubTitle);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, SubTitlePixelSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, TitleSpacing);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QPixmap, CardPixmap);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, QSize, CardPixmapSize);
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, int, CardPixmapBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaInteractiveCard, ElaCardPixType::PixMode, CardPixMode);

void ElaInteractiveCard::setCardOrientation(ElaCardOrientation::Orientation orientation)
{
    Q_D(ElaInteractiveCard);
    d->_pCardOrientation = orientation;
    // 纵向时图标会占满默认高度，故抬高最小高度：上边距10 + 图标 + 间隙6 + 标题 + 标题间距 + 副标题 + 下边距6
    setMinimumHeight(orientation == ElaCardOrientation::Orientation::Vertical
                         ? 10 + d->_pCardPixmapSize.height() + 6 + d->_pTitlePixelSize + d->_pTitleSpacing + d->_pSubTitlePixelSize + 6
                         : 80);
    Q_EMIT pCardOrientationChanged();
}

ElaCardOrientation::Orientation ElaInteractiveCard::getCardOrientation() const
{
    Q_D(const ElaInteractiveCard);
    return d->_pCardOrientation;
}

ElaInteractiveCard::ElaInteractiveCard(QWidget* parent)
    : QPushButton(parent), d_ptr(new ElaInteractiveCardPrivate())
{
    Q_D(ElaInteractiveCard);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pTitlePixelSize = 15;
    d->_pSubTitlePixelSize = 12;
    setMinimumSize(270, 80);
    d->_pCardPixmapSize = QSize(64, 64);
    d->_pTitleSpacing = 2;
    d->_pCardPixmapBorderRadius = 6;
    d->_pCardPixMode = ElaCardPixType::PixMode::Ellipse;
    d->_pCardOrientation = ElaCardOrientation::Orientation::Horizontal;
    d->_themeMode = eTheme->getThemeMode();
    setMouseTracking(true);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

ElaInteractiveCard::~ElaInteractiveCard()
{
}

void ElaInteractiveCard::setCardPixmapSize(int width, int height)
{
    Q_D(ElaInteractiveCard);
    d->_pCardPixmapSize = QSize(width, height);
    if (d->_pCardOrientation == ElaCardOrientation::Orientation::Vertical)
    {
        setMinimumHeight(10 + height + 6 + d->_pTitlePixelSize + d->_pTitleSpacing + d->_pSubTitlePixelSize + 6);
    }
    Q_EMIT pCardPixmapSizeChanged();
}

void ElaInteractiveCard::paintEvent(QPaintEvent* event)
{
    Q_D(ElaInteractiveCard);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(underMouse() ? ElaThemeColor(d->_themeMode, BasicHoverAlpha) : Qt::transparent);
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);

    // 统一的图标绘制（依据 CardPixMode 决定形状），rect 为绘制区域
    auto drawPixmap = [&](const QRect& rect) {
        if (d->_pCardPixmap.isNull())
        {
            return;
        }
        painter.save();
        QPainterPath path;
        if (d->_pCardPixMode == ElaCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2), rect.width() / 2, rect.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(rect, d->_pCardPixmap);
        }
        else if (d->_pCardPixMode == ElaCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(rect), d->_pCardPixmapBorderRadius, d->_pCardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(rect, d->_pCardPixmap);
        }
        else
        {
            painter.drawPixmap(rect, d->_pCardPixmap);
        }
        painter.restore();
    };

    const bool hasSub = !d->_pSubTitle.isEmpty();

    if (d->_pCardOrientation == ElaCardOrientation::Orientation::Vertical)
    {
        // 纵向模式：图标在上（水平居中），标题与副标题在下（水平居中，各按自身字号占一行）
        const int pixW = d->_pCardPixmapSize.width();
        const int pixH = d->_pCardPixmapSize.height();
        const int hMargin = 8;
        const int topMargin = 10;
        const int gap = 6;
        drawPixmap(QRect((width() - pixW) / 2, topMargin, pixW, pixH));

        const int textX = hMargin;
        const int textW = qMax(0, width() - hMargin * 2);
        const int textTop = topMargin + pixH + gap;
        const int titleH = d->_pTitlePixelSize;
        const int subH = d->_pSubTitlePixelSize;

        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        QFont font = this->font();
        font.setWeight(QFont::Bold);
        font.setPixelSize(d->_pTitlePixelSize);
        painter.setFont(font);
        painter.drawText(QRect(textX, textTop, textW, titleH), Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignVCenter, d->_pTitle);
        if (hasSub)
        {
            font.setWeight(QFont::Normal);
            font.setPixelSize(d->_pSubTitlePixelSize);
            painter.setFont(font);
            painter.drawText(QRect(textX, textTop + titleH + d->_pTitleSpacing, textW, subH), Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignVCenter, d->_pSubTitle);
        }
        painter.restore();
    }
    else {
        // 横向模式（默认）：图标在左、文字在右
        int pixW = d->_pCardPixmapSize.width();
        int pixH = d->_pCardPixmapSize.height();
        int iconY = (height() - pixH) / 2;
        drawPixmap(QRect(10, iconY, pixW, pixH));
        // 文字绘制
        painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
        QFont font = this->font();
        font.setWeight(QFont::Bold);
        font.setPixelSize(d->_pTitlePixelSize);
        painter.setFont(font);
        int textStartX = pixW + 26;
        int textWidth = width() - textStartX;
        painter.drawText(QRect(textStartX, rect().y(), textWidth, height() / 2 - d->_pTitleSpacing), Qt::TextWordWrap | Qt::AlignBottom | Qt::AlignLeft, d->_pTitle);
        if (hasSub) {
            font.setWeight(QFont::Normal);
            font.setPixelSize(d->_pSubTitlePixelSize);
            painter.setFont(font);
            painter.drawText(QRect(textStartX, height() / 2 + d->_pTitleSpacing, textWidth, height() / 2 - d->_pTitleSpacing), Qt::TextWordWrap | Qt::AlignTop | Qt::AlignLeft, d->_pSubTitle);
        }
        painter.restore();
    }
}
