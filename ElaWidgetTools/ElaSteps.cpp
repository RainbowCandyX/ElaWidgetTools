#include "ElaSteps.h"

#include <QMouseEvent>
#include <QPainter>
#include <QRectF>

#include "ElaTheme.h"
#include "private/ElaStepsPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaSteps, int, CurrentStep)
Q_PROPERTY_CREATE_Q_CPP(ElaSteps, int, StepCount)

ElaSteps::ElaSteps(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaStepsPrivate())
{
	Q_D(ElaSteps);
	d->q_ptr = this;
	setObjectName("ElaSteps");
	setMouseTracking(true);
	updateSize();

	d->_pCurrentStep = 0;
	d->_pStepCount = 3;

	d->_themeMode = eTheme->getThemeMode();
	connect(this, &ElaSteps::pCurrentStepChanged, this, [=]() {update();});
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaSteps::~ElaSteps()
{
}

void ElaSteps::setStepTitles(const QStringList &titles)
{
	Q_D(ElaSteps);
	d->_stepTitles = titles;
	update();
}

QStringList ElaSteps::getStepTitles() const
{
	Q_D(const ElaSteps);
	return d->_stepTitles;
}

void ElaSteps::setStepOrientation(Qt::Orientation orientation)
{
	Q_D(ElaSteps);
	if (d->_pStepOrientation == orientation)
		return;
	d->_pStepOrientation = orientation;
	updateSize();
	update();
}

Qt::Orientation ElaSteps::stepOrientation() const
{
	Q_D(const ElaSteps);
	return d->_pStepOrientation;
}

void ElaSteps::setStepClickEnabled(bool enabled)
{
	Q_D(ElaSteps);
	if (d->_pStepClickEnabled == enabled)
	{
		return;
	}
	d->_pStepClickEnabled = enabled;
	if (!enabled)
	{
		d->_pHoverStep = -1;
		unsetCursor();
		update();
	}
}

bool ElaSteps::getStepClickEnabled() const
{
	Q_D(const ElaSteps);
	return d->_pStepClickEnabled;
}

void ElaSteps::updateSize()
{
	Q_D(ElaSteps);
	if (d->_pStepOrientation == Qt::Vertical)
		setFixedHeight(60 + qMax(0, d->_pStepCount - 1) * 52);
	else
		setFixedHeight(70);
}

void ElaSteps::next()
{
	Q_D(ElaSteps);
	if (d->_pCurrentStep < d->_pStepCount - 1)
	{
		d->_pCurrentStep++;
		update();
		Q_EMIT currentStepChanged(d->_pCurrentStep);
	}
}

void ElaSteps::previous()
{
	Q_D(ElaSteps);
	if (d->_pCurrentStep > 0)
	{
		d->_pCurrentStep--;
		update();
		Q_EMIT currentStepChanged(d->_pCurrentStep);
	}
}

int ElaSteps::stepAtPoint(const QPoint &pos) const
{
	Q_D(const ElaSteps);
	const int stepCount = d->_pStepCount;
	if (stepCount <= 0)
	{
		return -1;
	}

	if (d->_pStepOrientation == Qt::Vertical)
	{
		const int circleRadius = 14;
		const int leftX = 22;
		const int firstCenter = 30;
		const int rowSpacing = 52;
		const int titleX = leftX + 28 + 14;
		const int titleMaxW = qMax(40, width() - titleX - 16);

		for (int i = 0; i < stepCount; ++i)
		{
			const qreal cy = firstCenter + i * rowSpacing;
			const QRectF circleRect(leftX - circleRadius, cy - circleRadius, 28, 28);
			const QRectF titleRect(titleX, cy - rowSpacing / 2.0, titleMaxW, rowSpacing);
			if (circleRect.contains(pos) || titleRect.contains(pos))
			{
				return i;
			}
		}
	}
	else
	{
		const int margin = 50;
		const int circleRadius = 14;
		const qreal usableWidth = width() - margin * 2;
		const qreal spacing = (stepCount > 1) ? usableWidth / (stepCount - 1) : 0;

		for (int i = 0; i < stepCount; ++i)
		{
			const qreal cx = (stepCount > 1) ? (margin + i * spacing) : (width() / 2.0);
			const QRectF circleRect(cx - circleRadius, 8, 28, 28);
			const QRectF titleRect(cx - 60, 42, 120, 20);
			if (circleRect.contains(pos) || titleRect.contains(pos))
			{
				return i;
			}
		}
	}
	return -1;
}

void ElaSteps::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaSteps);
	if (!d->_pStepClickEnabled || event->button() != Qt::LeftButton)
	{
		QWidget::mouseReleaseEvent(event);
		return;
	}

	const int hit = stepAtPoint(event->pos());
	if (hit >= 0 && hit != d->_pCurrentStep)
	{
		d->_pCurrentStep = hit;
		update();
		Q_EMIT currentStepChanged(hit);
		event->accept();
		return;
	}

	QWidget::mouseReleaseEvent(event);
}

