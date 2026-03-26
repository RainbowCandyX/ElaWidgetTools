#include "ElaAutoCompleteDelegate.h"
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include "ElaTheme.h"

ElaAutoCompleteDelegate::ElaAutoCompleteDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
	_themeMode = eTheme->getThemeMode();
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		_themeMode = themeMode;
	});
}

ElaAutoCompleteDelegate::~ElaAutoCompleteDelegate()
{
}

void ElaAutoCompleteDelegate::setHighlightText(const QString &text)
{
	_highlightText = text;
}

void ElaAutoCompleteDelegate::setMatchMode(ElaAutoComplete::MatchMode mode)
{
	_matchMode = mode;
}

void ElaAutoCompleteDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter->setPen(Qt::NoPen);

	QRectF itemRect = option.rect;
	itemRect.adjust(4, 2, -4, -2);
	if (option.state & QStyle::State_MouseOver)
	{
		QPainterPath hoverPath;
		hoverPath.addRoundedRect(itemRect, 4, 4);
		painter->fillPath(hoverPath, ElaThemeColor(_themeMode, BasicHover));
	}

	QString displayText = index.data(Qt::DisplayRole).toString();
	QFont textFont = option.font;
	textFont.setPixelSize(13);
	painter->setFont(textFont);

	QRectF textRect = itemRect.adjusted(10, 0, -4, 0);

	if (!_highlightText.isEmpty())
	{
		QFontMetrics fm(textFont);
		QFont boldFont = textFont;
		boldFont.setBold(true);
		QFontMetrics boldFm(boldFont);

		int searchLen = _highlightText.length();
		QVector<QPair<int, int> > matches;
		if (_matchMode == ElaAutoComplete::StartsWith)
		{
			if (displayText.startsWith(_highlightText, Qt::CaseInsensitive))
			{
				matches.append({0, searchLen});
			}
		}
		else if (_matchMode == ElaAutoComplete::EndsWith)
		{
			if (displayText.endsWith(_highlightText, Qt::CaseInsensitive))
			{
				matches.append({displayText.length() - searchLen, searchLen});
			}
		}
		else if (_matchMode == ElaAutoComplete::RegExp)
		{
			QRegularExpression re(_highlightText, QRegularExpression::CaseInsensitiveOption);
			if (re.isValid())
			{
				QRegularExpressionMatchIterator it = re.globalMatch(displayText);
				while (it.hasNext())
				{
					QRegularExpressionMatch m = it.next();
					if (m.capturedLength() > 0)
					{
						matches.append({static_cast<int>(m.capturedStart()), static_cast<int>(m.capturedLength())});
					}
				}
			}
		}
		else
		{
			int searchFrom = 0;
			while (searchFrom < displayText.length())
			{
				int idx = displayText.indexOf(_highlightText, searchFrom, Qt::CaseInsensitive);
				if (idx < 0)
				{
					break;
				}
				matches.append({idx, searchLen});
				searchFrom = idx + searchLen;
			}
		}

		if (!matches.isEmpty())
		{
			qreal x = textRect.left();
			qreal y = textRect.center().y() + fm.ascent() / 2.0 - 1;
			int pos = 0;

			for (const auto &m: matches)
			{
				if (m.first > pos)
				{
					QString segment = displayText.mid(pos, m.first - pos);
					painter->setFont(textFont);
					painter->setPen(ElaThemeColor(_themeMode, BasicText));
					painter->drawText(QPointF(x, y), segment);
					x += fm.horizontalAdvance(segment);
				}
				QString matchStr = displayText.mid(m.first, m.second);
				painter->setFont(boldFont);
				painter->setPen(ElaThemeColor(_themeMode, PrimaryNormal));
				painter->drawText(QPointF(x, y), matchStr);
				x += boldFm.horizontalAdvance(matchStr);
				pos = m.first + m.second;
			}
			if (pos < displayText.length())
			{
				QString tail = displayText.mid(pos);
				painter->setFont(textFont);
				painter->setPen(ElaThemeColor(_themeMode, BasicText));
				painter->drawText(QPointF(x, y), tail);
			}

			painter->restore();
			return;
		}
	}

	painter->setPen(ElaThemeColor(_themeMode, BasicText));
	painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, displayText);

	painter->restore();
}

QSize ElaAutoCompleteDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)
	return QSize(0, 36);
}