#ifndef ELAQRCODEPRIVATE_H
#define ELAQRCODEPRIVATE_H

#include <QObject>
#include <QVector>

#include "ElaDef.h"

class ElaQRCode;
class ElaQRCodePrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaQRCode)
	Q_PROPERTY_CREATE_D(int, BorderRadius)
	Q_PROPERTY_CREATE_D(int, QuietZone)
	Q_PROPERTY_CREATE_D(QColor, ForegroundColor)
	Q_PROPERTY_CREATE_D(QColor, BackgroundColor)
public:
	explicit ElaQRCodePrivate(QObject* parent = nullptr);
	~ElaQRCodePrivate() override;

	void encode(const QString& text);

	QString _text;
	QVector<QVector<bool>> _modules;
	int _moduleCount{0};
	int _ecLevel{1}; // 0=L, 1=M, 2=Q, 3=H
	ElaThemeType::ThemeMode _themeMode;
};

#endif // ELAQRCODEPRIVATE_H
