#include "ElaEmojiPicker.h"

#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QScreen>

#include "ElaLineEdit.h"
#include "ElaScrollArea.h"
#include "ElaTheme.h"
#include "private/ElaEmojiPickerPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaEmojiPicker, int, EmojiSize)
Q_PROPERTY_CREATE_Q_CPP(ElaEmojiPicker, int, Columns)

ElaEmojiPicker::ElaEmojiPicker(QWidget *parent) : QWidget{nullptr}, d_ptr(new ElaEmojiPickerPrivate())
{
	Q_D(ElaEmojiPicker);
	d->q_ptr = this;
	d->_pEmojiSize = 36;
	d->_pColumns = 8;
	d->_themeMode = eTheme->getThemeMode();

	setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(340, 420);

	d->initEmojis();

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
	mainLayout->setContentsMargins(2, 2, 2, 2);
#else
	mainLayout->setContentsMargins(8, 8, 8, 8);
#endif
	mainLayout->setSpacing(4);

	d->_searchEdit = new ElaLineEdit(this);
	d->_searchEdit->setPlaceholderText(QString::fromUtf8("搜索表情..."));
	d->_searchEdit->setFixedHeight(32);
	connect(d->_searchEdit, &ElaLineEdit::textChanged, this, [d](const QString &text)
	{
		d->filterEmojis(text);
	});

	QHBoxLayout *tabLayout = new QHBoxLayout();
	tabLayout->setSpacing(2);
	for (int i = 0; i < d->_categories.size(); i++)
	{
		QPushButton *tabBtn = new QPushButton(d->_categories[i].icon, this);
		tabBtn->setFixedSize(32, 32);
		tabBtn->setFlat(true);
		tabBtn->setStyleSheet("QPushButton{font-size:18px;border:none;border-radius:4px;}"
			"QPushButton:hover{background-color:rgba(128,128,128,40);}");
		connect(tabBtn, &QPushButton::clicked, this, [d, i]()
		{
			d->_currentCategory = i;
			d->_searchEdit->clear();
			d->buildGrid(i);
		});
		tabLayout->addWidget(tabBtn);
	}
	tabLayout->addStretch();

	ElaScrollArea *scrollArea = new ElaScrollArea(this);
	scrollArea->setWidgetResizable(true);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setStyleSheet("QScrollArea{background:transparent;border:none;}");

	d->_gridContainer = new QWidget();
	d->_gridContainer->setStyleSheet("background:transparent;");
	d->_gridLayout = new QGridLayout(d->_gridContainer);
	d->_gridLayout->setSpacing(2);
	d->_gridLayout->setContentsMargins(0, 0, 0, 0);
	scrollArea->setWidget(d->_gridContainer);

	mainLayout->addWidget(d->_searchEdit);
	mainLayout->addLayout(tabLayout);
	mainLayout->addWidget(scrollArea);

	d->buildGrid(d->_recentEmojis.isEmpty() ? 1 : 0);

	connect(eTheme, &ElaTheme::themeModeChanged, this, [this, d](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		d->buildGrid(d->_currentCategory);
		update();
	});
}

ElaEmojiPicker::~ElaEmojiPicker()
{
}

void ElaEmojiPicker::popup(QWidget *anchor)
{
	if (!anchor)
	{
		return;
	}
	QPoint pos = anchor->mapToGlobal(QPoint(0, -height() - 5));
	if (pos.y() < 0)
	{
		pos = anchor->mapToGlobal(QPoint(0, anchor->height() + 5));
	}
	popup(pos);
}

void ElaEmojiPicker::popup(const QPoint &pos)
{
	move(pos);
	show();
	raise();
	activateWindow();
}

void ElaEmojiPicker::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaEmojiPicker);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
#if QT_VERSION >= QT_VERSION_CHECK(6, 11, 0)
	QRect fg = rect();
