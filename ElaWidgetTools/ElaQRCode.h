#ifndef ELAQRCODE_H
#define ELAQRCODE_H

#include <QWidget>

#include "ElaProperty.h"

class ElaQRCodePrivate;
class ELA_EXPORT ElaQRCode : public QWidget
{
	Q_OBJECT
	Q_Q_CREATE(ElaQRCode)
	Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
	Q_PROPERTY_CREATE_Q_H(int, QuietZone)
	Q_PROPERTY_CREATE_Q_H(QColor, ForegroundColor)
	Q_PROPERTY_CREATE_Q_H(QColor, BackgroundColor)
public:
	enum ErrorCorrectionLevel
	{
		Low = 0,
		Medium,
		Quartile,
		High
	};
	Q_ENUM(ErrorCorrectionLevel)

	explicit ElaQRCode(QWidget* parent = nullptr);
	explicit ElaQRCode(const QString& text, QWidget* parent = nullptr);
	~ElaQRCode() override;

	void setText(const QString& text);
	QString getText() const;

	void setErrorCorrectionLevel(ErrorCorrectionLevel level);
	ErrorCorrectionLevel getErrorCorrectionLevel() const;

	QPixmap toPixmap(int size = 256) const;

protected:
	void paintEvent(QPaintEvent* event) override;
};

#endif // ELAQRCODE_H
