#include "ElaPopconfirm.h"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QVBoxLayout>

#include "ElaIcon.h"
#include "ElaPushButton.h"
#include "ElaTheme.h"
#include "private/ElaPopconfirmPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, QString, ConfirmButtonText)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, QString, CancelButtonText)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, ElaIconType::IconName, Icon)
Q_PROPERTY_CREATE_Q_CPP(ElaPopconfirm, bool, IsLightDismiss)

ElaPopconfirm::ElaPopconfirm(QWidget *parent) : QWidget{nullptr}, d_ptr(new ElaPopconfirmPrivate())
{
	Q_D(ElaPopconfirm);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pConfirmButtonText = "确定";
	d->_pCancelButtonText = "取消";
	d->_pIcon = ElaIconType::CircleExclamation;
	d->_pIsLightDismiss = true;
	d->_themeMode = eTheme->getThemeMode();

	setObjectName("ElaPopconfirm");
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	d->_opacityEffect = new QGraphicsOpacityEffect(this);
	d->_opacityEffect->setOpacity(0);
	setGraphicsEffect(d->_opacityEffect);

	d->_cancelButton = new ElaPushButton(d->_pCancelButtonText, this);
	d->_cancelButton->setFixedHeight(30);
	d->_cancelButton->setBorderRadius(4);
	connect(d->_cancelButton, &ElaPushButton::clicked, this, [=]()
	{
		Q_EMIT cancelled();
		closePopconfirm();
	});

	d->_confirmButton = new ElaPushButton(d->_pConfirmButtonText, this);
	d->_confirmButton->setFixedHeight(30);
	d->_confirmButton->setBorderRadius(4);
	d->_confirmButton->setLightDefaultColor(ElaThemeColor(ElaThemeType::Light, PrimaryNormal));
	d->_confirmButton->setLightHoverColor(ElaThemeColor(ElaThemeType::Light, PrimaryHover));
	d->_confirmButton->setLightPressColor(ElaThemeColor(ElaThemeType::Light, PrimaryPress));
	d->_confirmButton->setLightTextColor(Qt::white);
	d->_confirmButton->setDarkDefaultColor(ElaThemeColor(ElaThemeType::Dark, PrimaryNormal));
	d->_confirmButton->setDarkHoverColor(ElaThemeColor(ElaThemeType::Dark, PrimaryHover));
	d->_confirmButton->setDarkPressColor(ElaThemeColor(ElaThemeType::Dark, PrimaryPress));
	d->_confirmButton->setDarkTextColor(Qt::black);
	connect(d->_confirmButton, &ElaPushButton::clicked, this, [=]()
	{
		Q_EMIT confirmed();
		closePopconfirm();
	});

	d->_mainLayout = new QVBoxLayout(this);
	int sw = d->_shadowBorderWidth;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
	d->_mainLayout->setContentsMargins(sw, sw, sw, sw);
#else
	d->_mainLayout->setContentsMargins(sw * 2, sw * 2, sw * 2, sw * 2);
#endif

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->setSpacing(8);
	buttonLayout->addStretch();
	buttonLayout->addWidget(d->_cancelButton);
	buttonLayout->addWidget(d->_confirmButton);
	d->_mainLayout->addStretch();
	d->_mainLayout->addLayout(buttonLayout);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
	hide();
}

ElaPopconfirm::~ElaPopconfirm()
{
}