#else
	eTheme->drawEffectShadow(&painter, rect(), 6, 8);
	QRect fg = rect().adjusted(6, 6, -6, -6);
#endif
	painter.setPen(ElaThemeColor(d->_themeMode, PopupBorder));
	painter.setBrush(ElaThemeColor(d->_themeMode, PopupBase));
	painter.drawRoundedRect(fg, 8, 8);
}

ElaEmojiPickerPrivate::ElaEmojiPickerPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaEmojiPickerPrivate::~ElaEmojiPickerPrivate()
{
}

void ElaEmojiPickerPrivate::initEmojis()
{
	_categories =
	{
		{QString::fromUtf8("最近"), QString::fromUtf8("🕐"), {}},
		{
			QString::fromUtf8("表情"), QString::fromUtf8("😀"),
			{
				QString::fromUtf8("😀"), QString::fromUtf8("😃"), QString::fromUtf8("😄"), QString::fromUtf8("😁"),
				QString::fromUtf8("😆"), QString::fromUtf8("😅"), QString::fromUtf8("🤣"), QString::fromUtf8("😂"),
				QString::fromUtf8("🙂"), QString::fromUtf8("🙃"), QString::fromUtf8("😉"), QString::fromUtf8("😊"),
				QString::fromUtf8("😇"), QString::fromUtf8("🥰"), QString::fromUtf8("😍"), QString::fromUtf8("🤩"),
				QString::fromUtf8("😘"), QString::fromUtf8("😗"), QString::fromUtf8("😚"), QString::fromUtf8("😙"),
				QString::fromUtf8("🥲"), QString::fromUtf8("😋"), QString::fromUtf8("😛"), QString::fromUtf8("😜"),
				QString::fromUtf8("🤪"), QString::fromUtf8("😝"), QString::fromUtf8("🤑"), QString::fromUtf8("🤗"),
				QString::fromUtf8("🤭"), QString::fromUtf8("🤫"), QString::fromUtf8("🤔"), QString::fromUtf8("🫡"),
				QString::fromUtf8("🤐"), QString::fromUtf8("🤨"), QString::fromUtf8("😐"), QString::fromUtf8("😑"),
				QString::fromUtf8("😶"), QString::fromUtf8("🫥"), QString::fromUtf8("😏"), QString::fromUtf8("😒"),
				QString::fromUtf8("🙄"), QString::fromUtf8("😬"), QString::fromUtf8("🤥"), QString::fromUtf8("😌"),
				QString::fromUtf8("😔"), QString::fromUtf8("😪"), QString::fromUtf8("🤤"), QString::fromUtf8("😴"),
				QString::fromUtf8("😷"), QString::fromUtf8("🤒"), QString::fromUtf8("🤕"), QString::fromUtf8("🤢"),
				QString::fromUtf8("🤮"), QString::fromUtf8("🤧"), QString::fromUtf8("🥵"), QString::fromUtf8("🥶"),
				QString::fromUtf8("🥴"), QString::fromUtf8("😵"), QString::fromUtf8("🤯"), QString::fromUtf8("🤠"),
				QString::fromUtf8("🥳"), QString::fromUtf8("🥸"), QString::fromUtf8("😎"), QString::fromUtf8("🤓"),
				QString::fromUtf8("🧐"), QString::fromUtf8("😕"), QString::fromUtf8("🫤"), QString::fromUtf8("😟"),
				QString::fromUtf8("🙁"), QString::fromUtf8("😮"), QString::fromUtf8("😯"), QString::fromUtf8("😲"),
				QString::fromUtf8("😳"), QString::fromUtf8("🥺"), QString::fromUtf8("🥹"), QString::fromUtf8("😦"),
				QString::fromUtf8("😧"), QString::fromUtf8("😨"), QString::fromUtf8("😰"), QString::fromUtf8("😥"),
				QString::fromUtf8("😢"), QString::fromUtf8("😭"), QString::fromUtf8("😱"), QString::fromUtf8("😖"),
				QString::fromUtf8("😣"), QString::fromUtf8("😞"), QString::fromUtf8("😓"), QString::fromUtf8("😩"),
				QString::fromUtf8("😫"), QString::fromUtf8("🥱"), QString::fromUtf8("😤"), QString::fromUtf8("😡"),
				QString::fromUtf8("😠"), QString::fromUtf8("🤬"), QString::fromUtf8("😈"), QString::fromUtf8("👿"),
				QString::fromUtf8("💀"), QString::fromUtf8("☠️"), QString::fromUtf8("💩"), QString::fromUtf8("🤡"),
				QString::fromUtf8("👹"), QString::fromUtf8("👺"), QString::fromUtf8("👻"), QString::fromUtf8("👽"),
				QString::fromUtf8("👾"), QString::fromUtf8("🤖"), QString::fromUtf8("🎃"), QString::fromUtf8("😺"),
				QString::fromUtf8("😸"), QString::fromUtf8("😹"), QString::fromUtf8("😻"), QString::fromUtf8("😼"),
			}
		},
		{
			QString::fromUtf8("手势"), QString::fromUtf8("👋"),
			{
				QString::fromUtf8("👋"), QString::fromUtf8("🤚"), QString::fromUtf8("🖐️"), QString::fromUtf8("✋"),
				QString::fromUtf8("🖖"), QString::fromUtf8("🫱"), QString::fromUtf8("🫲"), QString::fromUtf8("🫳"),
				QString::fromUtf8("🫴"), QString::fromUtf8("👌"), QString::fromUtf8("🤌"), QString::fromUtf8("🤏"),
				QString::fromUtf8("✌️"), QString::fromUtf8("🤞"), QString::fromUtf8("🫰"), QString::fromUtf8("🤟"),
				QString::fromUtf8("🤘"), QString::fromUtf8("🤙"), QString::fromUtf8("👈"), QString::fromUtf8("👉"),
				QString::fromUtf8("👆"), QString::fromUtf8("🖕"), QString::fromUtf8("👇"), QString::fromUtf8("☝️"),
				QString::fromUtf8("🫵"), QString::fromUtf8("👍"), QString::fromUtf8("👎"), QString::fromUtf8("✊"),
				QString::fromUtf8("👊"), QString::fromUtf8("🤛"), QString::fromUtf8("🤜"), QString::fromUtf8("👏"),
				QString::fromUtf8("🙌"), QString::fromUtf8("🫶"), QString::fromUtf8("👐"), QString::fromUtf8("🤲"),
				QString::fromUtf8("🤝"), QString::fromUtf8("🙏"), QString::fromUtf8("✍️"), QString::fromUtf8("💅"),
			}
		},
		{
			QString::fromUtf8("爱心"), QString::fromUtf8("❤️"),
			{
				QString::fromUtf8("❤️"), QString::fromUtf8("🧡"), QString::fromUtf8("💛"), QString::fromUtf8("💚"),
				QString::fromUtf8("💙"), QString::fromUtf8("💜"), QString::fromUtf8("🖤"), QString::fromUtf8("🤍"),
				QString::fromUtf8("🤎"), QString::fromUtf8("💔"), QString::fromUtf8("❤️‍🔥"), QString::fromUtf8("❤️‍🩹"),
				QString::fromUtf8("💕"), QString::fromUtf8("💞"), QString::fromUtf8("💓"), QString::fromUtf8("💗"),
				QString::fromUtf8("💖"), QString::fromUtf8("💘"), QString::fromUtf8("💝"), QString::fromUtf8("💟"),
				QString::fromUtf8("💌"), QString::fromUtf8("💋"), QString::fromUtf8("💯"), QString::fromUtf8("💢"),
				QString::fromUtf8("💥"), QString::fromUtf8("💫"), QString::fromUtf8("💦"), QString::fromUtf8("💨"),
				QString::fromUtf8("🕳️"), QString::fromUtf8("💬"), QString::fromUtf8("💭"), QString::fromUtf8("🗯️"),
			}
		},
		{
			QString::fromUtf8("动物"), QString::fromUtf8("🐶"),
			{
				QString::fromUtf8("🐶"), QString::fromUtf8("🐱"), QString::fromUtf8("🐭"), QString::fromUtf8("🐹"),
				QString::fromUtf8("🐰"), QString::fromUtf8("🦊"), QString::fromUtf8("🐻"), QString::fromUtf8("🐼"),
				QString::fromUtf8("🐻‍❄️"), QString::fromUtf8("🐨"), QString::fromUtf8("🐯"), QString::fromUtf8("🦁"),
				QString::fromUtf8("🐮"), QString::fromUtf8("🐷"), QString::fromUtf8("🐸"), QString::fromUtf8("🐵"),
				QString::fromUtf8("🙈"), QString::fromUtf8("🙉"), QString::fromUtf8("🙊"), QString::fromUtf8("🐔"),
				QString::fromUtf8("🐧"), QString::fromUtf8("🐦"), QString::fromUtf8("🐤"), QString::fromUtf8("🦄"),
				QString::fromUtf8("🐝"), QString::fromUtf8("🪱"), QString::fromUtf8("🐛"), QString::fromUtf8("🦋"),
				QString::fromUtf8("🐌"), QString::fromUtf8("🐞"), QString::fromUtf8("🐜"), QString::fromUtf8("🪲"),
				QString::fromUtf8("🐢"), QString::fromUtf8("🐍"), QString::fromUtf8("🦎"), QString::fromUtf8("🦂"),
				QString::fromUtf8("🐙"), QString::fromUtf8("🦑"), QString::fromUtf8("🦐"), QString::fromUtf8("🐠"),
				QString::fromUtf8("🐟"), QString::fromUtf8("🐡"), QString::fromUtf8("🐬"), QString::fromUtf8("🐳"),
				QString::fromUtf8("🐋"), QString::fromUtf8("🦈"), QString::fromUtf8("🦭"), QString::fromUtf8("🐊"),
			}
		},
		{
			QString::fromUtf8("食物"), QString::fromUtf8("🍔"),
			{
				QString::fromUtf8("🍎"), QString::fromUtf8("🍐"), QString::fromUtf8("🍊"), QString::fromUtf8("🍋"),
				QString::fromUtf8("🍌"), QString::fromUtf8("🍉"), QString::fromUtf8("🍇"), QString::fromUtf8("🍓"),
				QString::fromUtf8("🫐"), QString::fromUtf8("🍈"), QString::fromUtf8("🍒"), QString::fromUtf8("🍑"),
				QString::fromUtf8("🥭"), QString::fromUtf8("🍍"), QString::fromUtf8("🥥"), QString::fromUtf8("🥝"),
				QString::fromUtf8("🍅"), QString::fromUtf8("🥑"), QString::fromUtf8("🍆"), QString::fromUtf8("🌶️"),
				QString::fromUtf8("🥦"), QString::fromUtf8("🥬"), QString::fromUtf8("🥒"), QString::fromUtf8("🌽"),
				QString::fromUtf8("🍔"), QString::fromUtf8("🍟"), QString::fromUtf8("🍕"), QString::fromUtf8("🌭"),
				QString::fromUtf8("🥪"), QString::fromUtf8("🌮"), QString::fromUtf8("🌯"), QString::fromUtf8("🍜"),
				QString::fromUtf8("🍝"), QString::fromUtf8("🍣"), QString::fromUtf8("🍱"), QString::fromUtf8("🍛"),
				QString::fromUtf8("🍙"), QString::fromUtf8("🍚"), QString::fromUtf8("🍘"), QString::fromUtf8("🥟"),
				QString::fromUtf8("🍰"), QString::fromUtf8("🎂"), QString::fromUtf8("🧁"), QString::fromUtf8("🍩"),
				QString::fromUtf8("🍪"), QString::fromUtf8("🍫"), QString::fromUtf8("🍬"), QString::fromUtf8("🍭"),
				QString::fromUtf8("☕"), QString::fromUtf8("🍵"), QString::fromUtf8("🧃"), QString::fromUtf8("🥤"),
				QString::fromUtf8("🍺"), QString::fromUtf8("🍻"), QString::fromUtf8("🥂"), QString::fromUtf8("🍷"),
			}
		},
		{
			QString::fromUtf8("物品"), QString::fromUtf8("⚽"),
			{
				QString::fromUtf8("⚽"), QString::fromUtf8("🏀"), QString::fromUtf8("🏈"), QString::fromUtf8("⚾"),
				QString::fromUtf8("🎾"), QString::fromUtf8("🏐"), QString::fromUtf8("🎱"), QString::fromUtf8("🏓"),
				QString::fromUtf8("🎮"), QString::fromUtf8("🕹️"), QString::fromUtf8("🎲"), QString::fromUtf8("🧩"),
				QString::fromUtf8("🎭"), QString::fromUtf8("🎨"), QString::fromUtf8("🎬"), QString::fromUtf8("🎤"),
				QString::fromUtf8("🎧"), QString::fromUtf8("🎵"), QString::fromUtf8("🎶"), QString::fromUtf8("🎸"),
				QString::fromUtf8("🎹"), QString::fromUtf8("🎺"), QString::fromUtf8("🎻"), QString::fromUtf8("🥁"),
				QString::fromUtf8("📱"), QString::fromUtf8("💻"), QString::fromUtf8("⌨️"), QString::fromUtf8("🖥️"),
				QString::fromUtf8("🖨️"), QString::fromUtf8("📷"), QString::fromUtf8("📹"), QString::fromUtf8("🔍"),
				QString::fromUtf8("💡"), QString::fromUtf8("🔦"), QString::fromUtf8("📖"), QString::fromUtf8("📚"),
				QString::fromUtf8("✏️"), QString::fromUtf8("📝"), QString::fromUtf8("📌"), QString::fromUtf8("📎"),
				QString::fromUtf8("🔑"), QString::fromUtf8("🔒"), QString::fromUtf8("🔓"), QString::fromUtf8("🛠️"),
				QString::fromUtf8("⚙️"), QString::fromUtf8("💰"), QString::fromUtf8("💎"), QString::fromUtf8("🏆"),
			}
		},
		{
			QString::fromUtf8("符号"), QString::fromUtf8("✅"),
			{
				QString::fromUtf8("✅"), QString::fromUtf8("❌"), QString::fromUtf8("❓"), QString::fromUtf8("❗"),
				QString::fromUtf8("‼️"), QString::fromUtf8("⁉️"), QString::fromUtf8("⭕"), QString::fromUtf8("🚫"),
				QString::fromUtf8("🔴"), QString::fromUtf8("🟠"), QString::fromUtf8("🟡"), QString::fromUtf8("🟢"),
				QString::fromUtf8("🔵"), QString::fromUtf8("🟣"), QString::fromUtf8("⚫"), QString::fromUtf8("⚪"),
				QString::fromUtf8("🟤"), QString::fromUtf8("🔺"), QString::fromUtf8("🔻"), QString::fromUtf8("🔸"),
				QString::fromUtf8("🔹"), QString::fromUtf8("🔶"), QString::fromUtf8("🔷"), QString::fromUtf8("▪️"),
				QString::fromUtf8("▫️"), QString::fromUtf8("◾"), QString::fromUtf8("◽"), QString::fromUtf8("🔘"),
				QString::fromUtf8("⬆️"), QString::fromUtf8("⬇️"), QString::fromUtf8("⬅️"), QString::fromUtf8("➡️"),
				QString::fromUtf8("↩️"), QString::fromUtf8("↪️"), QString::fromUtf8("🔄"), QString::fromUtf8("🔃"),
				QString::fromUtf8("⭐"), QString::fromUtf8("🌟"), QString::fromUtf8("✨"), QString::fromUtf8("🔥"),
				QString::fromUtf8("🎉"), QString::fromUtf8("🎊"), QString::fromUtf8("🏁"), QString::fromUtf8("🚩"),
				QString::fromUtf8("🏳️"), QString::fromUtf8("🏴"), QString::fromUtf8("🔔"), QString::fromUtf8("🔕"),
			}
		},
	};
}

