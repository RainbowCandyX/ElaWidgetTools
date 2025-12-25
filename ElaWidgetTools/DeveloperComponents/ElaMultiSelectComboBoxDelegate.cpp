#include "ElaMultiSelectComboBoxDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"

ElaMultiSelectComboBoxDelegate::ElaMultiSelectComboBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		_themeMode = themeMode;
	});
}

ElaMultiSelectComboBoxDelegate::~ElaMultiSelectComboBoxDelegate()
{
}

void ElaMultiSelectComboBoxDelegate::setItemSelection(const QVector<bool> &selection)
{
	_itemSelection = selection;
}

void ElaMultiSelectComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

	int margin = 2;
	QRect bgRect = option.rect;
	bgRect.adjust(margin, margin, -margin, -margin);
#ifndef Q_OS_WIN
	bgRect.adjust(6, 0, -6, 0);
#endif

	QPainterPath bgPath;
	bgPath.addRoundedRect(bgRect, 5, 5);

	bool isSelected = index.row() < _itemSelection.size() && _itemSelection[index.row()];

	painter->setPen(Qt::NoPen);
	if (isSelected)
	{
		if (option.state & QStyle::State_MouseOver)
		{
			painter->fillPath(bgPath, ElaThemeColor(_themeMode, BasicSelectedHoverAlpha));
		}
		else
		{
			painter->fillPath(bgPath, ElaThemeColor(_themeMode, BasicSelectedAlpha));
		}
	}
	else
	{
		if (option.state & QStyle::State_MouseOver)
		{
			painter->fillPath(bgPath, ElaThemeColor(_themeMode, BasicHoverAlpha));
		}
	}

	int checkBoxSize = 21;
	QRect checkBoxRect(bgRect.x() + 8,
	                   bgRect.y() + (bgRect.height() - checkBoxSize) / 2,
	                   checkBoxSize, checkBoxSize);
	checkBoxRect.adjust(1, 1, -1, -1);

	painter->setPen(Qt::NoPen);
	if (isSelected)
	{
		if (option.state & QStyle::State_MouseOver)
		{
			painter->setBrush(ElaThemeColor(_themeMode, PrimaryHover));
		}
		else
		{
			painter->setBrush(ElaThemeColor(_themeMode, PrimaryNormal));
		}
	}
	else
	{
		painter->setPen(ElaThemeColor(_themeMode, BasicBorderDeep));
		if (option.state & QStyle::State_MouseOver)
		{
			painter->setBrush(ElaThemeColor(_themeMode, BasicHover));
		}
		else
		{
			painter->setBrush(ElaThemeColor(_themeMode, BasicBase));
		}
	}
	painter->drawRoundedRect(checkBoxRect, 2, 2);

	if (isSelected)
	{
		painter->setPen(ElaThemeColor(ElaThemeType::Dark, BasicText));
		QFont iconFont("ElaAwesome");
		iconFont.setPixelSize(checkBoxSize * 0.75);
		painter->setFont(iconFont);
		painter->drawText(checkBoxRect, Qt::AlignCenter,
		                  QChar((unsigned short) ElaIconType::Check));
	}

	painter->setPen(ElaThemeColor(_themeMode, BasicText));
	QRect textRect(bgRect.x() + checkBoxSize + 18, bgRect.y(),
	               bgRect.width() - checkBoxSize - 18, bgRect.height());
	painter->drawText(textRect, Qt::AlignVCenter,
	                  index.data(Qt::DisplayRole).toString());

	painter->restore();
}

QSize ElaMultiSelectComboBoxDelegate::sizeHint(const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
	return QSize(option.rect.width(), 35);
}