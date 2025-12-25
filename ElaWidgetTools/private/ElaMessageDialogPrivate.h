#ifndef ELAMESSAGEDIALOGPRIVATE_H
#define ELAMESSAGEDIALOGPRIVATE_H

#include <QObject>
#include <QWidget>

#include "ElaDef.h"

class ElaMessageDialog;

class ElaMessageDialogButton : public QWidget
{
	Q_OBJECT

public:
	enum ButtonType
	{
		Confirm,
		Cancel
	};

	explicit ElaMessageDialogButton(ButtonType type, QWidget *parent = nullptr);
	~ElaMessageDialogButton() override;

Q_SIGNALS:
	void clicked();

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void enterEvent(QEnterEvent *event) override;
	void leaveEvent(QEvent *event) override;

private:
	ButtonType _buttonType;
	bool _isPressed{false};
	bool _isHovered{false};
	ElaThemeType::ThemeMode _themeMode;
};

class ElaMessageDialogPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaMessageDialog)

	Q_PROPERTY_CREATE_D(int, BorderRadius)
	Q_PROPERTY_CREATE_D(QString, Title)
	Q_PROPERTY_CREATE_D(QString, Content)
	Q_PROPERTY_CREATE_D(int, TitlePixelSize)
	Q_PROPERTY_CREATE_D(int, ContentPixelSize)

public:
	explicit ElaMessageDialogPrivate(QObject *parent = nullptr);

	~ElaMessageDialogPrivate();

	ElaThemeType::ThemeMode _themeMode;
	ElaMessageDialogButton *_confirmButton{nullptr};
	ElaMessageDialogButton *_cancelButton{nullptr};
};

#endif // ELAMESSAGEDIALOGPRIVATE_H