#include "ElaTheme.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QStyleHints>

#include "ElaThemePrivate.h"
Q_SINGLETON_CREATE_CPP(ElaTheme)

ElaTheme::ElaTheme(QObject *parent)
	: QObject{parent}, d_ptr(new ElaThemePrivate())
{
	Q_D(ElaTheme);
	d->q_ptr = this;
	d->_initThemeColor();
}

ElaTheme::~ElaTheme()
{
}

void ElaTheme::setThemeMode(ElaThemeType::ThemeMode themeMode)
{
	Q_D(ElaTheme);
	d->_themeMode = themeMode;
	Q_EMIT themeModeChanged(d->_themeMode);
}

ElaThemeType::ThemeMode ElaTheme::getThemeMode() const
{
	Q_D(const ElaTheme);
	return d->_themeMode;
}

void ElaTheme::setIsFollowSystemTheme(bool isFollow)
{
	Q_D(ElaTheme);
	if (d->_isFollowSystemTheme == isFollow)
	{
		return;
	}
	d->_isFollowSystemTheme = isFollow;
	if (isFollow)
	{
		if (QCoreApplication::instance())
		{
			QCoreApplication::instance()->installEventFilter(d);
		}
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
		d->_colorSchemeConnection = connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged, d, [d](Qt::ColorScheme) { d->_applySystemTheme(); });
#endif
		d->_applySystemTheme();
	}
	else
	{
		if (QCoreApplication::instance())
		{
			QCoreApplication::instance()->removeEventFilter(d);
		}
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
		if (d->_colorSchemeConnection)
		{
			disconnect(d->_colorSchemeConnection);
			d->_colorSchemeConnection = {};
		}
#endif
	}
	Q_EMIT pIsFollowSystemThemeChanged(isFollow);
}

bool ElaTheme::getIsFollowSystemTheme() const
{
	Q_D(const ElaTheme);
	return d->_isFollowSystemTheme;
}

void ElaTheme::drawEffectShadow(QPainter *painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
	Q_D(ElaTheme);
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing);
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	QColor color = d->_themeMode == ElaThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
	for (int i = 0; i < shadowBorderWidth; i++)
	{
		path.addRoundedRect(widgetRect.x() + shadowBorderWidth - i, widgetRect.y() + shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
		int alpha = 1 * (shadowBorderWidth - i + 1);
		color.setAlpha(alpha > 255 ? 255 : alpha);
		painter->setPen(color);
		painter->drawPath(path);
	}
	painter->restore();
}

void ElaTheme::setThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor, QColor newColor)
{
	Q_D(ElaTheme);
	if (themeMode == ElaThemeType::Light)
	{
		d->_lightThemeColorList[themeColor] = newColor;
	}
	else
	{
		d->_darkThemeColorList[themeColor] = newColor;
	}
}

const QColor &ElaTheme::getThemeColor(ElaThemeType::ThemeMode themeMode, ElaThemeType::ThemeColor themeColor)
{
	Q_D(ElaTheme);
	if (themeMode == ElaThemeType::Light)
	{
		return d->_lightThemeColorList[themeColor];
	}
	else
	{
		return d->_darkThemeColorList[themeColor];
	}
}