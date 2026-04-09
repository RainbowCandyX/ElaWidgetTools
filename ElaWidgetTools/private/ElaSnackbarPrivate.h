#ifndef ELASTACKBARPRIVATE_H
#define ELASTACKBARPRIVATE_H

#include <QObject>

#include "ElaDef.h"
#include "ElaSnackbar.h"

class ElaSnackbarPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaSnackbar)
	Q_PROPERTY_CREATE_D(int, BorderRadius)
	Q_PROPERTY_CREATE_D(int, DisplayMsec)
public:
	explicit ElaSnackbarPrivate(QObject* parent = nullptr);
	~ElaSnackbarPrivate() override;

private:
	ElaThemeType::ThemeMode _themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
	int _shadowBorderWidth{0};
#else
	int _shadowBorderWidth{6};
#endif
	ElaSnackbar::SnackbarType _snackbarType{ElaSnackbar::Info};
	QString _text;
	QString _actionText;
	QRect _actionRect;
	bool _isActionHover{false};
	bool _isActionPressed{false};
	bool _isClosing{false};
	std::function<void()> _doClose;
	int _baseBottomY{0};
	int _centerX{0};
};

#endif // ELASTACKBARPRIVATE_H
