#ifndef ELASPLITTERHANDLE_H
#define ELASPLITTERHANDLE_H

#include <QSplitterHandle>

#include "ElaDef.h"

class ElaSplitterHandle : public QSplitterHandle
{
	Q_OBJECT
public:
	explicit ElaSplitterHandle(Qt::Orientation orientation, QSplitter* parent = nullptr);
	~ElaSplitterHandle() override;

	void setGripLength(int length);
	int getGripLength() const;

protected:
	void paintEvent(QPaintEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void ElaMessageDialogButton::enterEvent(QEnterEvent* event)
#else
	void enterEvent(QEvent* event)override;
#endif
	void leaveEvent(QEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	ElaThemeType::ThemeMode _themeMode;
	bool _isHover{false};
	bool _isPressed{false};
	int _gripLength{36};
};

#endif // ELASPLITTERHANDLE_H
