#include "ElaSplashScreen.h"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

#include "ElaProgressBar.h"
#include "ElaProgressRing.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "private/ElaSplashScreenPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, int, Minimum)
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, int, Maximum)
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, bool, IsShowProgressBar)
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, bool, IsShowProgressRing)
Q_PROPERTY_CREATE_Q_CPP(ElaSplashScreen, bool, IsClosable)

void ElaSplashScreen::setValue(int value)
{
	Q_D(ElaSplashScreen);
	d->_pValue = value;
	if (d->_progressBar)
	{
		d->_progressBar->setValue(value);
	}
	if (d->_progressRing)
	{
		d->_progressRing->setValue(value);
	}
}

int ElaSplashScreen::getValue() const
{
	return d_ptr->_pValue;
}

ElaSplashScreen::ElaSplashScreen(QWidget *parent)
	: QWidget{nullptr}, d_ptr(new ElaSplashScreenPrivate())
{
	Q_D(ElaSplashScreen);
	d->q_ptr = this;
	d->_pBorderRadius = 12;
	d->_pMinimum = 0;
	d->_pMaximum = 100;
	d->_pValue = 0;
	d->_pIsShowProgressBar = true;
	d->_pIsShowProgressRing = false;
	d->_pIsClosable = false;
	d->_themeMode = eTheme->getThemeMode();

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(580, 400);

	d->_titleText = new ElaText(this);
	d->_titleText->setTextStyle(ElaTextType::TitleLarge);
	d->_titleText->setAlignment(Qt::AlignCenter);

	d->_subTitleText = new ElaText(this);
	d->_subTitleText->setTextStyle(ElaTextType::Subtitle);
	d->_subTitleText->setAlignment(Qt::AlignCenter);
	d->_subTitleText->setWordWrap(true);
	d->_subTitleText->setMinimumWidth(400);

	d->_statusText = new ElaText(this);
	d->_statusText->setTextStyle(ElaTextType::Body);
	d->_statusText->setAlignment(Qt::AlignCenter);

	d->_progressBar = new ElaProgressBar(this);
	d->_progressBar->setMinimum(0);
	d->_progressBar->setMaximum(100);
	d->_progressBar->setFixedHeight(24);

	d->_progressRing = new ElaProgressRing(this);
	d->_progressRing->setFixedSize(48, 48);
	d->_progressRing->setIsBusying(true);
	d->_progressRing->setVisible(false);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(36, 50, 36, 36);
	mainLayout->setSpacing(6);
	mainLayout->addStretch(2);
	mainLayout->addWidget(d->_titleText, 0, Qt::AlignCenter);
	mainLayout->addSpacing(4);
	mainLayout->addWidget(d->_subTitleText, 0, Qt::AlignCenter);
	mainLayout->addStretch(1);
	mainLayout->addWidget(d->_progressRing, 0, Qt::AlignCenter);
	mainLayout->addSpacing(8);
	mainLayout->addWidget(d->_statusText, 0, Qt::AlignCenter);
	mainLayout->addSpacing(12);
	mainLayout->addWidget(d->_progressBar);
	mainLayout->addStretch(1);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaSplashScreen::ElaSplashScreen(const QPixmap &logo, QWidget *parent)
	: ElaSplashScreen(parent)
{
	Q_D(ElaSplashScreen);
	d->_logo = logo;
}

ElaSplashScreen::~ElaSplashScreen()
{
}

void ElaSplashScreen::setLogo(const QPixmap &logo)
{
	Q_D(ElaSplashScreen);
	d->_logo = logo;
	update();
}

void ElaSplashScreen::setTitle(const QString &title)
{
	Q_D(ElaSplashScreen);
	d->_titleText->setText(title);
}

void ElaSplashScreen::setSubTitle(const QString &subTitle)
{
	Q_D(ElaSplashScreen);
	d->_subTitleText->setText(subTitle);
}

void ElaSplashScreen::setStatusText(const QString &text)
{
	Q_D(ElaSplashScreen);
	d->_statusText->setText(text);
}

void ElaSplashScreen::show()
{
	Q_D(ElaSplashScreen);
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
	{
		QRect screenRect = screen->availableGeometry();
		move((screenRect.width() - width()) / 2, (screenRect.height() - height()) / 2);
	}
	d->_progressBar->setVisible(d->_pIsShowProgressBar);
	d->_progressRing->setVisible(d->_pIsShowProgressRing);
	QWidget::show();
	d->_doShowAnimation();
}

void ElaSplashScreen::close()
{
	Q_D(ElaSplashScreen);
	d->_doCloseAnimation();
}

void ElaSplashScreen::finish(QWidget *mainWindow)
{
	Q_D(ElaSplashScreen);
	if (mainWindow)
	{
		QPropertyAnimation *opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
		opacityAnimation->setDuration(300);
		opacityAnimation->setStartValue(1.0);
		opacityAnimation->setEndValue(0.0);
		opacityAnimation->setEasingCurve(QEasingCurve::InOutCubic);
		connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]()
		{
			QWidget::close();
			Q_EMIT closed();
			mainWindow->show();
			mainWindow->raise();
			mainWindow->activateWindow();
		});
		opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	}
}

void ElaSplashScreen::paintEvent(QPaintEvent *event)
{
	Q_D(ElaSplashScreen);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	eTheme->drawEffectShadow(&painter, rect(), 6, d->_pBorderRadius);

	QRect foregroundRect = rect().adjusted(6, 6, -6, -6);
	QPainterPath path;
	path.addRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
	painter.setClipPath(path);
	painter.setPen(Qt::NoPen);
	painter.setBrush(ElaThemeColor(d->_themeMode, DialogBase));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	if (!d->_logo.isNull())
	{
		int logoSize = qMin(80, qMin(foregroundRect.width(), foregroundRect.height()) / 3);
		QRect logoRect(foregroundRect.x() + (foregroundRect.width() - logoSize) / 2,
		               foregroundRect.y() + 30, logoSize, logoSize);
		painter.drawPixmap(logoRect, d->_logo.scaled(logoSize, logoSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}

	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, PopupBorder), 1));
	painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

	painter.restore();
}

void ElaSplashScreen::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaSplashScreen);
	if (event->button() == Qt::LeftButton)
	{
		d->_isDragging = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		d->_dragStartPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
#else
		d->_dragStartPos = event->globalPos() - frameGeometry().topLeft();
#endif
		event->accept();
	}
}

void ElaSplashScreen::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(ElaSplashScreen);
	if (d->_isDragging)
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		move(event->globalPosition().toPoint() - d->_dragStartPos);
#else
		move(event->globalPos() - d->_dragStartPos);
#endif
		event->accept();
	}
}

void ElaSplashScreen::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaSplashScreen);
	d->_isDragging = false;
	QWidget::mouseReleaseEvent(event);
}