void ElaEmojiPickerPrivate::buildGrid(int categoryIndex)
{
	Q_Q(ElaEmojiPicker);
	QLayoutItem *item;
	while ((item = _gridLayout->takeAt(0)) != nullptr)
	{
		delete item->widget();
		delete item;
	}

	const QStringList &emojis = (categoryIndex == 0) ? _recentEmojis : _categories[categoryIndex].emojis;
	if (emojis.isEmpty() && categoryIndex == 0)
	{
		QLabel *hint = new QLabel(QString::fromUtf8("暂无最近使用"), _gridContainer);
		hint->setAlignment(Qt::AlignCenter);
		hint->setStyleSheet("color:gray;padding:20px;");
		_gridLayout->addWidget(hint, 0, 0, 1, _pColumns);
		return;
	}

	int row = 0, col = 0;
	for (const QString &emoji: emojis)
	{
		QPushButton *btn = new QPushButton(emoji, _gridContainer);
		btn->setFixedSize(_pEmojiSize, _pEmojiSize);
		btn->setFlat(true);
		btn->setStyleSheet(QString("QPushButton{font-size:%1px;border:none;border-radius:4px;}"
			"QPushButton:hover{background-color:rgba(128,128,128,40);}").arg(_pEmojiSize - 10));
		QObject::connect(btn, &QPushButton::clicked, q, [this, q, emoji]()
		{
			_recentEmojis.removeAll(emoji);
			_recentEmojis.prepend(emoji);
			if (_recentEmojis.size() > 32)
			{
				_recentEmojis.removeLast();
			}
			Q_EMIT q->emojiSelected(emoji);
			q->hide();
		});
		_gridLayout->addWidget(btn, row, col);
		col++;
		if (col >= _pColumns)
		{
			col = 0;
			row++;
		}
	}
}