void ElaSteps::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaSteps);
	if (!d->_pStepClickEnabled)
	{
		QWidget::mouseMoveEvent(event);
		return;
	}
	const int hit = stepAtPoint(event->pos());
	if (hit != d->_pHoverStep)
	{
		d->_pHoverStep = hit;
		setCursor(hit >= 0 ? Qt::PointingHandCursor : Qt::ArrowCursor);
		update();
	}
	QWidget::mouseMoveEvent(event);
}

void ElaSteps::leaveEvent(QEvent *event)
{
	Q_D(ElaSteps);
	if (d->_pHoverStep != -1)
	{
		d->_pHoverStep = -1;
		unsetCursor();
		update();
	}
	QWidget::leaveEvent(event);
}

void ElaSteps::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaSteps);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int stepCount = d->_pStepCount;
	if (stepCount <= 0)
	{
		return;
	}

	int w = width();
	int circleDiameter = 28;
	int circleRadius = circleDiameter / 2;
	int circleY = 8;
	int titleY = 42;
	int lineCenterY = circleY + circleRadius;

	// Evenly distribute circles with margin for end titles
	int margin = 50;
	qreal usableWidth = w - margin * 2;
	qreal spacing = (stepCount > 1) ? usableWidth / (stepCount - 1) : 0;

	QColor primaryNormal = ElaThemeColor(d->_themeMode, PrimaryNormal);
	QColor basicBase = ElaThemeColor(d->_themeMode, BasicBase);
	QColor basicBorder = ElaThemeColor(d->_themeMode, BasicBorder);
	QColor basicDetailsText = ElaThemeColor(d->_themeMode, BasicDetailsText);
	QColor whiteColor = Qt::white;

	QFont iconFont("ElaAwesome");
	iconFont.setPixelSize(14);

	QFont numberFont = this->font();
	numberFont.setPixelSize(14);
	numberFont.setWeight(QFont::Bold);

	QFont titleFont = this->font();
	titleFont.setPixelSize(13);

	if (d->_pStepOrientation == Qt::Vertical)
	{
		const int circleDiameter = 28;
		const int circleRadius = circleDiameter / 2;  // 14
		const int leftX = 22;                          // 圆点中心 x
		const int firstCenter = 16 + circleRadius;     // 首个圆点中心 y
		const int rowSpacing = 52;                     // 行间距
		const int titleX = leftX + circleDiameter + 14;
		const int titleMaxW = qMax(40, width() - titleX - 16);

		for (int i = 0; i < stepCount; i++)
		{
			const qreal cy = firstCenter + i * rowSpacing;

			// 连接竖线
			if (i < stepCount - 1)
			{
				const qreal nextCy = firstCenter + (i + 1) * rowSpacing;
				if (i < d->_pCurrentStep)
					painter.setPen(QPen(primaryNormal, 2));
				else
					painter.setPen(QPen(basicBorder, 2));
				painter.drawLine(QPointF(leftX, cy + circleRadius), QPointF(leftX, nextCy - circleRadius));
			}

			if (i == d->_pHoverStep)
			{
				QColor halo = primaryNormal;
				halo.setAlpha(45);
				painter.setPen(Qt::NoPen);
				painter.setBrush(halo);
				painter.drawEllipse(QRectF(leftX - circleRadius - 5, cy - circleRadius - 5, circleDiameter + 10, circleDiameter + 10));
			}

			// 圆点
			const QRectF circleRect(leftX - circleRadius, cy - circleRadius, circleDiameter, circleDiameter);
			painter.setPen(Qt::NoPen);
			if (i < d->_pCurrentStep)
			{
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
				painter.setPen(whiteColor);
				painter.setFont(iconFont);
				painter.drawText(circleRect, Qt::AlignCenter, QChar((unsigned short)0xea6c));
			}
			else if (i == d->_pCurrentStep)
			{
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
				painter.setPen(whiteColor);
				painter.setFont(numberFont);
				painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
			}
			else
			{
				painter.setBrush(basicBase);
				painter.drawEllipse(circleRect);
				painter.setPen(QPen(basicBorder, 1));
				painter.setBrush(Qt::NoBrush);
				painter.drawEllipse(circleRect);
				painter.setPen(basicDetailsText);
				painter.setFont(numberFont);
				painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
			}

			// 标题：序号圆点中心与文本垂直居中对齐，长文本自动换行
			if (i < d->_stepTitles.size())
			{
				painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
				painter.setFont(titleFont);
				const QRectF titleRect(titleX, cy - rowSpacing / 2.0, titleMaxW, rowSpacing);
				painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, d->_stepTitles[i]);
			}
		}
		return;
	}

	for (int i = 0; i < stepCount; i++)
	{
		qreal cx = (stepCount > 1) ? (margin + i * spacing) : (w / 2.0);

		// Draw connecting line (before circle, so circle overlaps)
		if (i < stepCount - 1)
		{
			qreal lineStartX = cx + circleRadius;
			qreal nextCx = (margin + (i + 1) * spacing);
			qreal lineEndX = nextCx - circleRadius;

			if (i < d->_pCurrentStep)
			{
				painter.setPen(QPen(primaryNormal, 2));
			}
			else
			{
				painter.setPen(QPen(basicBorder, 2));
			}
			painter.drawLine(QPointF(lineStartX, lineCenterY), QPointF(lineEndX, lineCenterY));
		}

		if (i == d->_pHoverStep)
		{
			QColor halo = primaryNormal;
			halo.setAlpha(45);
			painter.setPen(Qt::NoPen);
			painter.setBrush(halo);
			painter.drawEllipse(QRectF(cx - circleRadius - 5, circleY - 5, circleDiameter + 10, circleDiameter + 10));
		}

		// Draw circle
		QRectF circleRect(cx - circleRadius, circleY, circleDiameter, circleDiameter);
		painter.setPen(Qt::NoPen);

		if (i < d->_pCurrentStep)
		{
			// Completed
			painter.setBrush(primaryNormal);
			painter.drawEllipse(circleRect);

			// Draw check icon
			painter.setPen(whiteColor);
			painter.setFont(iconFont);
			painter.drawText(circleRect, Qt::AlignCenter, QChar((unsigned short) 0xea6c));
		}
		else if (i == d->_pCurrentStep)
		{
			// Active
			painter.setBrush(primaryNormal);
			painter.drawEllipse(circleRect);

			// Draw number
			painter.setPen(whiteColor);
			painter.setFont(numberFont);
			painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
		}
		else
		{
			// Pending
			painter.setBrush(basicBase);
			painter.drawEllipse(circleRect);
			painter.setPen(QPen(basicBorder, 1));
			painter.setBrush(Qt::NoBrush);
			painter.drawEllipse(circleRect);

			// Draw number
			painter.setPen(basicDetailsText);
			painter.setFont(numberFont);
			painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
		}

		// Draw title
		if (i < d->_stepTitles.size())
		{
			painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
			painter.setFont(titleFont);
			int titleWidth = 120;
			QRectF titleRect(cx - titleWidth / 2.0, titleY, titleWidth, 20);
			painter.drawText(titleRect, Qt::AlignHCenter | Qt::AlignTop, d->_stepTitles[i]);
		}
	}
}

ElaStepsPrivate::ElaStepsPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaStepsPrivate::~ElaStepsPrivate()
{
}