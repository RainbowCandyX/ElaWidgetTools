#ifndef ELAEMOJIPICKER_H
#define ELAEMOJIPICKER_H

#include <QWidget>

#include "ElaProperty.h"

class ElaEmojiPickerPrivate;
class ELA_EXPORT ElaEmojiPicker : public QWidget
{
	Q_OBJECT
	Q_Q_CREATE(ElaEmojiPicker)
	Q_PROPERTY_CREATE_Q_H(int, EmojiSize)
	Q_PROPERTY_CREATE_Q_H(int, Columns)
public:
	explicit ElaEmojiPicker(QWidget* parent = nullptr);
	~ElaEmojiPicker() override;

	void popup(QWidget* anchor);
	void popup(const QPoint& pos);

Q_SIGNALS:
	Q_SIGNAL void emojiSelected(const QString& emoji);

protected:
	void paintEvent(QPaintEvent* event) override;
};

#endif // ELAEMOJIPICKER_H
