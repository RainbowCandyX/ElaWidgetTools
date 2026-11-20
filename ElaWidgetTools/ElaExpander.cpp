#include "ElaExpander.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QVariantAnimation>

#include "ElaTheme.h"
#include "private/ElaExpanderPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaExpander, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaExpander, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaExpander, QString, SubTitle)
Q_PROPERTY_CREATE_Q_CPP(ElaExpander, ElaIconType::IconName, HeaderIcon)
Q_PROPERTY_CREATE_Q_CPP(ElaExpander, int, AnimationDuration)

ElaExpanderPrivate::ElaExpanderPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaExpanderPrivate::~ElaExpanderPrivate()
{
}

void ElaExpanderPrivate::_toggleExpand()
{
	_isExpanded = !_isExpanded;
	_startExpandAnimation(_isExpanded);
	Q_Q(ElaExpander);
	Q_EMIT q->expandStateChanged(_isExpanded);
}

void ElaExpanderPrivate::_updateContentHeight()
{
	Q_Q(ElaExpander);
	const QMargins margins = _contentLayout->contentsMargins();
	int height = margins.top() + margins.bottom();
	const int availableWidth = qMax(0, q->width() - margins.left() - margins.right());
	if (_contentWidget) 
	{
		if (_contentWidget->hasHeightForWidth() && availableWidth > 0)
		{
			height += qMax(_contentWidget->heightForWidth(availableWidth),
				_contentWidget->minimumSizeHint().height());
		}
		else
		{
			height += qMax(_contentWidget->sizeHint().height(),
				_contentWidget->minimumSizeHint().height());
		}
	}
	if (height < 40)
	{
		height = 40;
	}
	_contentHeight = height;
	_contentContainer->setFixedHeight(_contentHeight);
}

void ElaExpanderPrivate::_updateGeometry()
{
	Q_Q(ElaExpander);
	const int widgetWidth = q->width();
	const int headerY = (_expandDirection == ElaExpander::Down) ? 0 : q->height() - _headerHeight;
	const int contentY = (_expandDirection == ElaExpander::Down) ? _headerHeight : headerY - _contentHeight;
	_headerWidget->setGeometry(0, headerY, widgetWidth, _headerHeight);
	_contentContainer->setGeometry(0, contentY, widgetWidth, _contentHeight);
	if (_headerCustomWidget)
	{
		_headerCustomWidget->adjustSize();
		const int customWidth = qMin(_headerCustomWidget->width(), qMax(0, widgetWidth - 55));
		const int customHeight = qMin(_headerCustomWidget->height(), _headerHeight - 8);
		_headerCustomWidget->setGeometry(widgetWidth - customWidth - 40, headerY + (_headerHeight - customHeight) / 2, customWidth, customHeight);
	}
}

void ElaExpanderPrivate::_startExpandAnimation(bool expand)
{
	Q_Q(ElaExpander);
	if (!_contentWidget)
	{
		q->setFixedHeight(_headerHeight);
		return;
	}
	if (!_expandAnimation)
	{
		_expandAnimation = new QVariantAnimation(q);
		_expandAnimation->setEasingCurve(QEasingCurve::OutCubic);
		connect(_expandAnimation, &QVariantAnimation::valueChanged, q, [=](const QVariant &value)
		{
			q->setFixedHeight(value.toInt());
		});
		connect(_expandAnimation, &QVariantAnimation::finished, q, [=]()
		{
			if (!_isExpanded)
			{
				_contentContainer->hide();
			}
			q->update();
		});
	}
	_expandAnimation->stop();
	_expandAnimation->setDuration(_pAnimationDuration);
	_updateContentHeight();

	int collapsedHeight = _headerHeight;
	int expandedHeight = _headerHeight + _contentHeight;
	_expandAnimation->setStartValue(q->height());
	if (expand)
	{
		_contentContainer->show();
		_expandAnimation->setEndValue(expandedHeight);
	}
	else
	{
		if (_expandDirection == ElaExpander::Up)
		{
			_contentContainer->hide();
		}
		_expandAnimation->setEndValue(collapsedHeight);
	}
	_expandAnimation->start();
}

