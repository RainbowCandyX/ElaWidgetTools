#ifndef ELAFLOATBUTTONPRIVATE_H
#define ELAFLOATBUTTONPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaFloatButton;
class ElaMenu;
class ElaFloatButtonPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaFloatButton)
	Q_PROPERTY_CREATE_D(int, ButtonSize)
	Q_PROPERTY_CREATE_D(int, Margin)
	Q_PROPERTY_CREATE(int, HoverAlpha)
public:
	explicit ElaFloatButtonPrivate(QObject* parent = nullptr);
	~ElaFloatButtonPrivate() override;

	void updatePosition();

private:
	ElaIconType::IconName _icon{ElaIconType::Plus};
	int _position{0};
	ElaMenu* _menu{nullptr};
	bool _isPressed{false};
	bool _isAlphaAnimationFinished{true};
	ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAFLOATBUTTONPRIVATE_H
