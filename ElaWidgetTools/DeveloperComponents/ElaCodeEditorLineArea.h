#ifndef ELACODEEDITORLINEAREA_H
#define ELACODEEDITORLINEAREA_H

#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QWidget>

#include "ElaTheme.h"
#include "private/ElaCodeEditorPrivate.h"

// Expose protected methods of QPlainTextEdit
class ElaCodePlainTextEdit : public QPlainTextEdit
{
public:
    using QPlainTextEdit::QPlainTextEdit;
    QTextBlock getFirstVisibleBlock() const { return firstVisibleBlock(); }
    QRectF getBlockBoundingGeometry(const QTextBlock& block) const { return blockBoundingGeometry(block); }
    QRectF getBlockBoundingRect(const QTextBlock& block) const { return blockBoundingRect(block); }
    QPointF getContentOffset() const { return contentOffset(); }
};

class ElaCodeEditorLineArea : public QWidget
{
public:
	explicit ElaCodeEditorLineArea(ElaCodePlainTextEdit *editor, ElaCodeEditorPrivate *d, QWidget *parent = nullptr)
		: QWidget(parent), _editor(editor), _d(d)
	{
	}

	QSize sizeHint() const override
	{
		return QSize(lineAreaWidth(), 0);
	}

	int lineAreaWidth() const
	{
		int digits = 1;
		int maxBlock = qMax(1, _editor->blockCount());
		while (maxBlock >= 10)
		{
			maxBlock /= 10;
			++digits;
		}
		int space = _editor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + 20;
		return space;
	}

protected:
	void paintEvent(QPaintEvent *event) override
	{
		QPainter painter(this);
		ElaThemeType::ThemeMode themeMode = _d->_themeMode;
		painter.fillRect(event->rect(), ElaThemeColor(themeMode, BasicBaseDeep));

		QTextBlock block = _editor->getFirstVisibleBlock();
		int blockNumber = block.blockNumber();
		int top = qRound(_editor->getBlockBoundingGeometry(block).translated(_editor->getContentOffset()).top());
		int bottom = top + qRound(_editor->getBlockBoundingRect(block).height());

		int currentBlockNumber = _editor->textCursor().blockNumber();

		while (block.isValid() && top <= event->rect().bottom())
		{
			if (block.isVisible() && bottom >= event->rect().top())
			{
				QString number = QString::number(blockNumber + 1);
				if (blockNumber == currentBlockNumber)
				{
					painter.setPen(ElaThemeColor(themeMode, PrimaryNormal));
				}
				else
				{
					painter.setPen(ElaThemeColor(themeMode, BasicDetailsText));
				}
				painter.setFont(_editor->font());
				painter.drawText(0, top, width() - 8, _editor->fontMetrics().height(),
				                 Qt::AlignRight | Qt::AlignVCenter, number);
			}
			block = block.next();
			top = bottom;
			bottom = top + qRound(_editor->getBlockBoundingRect(block).height());
			++blockNumber;
		}
	}

private:
	ElaCodePlainTextEdit *_editor;
	ElaCodeEditorPrivate *_d;
	friend class ElaCodeEditorPrivate;
};

#endif // ELACODEEDITORLINEAREA_H