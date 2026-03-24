#ifndef ELAFLOATBUTTON_H
#define ELAFLOATBUTTON_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaFloatButtonPrivate;
class ElaMenu;
class ELA_EXPORT ElaFloatButton : public QWidget
{
	Q_OBJECT
	Q_Q_CREATE(ElaFloatButton)
	Q_PROPERTY_CREATE_Q_H(int, ButtonSize)
	Q_PROPERTY_CREATE_Q_H(int, Margin)
public:
	enum Position
	{
		BottomRight = 0,
		BottomLeft,
		TopRight,
		TopLeft
	};
	Q_ENUM(Position)

	explicit ElaFloatButton(QWidget* parent = nullptr);
	explicit ElaFloatButton(ElaIconType::IconName icon, QWidget* parent = nullptr);
	explicit ElaFloatButton(ElaIconType::IconName icon, Position position, QWidget* parent = nullptr);
	~ElaFloatButton() override;

	void setIcon(ElaIconType::IconName icon);
	ElaIconType::IconName getIcon() const;

	void setPosition(Position position);
	Position getPosition() const;

	void setMenu(ElaMenu* menu);
	ElaMenu* getMenu() const;

Q_SIGNALS:
	Q_SIGNAL void clicked();

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;
	bool event(QEvent* event) override;
};

#endif // ELAFLOATBUTTON_H
