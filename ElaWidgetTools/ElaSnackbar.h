#ifndef ELASTACKBAR_H
#define ELASTACKBAR_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaSnackbarPrivate;
class ELA_EXPORT ElaSnackbar : public QWidget
{
	Q_OBJECT
	Q_Q_CREATE(ElaSnackbar)
	Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
	Q_PROPERTY_CREATE_Q_H(int, DisplayMsec)
public:
	enum SnackbarType
	{
		Success = 0,
		Info,
		Warning,
		Error
	};
	Q_ENUM(SnackbarType)

	void dismiss();

	static void setMaxCount(int count);
	static int getMaxCount();

	static ElaSnackbar* success(const QString& text, const QString& actionText = "", int displayMsec = 4000, QWidget* parent = nullptr);
	static ElaSnackbar* info(const QString& text, const QString& actionText = "", int displayMsec = 4000, QWidget* parent = nullptr);
	static ElaSnackbar* warning(const QString& text, const QString& actionText = "", int displayMsec = 4000, QWidget* parent = nullptr);
	static ElaSnackbar* error(const QString& text, const QString& actionText = "", int displayMsec = 4000, QWidget* parent = nullptr);

Q_SIGNALS:
	Q_SIGNAL void actionClicked();
	Q_SIGNAL void closed();

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	explicit ElaSnackbar(SnackbarType type, const QString& text, const QString& actionText, int displayMsec, QWidget* parent = nullptr);
	~ElaSnackbar() override;
	static void _reflowAll();
};

#endif // ELASTACKBAR_H
