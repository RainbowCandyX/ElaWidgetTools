#include "ElaSteps.h"

#include <QPainter>

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
	setFixedHeight(70);

	d->_pCurrentStep = 0;
	d->_pStepCount = 3;

	d->_themeMode = eTheme->getThemeMode();
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