ElaExpander::ElaExpander(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaExpanderPrivate())
{
	Q_D(ElaExpander);
	d->q_ptr = this;
	d->_pBorderRadius = 6;
	d->_pTitle = "";
	d->_pSubTitle = "";
	d->_pHeaderIcon = ElaIconType::None;
	d->_pAnimationDuration = 250;
	setObjectName("ElaExpander");
	setMouseTracking(true);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	// Header area (painted via paintEvent, not a separate widget)
	d->_headerWidget = new QWidget(this);
	d->_headerWidget->setFixedHeight(d->_headerHeight);
	d->_headerWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

	// Content container
	d->_contentContainer = new QWidget(this);
	d->_contentLayout = new QVBoxLayout(d->_contentContainer);
	d->_contentLayout->setContentsMargins(15, 5, 15, 5);
	d->_contentContainer->hide();

	setFixedHeight(d->_headerHeight);
	d->_updateContentHeight();
	d->_updateGeometry();

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaExpander::ElaExpander(const QString &title, QWidget *parent)
	: ElaExpander(parent)
{
	Q_D(ElaExpander);
	d->_pTitle = title;
}

ElaExpander::~ElaExpander()
{
}

void ElaExpander::setExpandDirection(ExpandDirection direction)
{
	Q_D(ElaExpander);
	d->_expandDirection = direction;
}

ElaExpander::ExpandDirection ElaExpander::getExpandDirection() const
{
	return d_ptr->_expandDirection;
}

void ElaExpander::setContentWidget(QWidget *widget)
{
	Q_D(ElaExpander);
	if (d->_contentWidget == widget)
	{
		return;
	}
	if (d->_contentWidget)
	{
		d->_contentLayout->removeWidget(d->_contentWidget);
		d->_contentWidget->setParent(nullptr);
		d->_contentWidget->deleteLater();
	}
	d->_contentWidget = widget;
	if (widget)
	{
		d->_contentLayout->addWidget(widget);
		widget->show();
	}
	d->_updateContentHeight();
	if (d->_isExpanded)
	{
		if (d->_expandAnimation)
		{
			d->_expandAnimation->stop();
		}
		setFixedHeight(d->_headerHeight + d->_contentHeight);
		d->_contentContainer->setVisible(widget != nullptr);
	}
	else
	{
		d->_contentContainer->hide();
	}
	d->_updateGeometry();
	update();
}

QWidget *ElaExpander::getContentWidget() const
{
	return d_ptr->_contentWidget;
}

void ElaExpander::setHeaderWidget(QWidget *widget)
{
	Q_D(ElaExpander);
	if (d->_headerCustomWidget == widget)
	{
		return;
	}
	if (d->_headerCustomWidget)
	{
		d->_headerCustomWidget->setParent(nullptr);
		d->_headerCustomWidget->deleteLater();
	}
	d->_headerCustomWidget = widget;
	if (widget)
	{
		widget->setParent(this);
		widget->show();
		widget->raise();
	}
	d->_updateGeometry();
	update();
}

void ElaExpander::setIsExpanded(bool expanded)
{
	Q_D(ElaExpander);
	if (d->_isExpanded != expanded)
	{
		d->_toggleExpand();
	}
}

bool ElaExpander::getIsExpanded() const
{
	return d_ptr->_isExpanded;
}

void ElaExpander::resizeToContent()
{
	Q_D(ElaExpander);
	d->_startExpandAnimation(d->_isExpanded);
}

void ElaExpander::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaExpander);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	// Background
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
	painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseAlpha));
	painter.drawRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->_pBorderRadius, d->_pBorderRadius);

	// Header separator line when expanded
	if (d->_isExpanded || (d->_expandAnimation && d->_expandAnimation->state() == QVariantAnimation::Running))
	{
		int lineY = (d->_expandDirection == Down) ? d->_headerHeight : height() - d->_headerHeight;
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 1));
		painter.drawLine(d->_pBorderRadius, lineY, width() - d->_pBorderRadius, lineY);
	}

	// Header area
	int headerY = (d->_expandDirection == Down) ? 0 : height() - d->_headerHeight;
	int rightMargin = 40;
	if (d->_headerCustomWidget)
	{
		rightMargin += d->_headerCustomWidget->width() + 8;
	}

	// Header icon
	int textStartX = 15;
	if (d->_pHeaderIcon != ElaIconType::None)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(16);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QRect iconRect(15, headerY, 20, d->_headerHeight);
		painter.drawText(iconRect, Qt::AlignVCenter | Qt::AlignLeft, QChar((unsigned short) d->_pHeaderIcon));
		textStartX = 42;
	}

	// Title
	if (!d->_pTitle.isEmpty())
	{
		QFont titleFont = font();
		titleFont.setPixelSize(14);
		titleFont.setBold(true);
		painter.setFont(titleFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QRect titleRect(textStartX, headerY, qMax(0, width() - textStartX - rightMargin), d->_headerHeight);
		painter.drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, d->_pTitle);

		// SubTitle
		if (!d->_pSubTitle.isEmpty())
		{
			QFontMetrics fm(titleFont);
			int titleWidth = fm.horizontalAdvance(d->_pTitle);
			QFont subFont = font();
			subFont.setPixelSize(12);
			painter.setFont(subFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
			QRect subRect(textStartX + titleWidth + 12, headerY, qMax(0, width() - textStartX - titleWidth - rightMargin - 12), d->_headerHeight);
			painter.drawText(subRect, Qt::AlignVCenter | Qt::AlignLeft, d->_pSubTitle);
		}
	}

	// Expand/collapse arrow
	QFont arrowFont = QFont("ElaAwesome");
	arrowFont.setPixelSize(14);
	painter.setFont(arrowFont);
	painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
	QRect arrowRect(width() - 35, headerY, 20, d->_headerHeight);
	ElaIconType::IconName arrowIcon;
	if (d->_expandDirection == ElaExpander::Down)
	{
		arrowIcon = d->_isExpanded ? ElaIconType::AngleUp : ElaIconType::AngleDown;
	}
	else
	{
		arrowIcon = d->_isExpanded ? ElaIconType::AngleDown : ElaIconType::AngleUp;
	}
	painter.drawText(arrowRect, Qt::AlignVCenter | Qt::AlignCenter, QChar((unsigned short) arrowIcon));
}

void ElaExpander::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaExpander);
	int headerY = (d->_expandDirection == Down) ? 0 : height() - d->_headerHeight;
	QRect headerRect(0, headerY, width(), d->_headerHeight);
	if (headerRect.contains(event->pos()) && event->button() == Qt::LeftButton)
	{
		d->_isPressed = true;
	}
	QWidget::mousePressEvent(event);
}

void ElaExpander::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaExpander);
	if (d->_isPressed && event->button() == Qt::LeftButton)
	{
		int headerY = (d->_expandDirection == Down) ? 0 : height() - d->_headerHeight;
		QRect headerRect(0, headerY, width(), d->_headerHeight);
		if (headerRect.contains(event->pos()))
		{
			d->_toggleExpand();
		}
		d->_isPressed = false;
	}
	QWidget::mouseReleaseEvent(event);
}

bool ElaExpander::event(QEvent *event)
{
	Q_D(ElaExpander);
	switch (event->type())
	{
		case QEvent::Enter:
		case QEvent::Leave:
			update();
			break;
		case QEvent::Show:
			resizeToContent();
		case QEvent::Resize:
			d->_updateGeometry();
			break;
		default:
			break;
	}
	return QWidget::event(event);
}