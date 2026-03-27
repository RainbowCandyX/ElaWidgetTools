#include "ElaUploadArea.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QPainter>
#include <QPainterPath>
#include <QUrl>

#include "ElaTheme.h"
#include "private/ElaUploadAreaPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaUploadArea, int, BorderRadius)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaUploadArea, QString, Title)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaUploadArea, QString, SubTitle)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaUploadArea, QStringList, AcceptedSuffixes)
Q_PROPERTY_CREATE_Q_CPP(ElaUploadArea, int, MaxFileCount)
Q_PROPERTY_CREATE_Q_CPP(ElaUploadArea, qint64, MaxFileSize)
Q_PROPERTY_CREATE_Q_CPP(ElaUploadArea, bool, IsMultiple)
Q_PROPERTY_REF_CREATE_Q_CPP(ElaUploadArea, QString, DialogTitle)

ElaUploadArea::ElaUploadArea(QWidget *parent) : QWidget(parent), d_ptr(new ElaUploadAreaPrivate())
{
	Q_D(ElaUploadArea);
	d->q_ptr = this;
	d->_pBorderRadius = 8;
	d->_pTitle = "拖拽文件到此处";
	d->_pSubTitle = "或点击选择文件";
	d->_pMaxFileCount = 0;
	d->_pMaxFileSize = 0;
	d->_pIsMultiple = true;
	d->_themeMode = eTheme->getThemeMode();
	setObjectName("ElaUploadArea");
	setMinimumSize(260, 160);
	setAcceptDrops(true);
	setMouseTracking(true);
	setCursor(Qt::PointingHandCursor);

	connect(eTheme, &ElaTheme::themeModeChanged, d, &ElaUploadAreaPrivate::onThemeChanged);
}

ElaUploadArea::~ElaUploadArea()
{
}

QStringList ElaUploadArea::getSelectedFiles() const
{
	return d_ptr->_filePaths;
}

void ElaUploadArea::clearFiles()
{
	Q_D(ElaUploadArea);
	d->_filePaths.clear();
	update();
}

void ElaUploadArea::setAcceptedMimeFilter(const QString &filter)
{
	Q_D(ElaUploadArea);
	d->_mimeFilter = filter;
}

QString ElaUploadArea::getAcceptedMimeFilter() const
{
	return d_ptr->_mimeFilter;
}

void ElaUploadArea::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaUploadArea);
	QPainter painter(this);
	painter.save();
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QRectF bgRect(1, 1, width() - 2, height() - 2);
	QPainterPath bgPath;
	bgPath.addRoundedRect(bgRect, d->_pBorderRadius, d->_pBorderRadius);

	if (d->_isDragOver)
	{
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, PrimaryNormal), 2, Qt::DashLine));
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicBaseDeepAlpha));
	}
	else if (d->_isHover)
	{
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorderHover), 2, Qt::DashLine));
		painter.setBrush(ElaThemeColor(d->_themeMode, BasicHoverAlpha));
	}
	else
	{
		painter.setPen(QPen(ElaThemeColor(d->_themeMode, BasicBorder), 2, Qt::DashLine));
		painter.setBrush(Qt::NoBrush);
	}
	painter.drawPath(bgPath);

	int contentY = height() / 2;
	int fileCount = d->_filePaths.size();

	if (fileCount == 0)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(36);
		painter.setFont(iconFont);
		painter.setPen(d->_isDragOver
			               ? ElaThemeColor(d->_themeMode, PrimaryNormal)
			               : ElaThemeColor(d->_themeMode, BasicTextNoFocus));
		QRectF iconRect(0, contentY - 48, width(), 40);
		painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::CloudArrowUp));

		QFont titleFont = painter.font();
		titleFont.setFamily(qApp->font().family());
		titleFont.setPixelSize(15);
		titleFont.setBold(true);
		painter.setFont(titleFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		QRectF titleRect(0, contentY, width(), 24);
		painter.drawText(titleRect, Qt::AlignCenter, d->_pTitle);

		QFont subFont = titleFont;
		subFont.setPixelSize(12);
		subFont.setBold(false);
		painter.setFont(subFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
		QRectF subRect(0, contentY + 26, width(), 20);
		painter.drawText(subRect, Qt::AlignCenter, d->_pSubTitle);
	}
	else
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(24);
		painter.setFont(iconFont);
		painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
		QRectF iconRect(0, 12, width(), 28);
		painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) ElaIconType::CloudArrowUp));

		QFont fileFont = painter.font();
		fileFont.setFamily(qApp->font().family());
		fileFont.setPixelSize(12);
		painter.setFont(fileFont);

		int startY = 46;
		int lineHeight = 22;
		int maxDisplayCount = qMin(fileCount, (height() - startY - 10) / lineHeight);

		for (int i = 0; i < maxDisplayCount; ++i)
		{
			QFileInfo fileInfo(d->_filePaths[i]);
			QString displayName = fileInfo.fileName();

			painter.setPen(ElaThemeColor(d->_themeMode, PrimaryNormal));
			QFont fileIconFont("ElaAwesome");
			fileIconFont.setPixelSize(12);
			painter.setFont(fileIconFont);
			QRectF fileIconRect(12, startY + i * lineHeight, 16, lineHeight);
			painter.drawText(fileIconRect, Qt::AlignVCenter | Qt::AlignCenter,
			                 QChar((unsigned short) ElaIconType::File));

			painter.setFont(fileFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
			QRectF nameRect(32, startY + i * lineHeight, width() - 60, lineHeight);
			QString elidedName = painter.fontMetrics().elidedText(displayName, Qt::ElideMiddle, nameRect.width());
			painter.drawText(nameRect, Qt::AlignVCenter | Qt::AlignLeft, elidedName);

			painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
			QFont xFont("ElaAwesome");
			xFont.setPixelSize(10);
			painter.setFont(xFont);
			QRectF xRect(width() - 28, startY + i * lineHeight, 16, lineHeight);
			painter.drawText(xRect, Qt::AlignVCenter | Qt::AlignCenter,
			                 QChar((unsigned short) ElaIconType::Xmark));
		}

		if (fileCount > maxDisplayCount)
		{
			painter.setFont(fileFont);
			painter.setPen(ElaThemeColor(d->_themeMode, BasicTextNoFocus));
			QRectF moreRect(0, startY + maxDisplayCount * lineHeight, width(), lineHeight);
			painter.drawText(moreRect, Qt::AlignCenter,
			                 QString("...还有 %1 个文件").arg(fileCount - maxDisplayCount));
		}
	}

	painter.restore();
}

