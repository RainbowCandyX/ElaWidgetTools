#include "ElaPersonPicture.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaPersonPicturePrivate.h"
Q_PROPERTY_CREATE_Q_CPP(ElaPersonPicture, QString, DisplayName)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaPersonPicture, QPixmap, Picture)

void ElaPersonPicture::setPictureSize(int PictureSize)
{
	Q_D(ElaPersonPicture);
	d->_pPictureSize = PictureSize;
	setFixedSize(PictureSize, PictureSize);
	Q_EMIT pPictureSizeChanged();
}

int ElaPersonPicture::getPictureSize() const
{
	return d_ptr->_pPictureSize;
}

ElaPersonPicture::ElaPersonPicture(QWidget *parent)
	: QWidget(parent), d_ptr(new ElaPersonPicturePrivate())
{
	Q_D(ElaPersonPicture);
	d->q_ptr = this;
	d->_pPictureSize = 40;
	setFixedSize(d->_pPictureSize, d->_pPictureSize);
	setObjectName("ElaPersonPicture");
	d->_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaPersonPicture::~ElaPersonPicture()
{
}

QSize ElaPersonPicture::sizeHint() const
{
	int size = d_ptr->_pPictureSize;
	return QSize(size, size);
}

void ElaPersonPicture::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaPersonPicture);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int size = d->_pPictureSize;
	qreal borderWidth = 2.0;
	QRectF circleRect(borderWidth / 2, borderWidth / 2, size - borderWidth, size - borderWidth);
	QPointF center = circleRect.center();
	qreal radius = circleRect.width() / 2;

	if (!d->_pPicture.isNull())
	{
		// Draw circular-clipped pixmap
		painter.save();
		QPainterPath clipPath;
		clipPath.addEllipse(center, radius, radius);
		painter.setClipPath(clipPath);
		painter.drawPixmap(circleRect.toRect(), d->_pPicture.scaled(circleRect.size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
		painter.restore();
	}
	else if (!d->_pDisplayName.isEmpty())
	{
		// Draw circle with PrimaryNormal background and white initials
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, PrimaryNormal));
		painter.drawEllipse(center, radius, radius);
		painter.restore();

		painter.save();
		QString initials = d->_pDisplayName.left(2);
		QFont font = this->font();
		font.setPixelSize(size / 3);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(Qt::white);
		painter.drawText(circleRect.toRect(), Qt::AlignCenter, initials);
		painter.restore();
	}
	else
	{
		// Draw circle with BasicBase background and user icon
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicBase));
		painter.drawEllipse(center, radius, radius);
		painter.restore();

		painter.save();
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(size / 3);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		painter.drawText(circleRect.toRect(), Qt::AlignCenter, QChar((unsigned short) ElaIconType::User));
		painter.restore();
	}

	// Draw 2px circular border
	painter.save();
	painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), borderWidth));
	painter.setBrush(Qt::NoBrush);
	painter.drawEllipse(center, radius, radius);
	painter.restore();
}

ElaPersonPicturePrivate::ElaPersonPicturePrivate(QObject *parent)
	: QObject(parent)
{
}

ElaPersonPicturePrivate::~ElaPersonPicturePrivate()
{
}