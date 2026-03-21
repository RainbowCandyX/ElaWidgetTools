#ifndef ELAEMOJIPICKERPRIVATE_H
#define ELAEMOJIPICKERPRIVATE_H

#include <QObject>
#include <QStringList>
#include <QVector>

#include "ElaDef.h"

class ElaEmojiPicker;
class QGridLayout;
class ElaLineEdit;
class ElaEmojiPickerPrivate : public QObject
{
	Q_OBJECT
	Q_D_CREATE(ElaEmojiPicker)
	Q_PROPERTY_CREATE_D(int, EmojiSize)
	Q_PROPERTY_CREATE_D(int, Columns)
public:
	explicit ElaEmojiPickerPrivate(QObject* parent = nullptr);
	~ElaEmojiPickerPrivate() override;

	struct EmojiCategory
	{
		QString name;
		QString icon;
		QStringList emojis;
	};

	void initEmojis();
	void buildGrid(int categoryIndex);
	void filterEmojis(const QString& text);

	QVector<EmojiCategory> _categories;
	QStringList _recentEmojis;
	int _currentCategory{0};
	ElaThemeType::ThemeMode _themeMode;

	QGridLayout* _gridLayout{nullptr};
	ElaLineEdit* _searchEdit{nullptr};
	QWidget* _gridContainer{nullptr};
};

#endif // ELAEMOJIPICKERPRIVATE_H