void ElaUploadArea::dragEnterEvent(QDragEnterEvent *event)
{
	Q_D(ElaUploadArea);
	if (event->mimeData()->hasUrls())
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();
		d->_isDragOver = true;
		update();
	}
}

void ElaUploadArea::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasUrls())
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();
	}
}

void ElaUploadArea::dragLeaveEvent(QDragLeaveEvent *event)
{
	Q_UNUSED(event)
	Q_D(ElaUploadArea);
	d->_isDragOver = false;
	update();
}

void ElaUploadArea::dropEvent(QDropEvent *event)
{
	Q_D(ElaUploadArea);
	event->accept();
	d->_isDragOver = false;
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls())
	{
		QStringList filePaths;
		for (const QUrl &url: mimeData->urls())
		{
			if (url.isLocalFile())
			{
				filePaths.append(url.toLocalFile());
			}
		}
		if (!filePaths.isEmpty())
		{
			if (!d->_pIsMultiple && filePaths.size() > 1)
			{
				filePaths = filePaths.mid(0, 1);
			}
			d->_addFiles(filePaths);
		}
	}
	update();
}

void ElaUploadArea::mousePressEvent(QMouseEvent *event)
{
	Q_D(ElaUploadArea);
	d->_isPressed = true;

	if (!d->_filePaths.isEmpty())
	{
		int startY = 46;
		int lineHeight = 22;
		int maxDisplayCount = qMin(d->_filePaths.size(), (height() - startY - 10) / lineHeight);
		int clickY = event->pos().y();
		int clickX = event->pos().x();

		for (int i = 0; i < maxDisplayCount; ++i)
		{
			int itemY = startY + i * lineHeight;
			if (clickY >= itemY && clickY < itemY + lineHeight &&
			    clickX >= width() - 28 && clickX <= width() - 12)
			{
				d->_removeFile(i);
				d->_isPressed = false;
				return;
			}
		}
	}

	update();
	QWidget::mousePressEvent(event);
}

void ElaUploadArea::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(ElaUploadArea);
	if (d->_isPressed && rect().contains(event->pos()))
	{
		d->_openFileDialog();
	}
	d->_isPressed = false;
	update();
	QWidget::mouseReleaseEvent(event);
}

void ElaUploadArea::enterEvent(QEnterEvent *event)
{
	Q_D(ElaUploadArea);
	d->_isHover = true;
	update();
	QWidget::enterEvent(event);
}

void ElaUploadArea::leaveEvent(QEvent *event)
{
	Q_D(ElaUploadArea);
	d->_isHover = false;
	update();
	QWidget::leaveEvent(event);
}