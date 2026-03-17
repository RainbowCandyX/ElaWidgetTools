#include "ElaWizard.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ElaTheme.h"
#include "private/ElaWizardPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaWizard, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaWizard, int, CurrentStep)

ElaWizard::ElaWizard(QWidget* parent)
	: QWidget{nullptr}, d_ptr(new ElaWizardPrivate())
{
	Q_D(ElaWizard);
	d->q_ptr = this;
	d->_pBorderRadius = 10;
	d->_pCurrentStep = 0;
	setObjectName("ElaWizard");
	setFixedSize(600, 450);
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	d->_stackedWidget = new QStackedWidget(this);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	int sw = d->_shadowBorderWidth;
	mainLayout->setContentsMargins(sw, sw + 70, sw, sw + 50);
	mainLayout->addWidget(d->_stackedWidget);

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});

	// Center on screen
	QScreen* screen = QApplication::primaryScreen();
	if (screen)
	{
		QRect screenGeometry = screen->geometry();
		int x = (screenGeometry.width() - width()) / 2 + screenGeometry.x();
		int y = (screenGeometry.height() - height()) / 2 + screenGeometry.y();
		move(x, y);
	}
}

ElaWizard::~ElaWizard()
{
}

void ElaWizard::addStep(const QString& title, QWidget* page)
{
	Q_D(ElaWizard);
	d->_stepTitles.append(title);
	d->_stepPages.append(page);
	d->_stackedWidget->addWidget(page);
	update();
}

void ElaWizard::next()
{
	Q_D(ElaWizard);
	if (d->_pCurrentStep < d->_stepPages.count() - 1)
	{
		d->_pCurrentStep++;
		d->_stackedWidget->setCurrentIndex(d->_pCurrentStep);
		update();
		Q_EMIT currentStepChanged(d->_pCurrentStep);
	}
}

void ElaWizard::previous()
{
	Q_D(ElaWizard);
	if (d->_pCurrentStep > 0)
	{
		d->_pCurrentStep--;
		d->_stackedWidget->setCurrentIndex(d->_pCurrentStep);
		update();
		Q_EMIT currentStepChanged(d->_pCurrentStep);
	}
}

void ElaWizard::finish()
{
	Q_EMIT finished();
	hide();
}

int ElaWizard::getStepCount() const
{
	Q_D(const ElaWizard);
	return d->_stepPages.count();
}

