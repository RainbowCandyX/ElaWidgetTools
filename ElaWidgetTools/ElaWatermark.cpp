#include "ElaWatermark.h"

#include <QEvent>
#include <QPainter>
#include <QtMath>

#include "ElaTheme.h"
#include "private/ElaWatermarkPrivate.h"

Q_PROPERTY_REF_CREATE_Q_CPP(ElaWatermark, QString, Text)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, int, FontPixelSize)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, qreal, Opacity)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, qreal, Rotation)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, int, GapX)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, int, GapY)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaWatermark, QColor, TextColor)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaWatermark, QImage, Image)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, int, ImageWidth)
Q_PROPERTY_CREATE_Q_CPP(ElaWatermark, int, ImageHeight)

ElaWatermarkPrivate::ElaWatermarkPrivate(QObject *parent)
	: QObject(parent)
{
}

ElaWatermarkPrivate::~ElaWatermarkPrivate()
{
}

void ElaWatermarkPrivate::rebuildPattern()
{
	Q_Q(ElaWatermark);
	_patternDirty = false;

	bool hasText = !_pText.isEmpty();
	bool hasImage = !_pImage.isNull();
	if (!hasText && !hasImage)
	{
		_patternPixmap = QPixmap();
		return;
	}

	QFont font;
	font.setPixelSize(_pFontPixelSize);
	QFontMetrics fm(font);

	int contentWidth = 0;
	int contentHeight = 0;
	if (hasText)
	{
		contentWidth = fm.horizontalAdvance(_pText);
		contentHeight = fm.height();
	}
	if (hasImage)
	{
		int imgW = _pImageWidth > 0 ? _pImageWidth : _pImage.width();
		int imgH = _pImageHeight > 0 ? _pImageHeight : _pImage.height();
		if (hasText)
		{
			contentWidth = qMax(contentWidth, imgW);
			contentHeight += 4 + imgH;
		}
		else
		{
			contentWidth = imgW;
			contentHeight = imgH;
		}
	}

	if (contentWidth <= 0 || contentHeight <= 0)
	{
		_patternPixmap = QPixmap();
		return;
	}

	int cellWidth = contentWidth + _pGapX;
	int cellHeight = contentHeight + _pGapY;

	qreal radians = qDegreesToRadians(_pRotation);
	int rotatedWidth = qCeil(qAbs(cellWidth * qCos(radians)) + qAbs(cellHeight * qSin(radians)));
	int rotatedHeight = qCeil(qAbs(cellWidth * qSin(radians)) + qAbs(cellHeight * qCos(radians)));

	qreal dpr = q->devicePixelRatioF();
	QPixmap tile(QSize(rotatedWidth, rotatedHeight) * dpr);
	tile.setDevicePixelRatio(dpr);
	tile.fill(Qt::transparent);

	QPainter painter(&tile);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	painter.translate(rotatedWidth / 2.0, rotatedHeight / 2.0);
	painter.rotate(_pRotation);
	painter.translate(-cellWidth / 2.0, -cellHeight / 2.0);

	int yOffset = 0;
	if (hasImage)
	{
		int imgW = _pImageWidth > 0 ? _pImageWidth : _pImage.width();
		int imgH = _pImageHeight > 0 ? _pImageHeight : _pImage.height();
		int imgX = (contentWidth - imgW) / 2;
		painter.setOpacity(_pOpacity);
		painter.drawImage(QRect(imgX, 0, imgW, imgH), _pImage);
		yOffset = imgH + 4;
	}

	if (hasText)
	{
		QColor color = _pTextColor.isValid() ? _pTextColor : ElaThemeColor(_themeMode, BasicText);
		color.setAlphaF(color.alphaF() * _pOpacity);
		painter.setOpacity(1.0);
		painter.setFont(font);
		painter.setPen(color);
		painter.drawText(QRect(0, yOffset, contentWidth, fm.height()), Qt::AlignCenter, _pText);
	}

	painter.end();
	_patternPixmap = tile;
}

ElaWatermark::ElaWatermark(QWidget *parent)
	: QWidget{parent}, d_ptr(new ElaWatermarkPrivate())
{
	Q_D(ElaWatermark);
	d->q_ptr = this;
	d->_pText = "";
	d->_pFontPixelSize = 14;
	d->_pOpacity = 0.12;
	d->_pRotation = -22;
	d->_pGapX = 80;
	d->_pGapY = 60;
	d->_pTextColor = QColor();
	d->_pImage = QImage();
	d->_pImageWidth = 0;
	d->_pImageHeight = 0;
	setObjectName("ElaWatermark");

	setAttribute(Qt::WA_TransparentForMouseEvents);
	setAttribute(Qt::WA_TranslucentBackground);
	setFocusPolicy(Qt::NoFocus);

	if (parent)
	{
		parent->installEventFilter(this);
		resize(parent->size());
		raise();
	}

	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		d->_patternDirty = true;
		update();
	});

	auto markDirty = [=]()
	{
		d->_patternDirty = true;
		update();
	};
	connect(this, &ElaWatermark::pTextChanged, this, markDirty);
	connect(this, &ElaWatermark::pFontPixelSizeChanged, this, markDirty);
	connect(this, &ElaWatermark::pOpacityChanged, this, markDirty);
	connect(this, &ElaWatermark::pRotationChanged, this, markDirty);
	connect(this, &ElaWatermark::pGapXChanged, this, markDirty);
	connect(this, &ElaWatermark::pGapYChanged, this, markDirty);
	connect(this, &ElaWatermark::pTextColorChanged, this, markDirty);
	connect(this, &ElaWatermark::pImageChanged, this, markDirty);
	connect(this, &ElaWatermark::pImageWidthChanged, this, markDirty);
	connect(this, &ElaWatermark::pImageHeightChanged, this, markDirty);
}

ElaWatermark::ElaWatermark(const QString &text, QWidget *parent)
	: ElaWatermark(parent)
{
	Q_D(ElaWatermark);
	d->_pText = text;
	d->_patternDirty = true;
}

ElaWatermark::~ElaWatermark()
{
}

bool ElaWatermark::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == parent() && event->type() == QEvent::Resize)
	{
		resize(static_cast<QWidget *>(parent())->size());
		raise();
	}
	return QWidget::eventFilter(watched, event);
}

void ElaWatermark::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaWatermark);

	if (d->_patternDirty)
	{
		d->rebuildPattern();
	}

	if (d->_patternPixmap.isNull())
	{
		return;
	}

	QPainter painter(this);
	painter.drawTiledPixmap(rect(), d->_patternPixmap);
}