void ElaPopconfirm::showPopconfirm(QWidget *target)
{
	Q_D(ElaPopconfirm);
	if (!target)
	{
		return;
	}
	d->_cancelButton->setText(d->_pCancelButtonText);
	d->_confirmButton->setText(d->_pConfirmButtonText);

	int sw = d->_shadowBorderWidth;
	int popupWidth = 280;
	int contentHeight = sw * 2 + 16;
	int textWidth = popupWidth - sw * 4 - 40;

	if (d->_pIcon != ElaIconType::None)
	{
		contentHeight += 0;
	}
	if (!d->_pTitle.isEmpty())
	{
		QFont titleFont = font();
		titleFont.setPixelSize(14);
		titleFont.setBold(true);
		QFontMetrics tfm(titleFont);
		contentHeight += tfm.height() + 6;
	}
	if (!d->_pContent.isEmpty())
	{
		QFont contentFont = font();
		contentFont.setPixelSize(13);
		QFontMetrics cfm(contentFont);
		QRect br = cfm.boundingRect(QRect(0, 0, textWidth, 0), Qt::TextWordWrap, d->_pContent);
		contentHeight += br.height() + 8;
	}
	contentHeight += 38;
	setFixedSize(popupWidth, contentHeight);

	QPoint targetGlobal = target->mapToGlobal(QPoint(0, target->height()));
	int x = targetGlobal.x() + (target->width() - width()) / 2;
	int y = targetGlobal.y() + 5;

	QScreen *screen = QApplication::screenAt(targetGlobal);
	if (screen)
	{
		QRect screenRect = screen->availableGeometry();
		if (x < screenRect.left())
		{
			x = screenRect.left();
		}
		if (x + width() > screenRect.right())
		{
			x = screenRect.right() - width();
		}
		if (y + height() > screenRect.bottom())
		{
			y = targetGlobal.y() - target->height() - height() - 5;
		}
		if (y < screenRect.top())
		{
			y = screenRect.top();
		}
	}
	move(x, y);
	show();
	raise();
	if (d->_pIsLightDismiss)
	{
		qApp->installEventFilter(d);
	}
	d->_doShowAnimation();
}

void ElaPopconfirm::closePopconfirm()
{
	Q_D(ElaPopconfirm);
	qApp->removeEventFilter(d);
	hide();
	Q_EMIT closed();
}

void ElaPopconfirm::paintEvent(QPaintEvent *event)
{
	Q_D(ElaPopconfirm);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	int sw = d->_shadowBorderWidth;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
	QRect fg = rect();
#else
	eTheme->drawEffectShadow(&painter, rect(), sw, d->_pBorderRadius);
	QRect fg = rect().adjusted(sw, sw, -sw, -sw);
#endif
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(fg, d->_pBorderRadius, d->_pBorderRadius);

	int textX = fg.x() + 14;
	int textY = fg.y() + 14;
	int textWidth = fg.width() - 28;
	int iconOffset = 0;

	if (d->_pIcon != ElaIconType::None)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(18);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
		painter.drawText(QRect(textX, textY - 2, 22, 22), Qt::AlignCenter, QChar(static_cast<unsigned short>(d->_pIcon)));
		iconOffset = 26;
	}

	if (!d->_pTitle.isEmpty())
	{
		QFont titleFont = painter.font();
		titleFont.setFamily(font().family());
		titleFont.setPixelSize(14);
		titleFont.setBold(true);
		painter.setFont(titleFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QRect titleRect(textX + iconOffset, textY, textWidth - iconOffset, painter.fontMetrics().height());
		painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, d->_pTitle);
		textY += titleRect.height() + 6;
	}

	if (!d->_pContent.isEmpty())
	{
		QFont contentFont = painter.font();
		contentFont.setPixelSize(13);
		contentFont.setBold(false);
		painter.setFont(contentFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicDetailsText));
		QRect contentRect(textX + iconOffset, textY, textWidth - iconOffset, fg.bottom() - textY - 42);
		painter.drawText(contentRect, Qt::AlignLeft | Qt::TextWordWrap, d->_pContent);
	}

	painter.restore();
}

ElaPopconfirmPrivate::ElaPopconfirmPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaPopconfirmPrivate::~ElaPopconfirmPrivate()
{
}

bool ElaPopconfirmPrivate::eventFilter(QObject *watched, QEvent *event)
{
	Q_Q(ElaPopconfirm);
	if (event->type() == QEvent::MouseButtonPress)
	{
		QWidget *clickedWidget = qobject_cast<QWidget *>(watched);
		if (clickedWidget && !q->isAncestorOf(clickedWidget) && clickedWidget != q)
		{
			Q_EMIT q->cancelled();
			q->closePopconfirm();
			return false;
		}
	}
	return QObject::eventFilter(watched, event);
}

void ElaPopconfirmPrivate::_doShowAnimation()
{
	QPropertyAnimation *animation = new QPropertyAnimation(_opacityEffect, "opacity");
	animation->setDuration(250);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->setEasingCurve(QEasingCurve::InOutCubic);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}