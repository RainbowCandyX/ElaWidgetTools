#include "ElaSkeleton.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

#include "ElaTheme.h"
#include "private/ElaSkeletonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSkeleton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSkeleton, bool, IsAnimated)

ElaSkeletonPrivate::ElaSkeletonPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaSkeletonPrivate::~ElaSkeletonPrivate()
{
}

void ElaSkeletonPrivate::_startShimmerAnimation()
{
	Q_Q(ElaSkeleton);
	if (_shimmerAnimation)
	{
		_shimmerAnimation->stop();
		delete _shimmerAnimation;
		_shimmerAnimation = nullptr;
	}
	_shimmerAnimation = new QVariantAnimation(this);
	_shimmerAnimation->setStartValue(0.0);
	_shimmerAnimation->setEndValue(1.0);
	_shimmerAnimation->setDuration(1500);
	_shimmerAnimation->setLoopCount(-1);
	connect(_shimmerAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant &value)
	{
		_pShimmerPosition = value.toReal();
		q->update();
	});
	_shimmerAnimation->start();
}

void ElaSkeletonPrivate::_stopShimmerAnimation()
{
	if (_shimmerAnimation)
	{
		_shimmerAnimation->stop();
		delete _shimmerAnimation;
		_shimmerAnimation = nullptr;
	}
	_pShimmerPosition = 0.0;
}

ElaSkeleton::ElaSkeleton(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaSkeletonPrivate())
{
	Q_D(ElaSkeleton);
	d->q_ptr = this;
	d->_pBorderRadius = 4;
	d->_pIsAnimated = true;
	d->_pShimmerPosition = 0.0;
	setObjectName("ElaSkeleton");
	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	d->_startShimmerAnimation();
}

ElaSkeleton::~ElaSkeleton()
{
	Q_D(ElaSkeleton);
	d->_stopShimmerAnimation();
}

void ElaSkeleton::setSkeletonType(SkeletonType type)
{
	Q_D(ElaSkeleton);
	d->_skeletonType = type;
	if (type == Circle)
	{
		int diameter = width();
		setFixedSize(diameter, diameter);
	}
	update();
}

ElaSkeleton::SkeletonType ElaSkeleton::getSkeletonType() const
{
	return d_ptr->_skeletonType;
}

void ElaSkeleton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaSkeleton);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);

	QColor baseColor = ElaThemeColor(d->_themeMode, BasicDisable);

	if (d->_pIsAnimated)
	{
		QLinearGradient gradient(0, 0, width(), 0);
		QColor lighterColor = baseColor.lighter(120);
		qreal pos = d->_pShimmerPosition;
		qreal shimmerWidth = 0.3;
		qreal start = pos - shimmerWidth;
		qreal end = pos + shimmerWidth;

		if (start < 0.0)
		{
			gradient.setColorAt(0.0, lighterColor);
			gradient.setColorAt(qMin(end, 1.0), baseColor);
			if (end < 1.0)
			{
				gradient.setColorAt(1.0, baseColor);
			}
		}
		else if (end > 1.0)
		{
			gradient.setColorAt(0.0, baseColor);
			gradient.setColorAt(start, baseColor);
			gradient.setColorAt(1.0, lighterColor);
		}
		else
		{
			gradient.setColorAt(0.0, baseColor);
			gradient.setColorAt(start, baseColor);
			gradient.setColorAt(pos, lighterColor);
			gradient.setColorAt(end, baseColor);
			gradient.setColorAt(1.0, baseColor);
		}
		painter.setBrush(gradient);
	}
	else
	{
		painter.setBrush(baseColor);
	}

	switch (d->_skeletonType)
	{
		case Text:
		{
			QPainterPath path;
			path.addRoundedRect(QRectF(0, 0, width(), height()), 4, 4);
			painter.drawPath(path);
			break;
		}
		case Circle:
		{
			painter.drawEllipse(QRectF(0, 0, width(), height()));
			break;
		}
		case Rectangle:
		{
			QPainterPath path;
			path.addRoundedRect(QRectF(0, 0, width(), height()), d->_pBorderRadius, d->_pBorderRadius);
			painter.drawPath(path);
			break;
		}
	}
}

QSize ElaSkeleton::sizeHint() const
{
	Q_D(const ElaSkeleton);
	switch (d->_skeletonType)
	{
		case Text:
			return QSize(200, 16);
		case Circle:
			return QSize(40, 40);
		case Rectangle:
			return QSize(200, 100);
	}
	return QSize(200, 16);
}