void ElaWizard::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	Q_D(ElaWizard);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int sw = d->_shadowBorderWidth;

	// Shadow
	eTheme->drawEffectShadow(&painter, rect(), sw, d->_pBorderRadius);

	// Background
	QRect foregroundRect = rect().adjusted(sw, sw, -sw, -sw);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	int fgX = foregroundRect.x();
	int fgY = foregroundRect.y();
	int fgW = foregroundRect.width();
	int fgH = foregroundRect.height();

	// --- Top area: step indicator (70px) ---
	int topAreaHeight = 70;
	int stepCount = d->_stepPages.count();

	if (stepCount > 0)
	{
		int circleDiameter = 28;
		int circleRadius = circleDiameter / 2;
		int circleY = fgY + 8;
		int titleY = fgY + 42;
		int lineCenterY = circleY + circleRadius;

		int margin = 50;
		qreal usableWidth = fgW - margin * 2;
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
			qreal cx = (stepCount > 1) ? (fgX + margin + i * spacing) : (fgX + fgW / 2.0);

			// Connecting line
			if (i < stepCount - 1)
			{
				qreal lineStartX = cx + circleRadius;
				qreal nextCx = (fgX + margin + (i + 1) * spacing);
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

			// Circle
			QRectF circleRect(cx - circleRadius, circleY, circleDiameter, circleDiameter);
			painter.setPen(Qt::NoPen);

			if (i < d->_pCurrentStep)
			{
				// Completed
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
				painter.setPen(whiteColor);
				painter.setFont(iconFont);
				painter.drawText(circleRect, Qt::AlignCenter, QChar((unsigned short) 0xea6c));
			}
			else if (i == d->_pCurrentStep)
			{
				// Active
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
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
				painter.setPen(basicDetailsText);
				painter.setFont(numberFont);
				painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
			}

			// Title
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

	// --- Separator line: top / middle ---
	QColor separatorColor = ElaThemeColor(d->_themeMode, BasicBorder);
	painter.setPen(QPen(separatorColor, 1));
	int topSepY = fgY + topAreaHeight;
	painter.drawLine(fgX + 1, topSepY, fgX + fgW - 1, topSepY);

	// --- Separator line: middle / bottom ---
	int bottomAreaHeight = 50;
	int bottomSepY = fgY + fgH - bottomAreaHeight;
	painter.drawLine(fgX + 1, bottomSepY, fgX + fgW - 1, bottomSepY);

	// --- Bottom area: buttons (50px) ---
	int btnH = 30;
	int btnY = bottomSepY + (bottomAreaHeight - btnH) / 2;
	int btnMargin = 15;
	int btnSpacing = 10;

	QFont btnFont = this->font();
	btnFont.setPixelSize(13);
	painter.setFont(btnFont);
	QFontMetrics fm(btnFont);

	QColor basicBase = ElaThemeColor(d->_themeMode, BasicBase);
	QColor basicText = ElaThemeColor(d->_themeMode, BasicText);
	QColor basicBorder = ElaThemeColor(d->_themeMode, BasicBorder);
	QColor primaryNormal = ElaThemeColor(d->_themeMode, PrimaryNormal);

	// Cancel button (left)
	QString cancelText = QStringLiteral("\u53D6\u6D88");
	int cancelW = fm.horizontalAdvance(cancelText) + 24;
	QRect cancelRect(fgX + btnMargin, btnY, cancelW, btnH);
	d->_cancelButtonRect = cancelRect;
	painter.setPen(QPen(basicBorder, 1));
	painter.setBrush(basicBase);
	painter.drawRoundedRect(cancelRect, 4, 4);
	painter.setPen(basicText);
	painter.drawText(cancelRect, Qt::AlignCenter, cancelText);

	// Next/Finish button (rightmost)
	bool isLastStep = (d->_pCurrentStep >= d->_stepPages.count() - 1) && (d->_stepPages.count() > 0);
	QString nextText = isLastStep ? QStringLiteral("\u5B8C\u6210") : QStringLiteral("\u4E0B\u4E00\u6B65");
	int nextW = fm.horizontalAdvance(nextText) + 24;
	QRect nextRect(fgX + fgW - btnMargin - nextW, btnY, nextW, btnH);
	d->_nextButtonRect = nextRect;
	painter.setPen(Qt::NoPen);
	painter.setBrush(primaryNormal);
	painter.drawRoundedRect(nextRect, 4, 4);
	painter.setPen(Qt::white);
	painter.drawText(nextRect, Qt::AlignCenter, nextText);

	// Previous button (before next)
	QString prevText = QStringLiteral("\u4E0A\u4E00\u6B65");
	int prevW = fm.horizontalAdvance(prevText) + 24;
	QRect prevRect(nextRect.x() - btnSpacing - prevW, btnY, prevW, btnH);
	d->_prevButtonRect = prevRect;
	bool prevDisabled = (d->_pCurrentStep == 0);
	painter.setPen(QPen(basicBorder, 1));
	painter.setBrush(basicBase);
	painter.drawRoundedRect(prevRect, 4, 4);
	if (prevDisabled)
	{
		QColor disabledText = basicText;
		disabledText.setAlpha(80);
		painter.setPen(disabledText);
	}
	else
	{
		painter.setPen(basicText);
	}
	painter.drawText(prevRect, Qt::AlignCenter, prevText);
}

void ElaWizard::mousePressEvent(QMouseEvent* event)
{
	Q_D(ElaWizard);
	QPoint pos = event->pos();
	if (d->_cancelButtonRect.contains(pos))
	{
		Q_EMIT cancelled();
		hide();
		return;
	}
	if (d->_prevButtonRect.contains(pos) && d->_pCurrentStep > 0)
	{
		previous();
		return;
	}
	if (d->_nextButtonRect.contains(pos))
	{
		bool isLastStep = (d->_pCurrentStep >= d->_stepPages.count() - 1) && (d->_stepPages.count() > 0);
		if (isLastStep)
		{
			finish();
		}
		else
		{
			next();
		}
		return;
	}
	QWidget::mousePressEvent(event);
}

// ElaWizardPrivate

ElaWizardPrivate::ElaWizardPrivate(QObject* parent)
	: QObject(parent)
{
}

ElaWizardPrivate::~ElaWizardPrivate()
{
}
