#include "ElaNumberBox.h"

#include <QKeyEvent>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>

#include "ElaTheme.h"
#include "private/ElaNumberBoxPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, double, Minimum)
Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, double, Maximum)
Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, double, Step)
Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, int, Decimals)
Q_PROPERTY_CREATE_Q_CPP(ElaNumberBox, bool, IsWrapping)

// Custom setter for Value to clamp and emit
void ElaNumberBox::setValue(double value)
{
    Q_D(ElaNumberBox);
    d->_pValue = value;
    d->_clampValue();
    Q_EMIT pValueChanged();
    Q_EMIT valueChanged(d->_pValue);
    update();
}

double ElaNumberBox::getValue() const
{
    return d_ptr->_pValue;
}

ElaNumberBoxPrivate::ElaNumberBoxPrivate(QObject* parent)
    : QObject(parent)
{
}

ElaNumberBoxPrivate::~ElaNumberBoxPrivate()
{
}

QRect ElaNumberBoxPrivate::_minusRect() const
{
    Q_Q(const ElaNumberBox);
    return QRect(0, 0, _buttonWidth, q->height());
}

QRect ElaNumberBoxPrivate::_plusRect() const
{
    Q_Q(const ElaNumberBox);
    return QRect(q->width() - _buttonWidth, 0, _buttonWidth, q->height());
}

QRect ElaNumberBoxPrivate::_valueRect() const
{
    Q_Q(const ElaNumberBox);
    return QRect(_buttonWidth, 0, q->width() - _buttonWidth * 2, q->height());
}

QString ElaNumberBoxPrivate::_formatValue() const
{
    return QString::number(_pValue, 'f', _pDecimals);
}

void ElaNumberBoxPrivate::_clampValue()
{
    if (_pValue < _pMinimum)
    {
        _pValue = _pIsWrapping ? _pMaximum : _pMinimum;
    }
    if (_pValue > _pMaximum)
    {
        _pValue = _pIsWrapping ? _pMinimum : _pMaximum;
    }
}

void ElaNumberBoxPrivate::_startEditing()
{
    Q_Q(ElaNumberBox);
    _isEditing = true;
    if (!_lineEdit)
    {
        _lineEdit = new QLineEdit(q);
        _lineEdit->setAlignment(Qt::AlignCenter);
        _lineEdit->setFrame(false);
        QFont f = q->font();
        f.setPixelSize(14);
        _lineEdit->setFont(f);
        _lineEdit->setStyleSheet("QLineEdit { background: transparent; border: none; }");
        _lineEdit->installEventFilter(q);
    }
    _lineEdit->setGeometry(_valueRect());
    _lineEdit->setText(_formatValue());
    _lineEdit->selectAll();
    _lineEdit->show();
    _lineEdit->setFocus();
}

void ElaNumberBoxPrivate::_finishEditing()
{
    Q_Q(ElaNumberBox);
    if (!_isEditing)
    {
        return;
    }
    _isEditing = false;
    if (_lineEdit)
    {
        bool ok;
        double newVal = _lineEdit->text().toDouble(&ok);
        if (ok)
        {
            _pValue = newVal;
            _clampValue();
            Q_EMIT q->pValueChanged();
            Q_EMIT q->valueChanged(_pValue);
        }
        _lineEdit->hide();
    }
    q->update();
}

ElaNumberBox::ElaNumberBox(QWidget* parent)
    : QWidget{parent}, d_ptr(new ElaNumberBoxPrivate())
{
    Q_D(ElaNumberBox);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pValue = 0.0;
    d->_pMinimum = 0.0;
    d->_pMaximum = 100.0;
    d->_pStep = 1.0;
    d->_pDecimals = 0;
    d->_pIsWrapping = false;
    setObjectName("ElaNumberBox");
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(160, 36);

    d->_themeMode = eTheme->getThemeMode();
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
}

ElaNumberBox::~ElaNumberBox()
{
}

void ElaNumberBox::stepUp()
{
    Q_D(ElaNumberBox);
    setValue(d->_pValue + d->_pStep);
}

void ElaNumberBox::stepDown()
{
    Q_D(ElaNumberBox);
    setValue(d->_pValue - d->_pStep);
}

void ElaNumberBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(ElaNumberBox);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect fullRect(0, 0, width(), height());
    QColor borderColor = ElaThemeColor(d->_themeMode, BasicBorder);
    QColor baseColor = ElaThemeColor(d->_themeMode, BasicBase);
    QColor hoverColor = ElaThemeColor(d->_themeMode, BasicHover);
    QColor pressColor = ElaThemeColor(d->_themeMode, BasicPress);
    QColor textColor = ElaThemeColor(d->_themeMode, BasicText);

    // Background
    QPainterPath clipPath;
    clipPath.addRoundedRect(fullRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.setClipPath(clipPath);

    // Minus button
    QRect minusR = d->_minusRect();
    QColor minusBg = baseColor;
    if (d->_isMinusPressed)
        minusBg = pressColor;
    else if (d->_isMinusHovered)
        minusBg = hoverColor;
    painter.setPen(Qt::NoPen);
    painter.setBrush(minusBg);
    painter.drawRect(minusR);

    // Plus button
    QRect plusR = d->_plusRect();
    QColor plusBg = baseColor;
    if (d->_isPlusPressed)
        plusBg = pressColor;
    else if (d->_isPlusHovered)
        plusBg = hoverColor;
    painter.setBrush(plusBg);
    painter.drawRect(plusR);

    // Value area
    QRect valR = d->_valueRect();
    painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
    painter.drawRect(valR);

    // Reset clip, draw border
    painter.setClipPath(clipPath);
    painter.setPen(QPen(borderColor, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->_pBorderRadius, d->_pBorderRadius);

    // Separator lines
    painter.setPen(QPen(borderColor, 1));
    painter.drawLine(d->_buttonWidth, 4, d->_buttonWidth, height() - 4);
    painter.drawLine(width() - d->_buttonWidth, 4, width() - d->_buttonWidth, height() - 4);

    // Minus icon
    QFont btnFont = font();
    btnFont.setPixelSize(18);
    btnFont.setBold(true);
    painter.setFont(btnFont);
    bool atMin = d->_pValue <= d->_pMinimum && !d->_pIsWrapping;
    painter.setPen(atMin ? ElaThemeColor(d->_themeMode, BasicTextDisable) : textColor);
    painter.drawText(minusR, Qt::AlignCenter, "-");

    // Plus icon
    bool atMax = d->_pValue >= d->_pMaximum && !d->_pIsWrapping;
    painter.setPen(atMax ? ElaThemeColor(d->_themeMode, BasicTextDisable) : textColor);
    painter.drawText(plusR, Qt::AlignCenter, "+");

    // Value text (only when not editing)
    if (!d->_isEditing)
    {
        QFont valFont = font();
        valFont.setPixelSize(14);
        painter.setFont(valFont);
        painter.setPen(textColor);
        painter.drawText(valR, Qt::AlignCenter, d->_formatValue());
    }
}

void ElaNumberBox::mousePressEvent(QMouseEvent* event)
{
    Q_D(ElaNumberBox);
    if (event->button() == Qt::LeftButton)
    {
        if (d->_minusRect().contains(event->pos()))
        {
            d->_isMinusPressed = true;
            update();
        }
        else if (d->_plusRect().contains(event->pos()))
        {
            d->_isPlusPressed = true;
            update();
        }
    }
    QWidget::mousePressEvent(event);
}

void ElaNumberBox::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(ElaNumberBox);
    if (event->button() == Qt::LeftButton)
    {
        if (d->_isMinusPressed && d->_minusRect().contains(event->pos()))
        {
            stepDown();
        }
        if (d->_isPlusPressed && d->_plusRect().contains(event->pos()))
        {
            stepUp();
        }
        d->_isMinusPressed = false;
        d->_isPlusPressed = false;
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void ElaNumberBox::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(ElaNumberBox);
    bool minusHover = d->_minusRect().contains(event->pos());
    bool plusHover = d->_plusRect().contains(event->pos());
    if (d->_isMinusHovered != minusHover || d->_isPlusHovered != plusHover)
    {
        d->_isMinusHovered = minusHover;
        d->_isPlusHovered = plusHover;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void ElaNumberBox::leaveEvent(QEvent* event)
{
    Q_D(ElaNumberBox);
    d->_isMinusHovered = false;
    d->_isPlusHovered = false;
    d->_isMinusPressed = false;
    d->_isPlusPressed = false;
    update();
    QWidget::leaveEvent(event);
}

void ElaNumberBox::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        stepUp();
    }
    else if (event->angleDelta().y() < 0)
    {
        stepDown();
    }
    event->accept();
}

void ElaNumberBox::keyPressEvent(QKeyEvent* event)
{
    Q_D(ElaNumberBox);
    switch (event->key())
    {
    case Qt::Key_Up:
        stepUp();
        return;
    case Qt::Key_Down:
        stepDown();
        return;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (d->_isEditing)
        {
            d->_finishEditing();
        }
        else
        {
            d->_startEditing();
        }
        return;
    case Qt::Key_Escape:
        if (d->_isEditing)
        {
            d->_isEditing = false;
            if (d->_lineEdit)
            {
                d->_lineEdit->hide();
            }
            setFocus();
            update();
        }
        return;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void ElaNumberBox::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_D(ElaNumberBox);
    if (d->_valueRect().contains(event->pos()))
    {
        d->_startEditing();
    }
    QWidget::mouseDoubleClickEvent(event);
}

bool ElaNumberBox::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(ElaNumberBox);
    if (watched == d->_lineEdit)
    {
        if (event->type() == QEvent::FocusOut)
        {
            d->_finishEditing();
        }
        else if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
            {
                d->_finishEditing();
                setFocus();
                return true;
            }
            if (ke->key() == Qt::Key_Escape)
            {
                d->_isEditing = false;
                d->_lineEdit->hide();
                setFocus();
                update();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