void ElaEmojiPickerPrivate::filterEmojis(const QString &text)
{
	Q_Q(ElaEmojiPicker);
	if (text.isEmpty())
	{
		buildGrid(_currentCategory);
		return;
	}

	QLayoutItem *item;
	while ((item = _gridLayout->takeAt(0)) != nullptr)
	{
		delete item->widget();
		delete item;
	}

	QStringList all;
	for (int i = 1; i < _categories.size(); i++)
	{
		all.append(_categories[i].emojis);
	}

	int row = 0, col = 0;
	for (const QString &emoji: all)
	{
		QPushButton *btn = new QPushButton(emoji, _gridContainer);
		btn->setFixedSize(_pEmojiSize, _pEmojiSize);
		btn->setFlat(true);
		btn->setStyleSheet(QString("QPushButton{font-size:%1px;border:none;border-radius:4px;}"
			"QPushButton:hover{background-color:rgba(128,128,128,40);}").arg(_pEmojiSize - 10));
		QObject::connect(btn, &QPushButton::clicked, q, [this, q, emoji]()
		{
			_recentEmojis.removeAll(emoji);
			_recentEmojis.prepend(emoji);
			if (_recentEmojis.size() > 32)
			{
				_recentEmojis.removeLast();
			}
			Q_EMIT q->emojiSelected(emoji);
			q->hide();
		});
		_gridLayout->addWidget(btn, row, col);
		col++;
		if (col >= _pColumns)
		{
			col = 0;
			row++;
		}
	}
}