#include "ElaQRCode.h"

#include <QPainter>
#include <QPainterPath>

#include "ElaTheme.h"
#include "private/ElaQRCodePrivate.h"

Q_PROPERTY_CREATE_Q_CPP(ElaQRCode, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(ElaQRCode, int, QuietZone)
Q_PROPERTY_CREATE_Q_CPP(ElaQRCode, QColor, ForegroundColor)
Q_PROPERTY_CREATE_Q_CPP(ElaQRCode, QColor, BackgroundColor)

ElaQRCode::ElaQRCode(QWidget *parent) : QWidget{parent}, d_ptr(new ElaQRCodePrivate())
{
	Q_D(ElaQRCode);
	d->q_ptr = this;
	d->_pBorderRadius = 0;
	d->_pQuietZone = 2;
	d->_pForegroundColor = Qt::black;
	d->_pBackgroundColor = Qt::white;
	d->_themeMode = eTheme->getThemeMode();
	setFixedSize(200, 200);
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode)
	{
		d->_themeMode = themeMode;
		update();
	});
}

ElaQRCode::ElaQRCode(const QString &text, QWidget *parent)
	: ElaQRCode(parent)
{
	setText(text);
}

ElaQRCode::~ElaQRCode()
{
}

void ElaQRCode::setText(const QString &text)
{
	Q_D(ElaQRCode);
	d->_text = text;
	d->encode(text);
	update();
}

QString ElaQRCode::getText() const
{
	return d_ptr->_text;
}

void ElaQRCode::setErrorCorrectionLevel(ErrorCorrectionLevel level)
{
	Q_D(ElaQRCode);
	d->_ecLevel = (int) level;
	if (!d->_text.isEmpty())
	{
		d->encode(d->_text);
		update();
	}
}

ElaQRCode::ErrorCorrectionLevel ElaQRCode::getErrorCorrectionLevel() const
{
	return (ErrorCorrectionLevel) d_ptr->_ecLevel;
}

QPixmap ElaQRCode::toPixmap(int size) const
{
	Q_D(const ElaQRCode);
	QPixmap pixmap(size, size);
	pixmap.fill(d->_pBackgroundColor);
	if (d->_moduleCount == 0)
	{
		return pixmap;
	}
	QPainter painter(&pixmap);
	int totalModules = d->_moduleCount + d->_pQuietZone * 2;
	qreal moduleSize = (qreal) size / totalModules;
	painter.setPen(Qt::NoPen);
	painter.setBrush(d->_pForegroundColor);
	for (int row = 0; row < d->_moduleCount; row++)
	{
		for (int col = 0; col < d->_moduleCount; col++)
		{
			if (d->_modules[row][col])
			{
				qreal x = (col + d->_pQuietZone) * moduleSize;
				qreal y = (row + d->_pQuietZone) * moduleSize;
				painter.drawRect(QRectF(x, y, moduleSize + 0.5, moduleSize + 0.5));
			}
		}
	}
	return pixmap;
}

void ElaQRCode::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(ElaQRCode);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	painter.setPen(Qt::NoPen);
	painter.setBrush(d->_pBackgroundColor);
	painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);

	if (d->_moduleCount == 0)
	{
		painter.setPen(ElaThemeColor(d->_themeMode, BasicText));
		painter.drawText(rect(), Qt::AlignCenter, "无数据");
		return;
	}

	int totalModules = d->_moduleCount + d->_pQuietZone * 2;
	int drawSize = qMin(width(), height());
	qreal moduleSize = (qreal) drawSize / totalModules;
	qreal offsetX = (width() - drawSize) / 2.0;
	qreal offsetY = (height() - drawSize) / 2.0;

	painter.setPen(Qt::NoPen);
	painter.setBrush(d->_pForegroundColor);
	for (int row = 0; row < d->_moduleCount; row++)
	{
		for (int col = 0; col < d->_moduleCount; col++)
		{
			if (d->_modules[row][col])
			{
				qreal x = offsetX + (col + d->_pQuietZone) * moduleSize;
				qreal y = offsetY + (row + d->_pQuietZone) * moduleSize;
				painter.drawRect(QRectF(x, y, moduleSize + 0.5, moduleSize + 0.5));
			}
		}
	}
}

namespace
{
	static uint8_t gfExp[512];
	static uint8_t gfLog[256];
	static bool gfInitDone = false;

	void gfInit()
	{
		if (gfInitDone)
		{
			return;
		}
		gfInitDone = true;
		int x = 1;
		for (int i = 0; i < 255; i++)
		{
			gfExp[i] = (uint8_t) x;
			gfLog[x] = (uint8_t) i;
			x <<= 1;
			if (x & 0x100)
			{
				x ^= 0x11D;
			}
		}
		for (int i = 255; i < 512; i++)
		{
			gfExp[i] = gfExp[i - 255];
		}
	}

	uint8_t gfMul(uint8_t a, uint8_t b)
	{
		if (a == 0 || b == 0)
		{
			return 0;
		}
		return gfExp[gfLog[a] + gfLog[b]];
	}

	QVector<uint8_t> rsGeneratorPoly(int eccLen)
	{
		QVector<uint8_t> g(eccLen + 1, 0);
		g[0] = 1;
		for (int i = 0; i < eccLen; i++)
		{
			for (int j = eccLen; j >= 1; j--)
			{
				g[j] = g[j - 1] ^ gfMul(g[j], gfExp[i]);
			}
			g[0] = gfMul(g[0], gfExp[i]);
		}
		return g;
	}

	QVector<uint8_t> rsEncode(const QVector<uint8_t> &data, int eccLen)
	{
		QVector<uint8_t> gen = rsGeneratorPoly(eccLen);
		QVector<uint8_t> ecc(eccLen, 0);
		for (int i = 0; i < data.size(); i++)
		{
			uint8_t coef = data[i] ^ ecc[0];
			for (int j = 0; j < eccLen - 1; j++)
			{
				ecc[j] = ecc[j + 1] ^ gfMul(coef, gen[eccLen - 1 - j]);
			}
			ecc[eccLen - 1] = gfMul(coef, gen[0]);
		}
		return ecc;
	}

	struct VersionInfo
	{
		int totalCodewords;
		int dataCodewords;
		int eccPerBlock;
		int numBlocks;
	};

	static const VersionInfo VERSION_INFO[4][11] =
	{
		{
			{0, 0, 0, 0}, {26, 19, 7, 1}, {44, 34, 10, 1}, {70, 55, 15, 1}, {100, 80, 20, 1},
			{134, 108, 26, 1}, {172, 136, 18, 2}, {196, 156, 20, 2}, {242, 194, 24, 2}, {292, 232, 30, 2}, {346, 274, 18, 4},
		},
		{
			{0, 0, 0, 0}, {26, 16, 10, 1}, {44, 28, 16, 1}, {70, 44, 26, 1}, {100, 64, 18, 2},
			{134, 86, 24, 2}, {172, 108, 16, 4}, {196, 124, 18, 4}, {242, 154, 22, 4}, {292, 182, 22, 4}, {346, 216, 26, 6},
		},
		{
			{0, 0, 0, 0}, {26, 13, 13, 1}, {44, 22, 22, 1}, {70, 34, 18, 2}, {100, 48, 26, 2},
			{134, 62, 18, 4}, {172, 76, 24, 4}, {196, 88, 18, 6}, {242, 110, 22, 6}, {292, 132, 20, 8}, {346, 154, 24, 8},
		},
		{
			{0, 0, 0, 0}, {26, 9, 17, 1}, {44, 16, 28, 1}, {70, 26, 22, 2}, {100, 36, 16, 4},
			{134, 46, 22, 4}, {172, 60, 28, 4}, {196, 66, 26, 5}, {242, 86, 26, 6}, {292, 100, 24, 8}, {346, 122, 28, 8},
		},
	};

	int selectVersion(int dataBytes, int ecLevel)
	{
		for (int v = 1; v <= 10; v++)
		{
			int countBits = (v <= 9) ? 8 : 16;
			int headerBits = 4 + countBits;
			int dataBits = dataBytes * 8;
			int totalBits = headerBits + dataBits;
			int totalBytes = (totalBits + 7) / 8;
			if (totalBytes <= VERSION_INFO[ecLevel][v].dataCodewords)
			{
				return v;
			}
		}
		return 10;
	}

	QVector<uint8_t> encodeDataCodewords(const QByteArray &utf8, int version, int ecLevel)
	{
		int countBits = (version <= 9) ? 8 : 16;
		int dataCodewords = VERSION_INFO[ecLevel][version].dataCodewords;

		QVector<bool> bits;

		bits << false << true << false << false;

		for (int i = countBits - 1; i >= 0; i--)
		{
			bits << ((utf8.size() >> i) & 1);
		}

		for (int i = 0; i < utf8.size(); i++)
		{
			uint8_t b = (uint8_t) utf8[i];
			for (int j = 7; j >= 0; j--)
			{
				bits << ((b >> j) & 1);
			}
		}

		int maxBits = dataCodewords * 8;
		for (int i = 0; i < 4 && bits.size() < maxBits; i++)
		{
			bits << false;
		}

		while (bits.size() % 8 != 0 && bits.size() < maxBits)
		{
			bits << false;
		}

		QVector<uint8_t> codewords;
		for (int i = 0; i < bits.size(); i += 8)
		{
			uint8_t byte = 0;
			for (int j = 0; j < 8 && i + j < bits.size(); j++)
			{
				byte = (byte << 1) | (bits[i + j] ? 1 : 0);
			}
			codewords.append(byte);
		}

		bool toggle = true;
		while (codewords.size() < dataCodewords)
		{
			codewords.append(toggle ? 0xEC : 0x11);
			toggle = !toggle;
		}
		return codewords;
	}

	static const int ALIGN_POS[][7] =
	{
		{},
		{},
		{6, 18},
		{6, 22},
		{6, 26},
		{6, 30},
		{6, 34},
		{6, 22, 38},
		{6, 24, 42},
		{6, 26, 46},
		{6, 28, 50},
	};
	static const int ALIGN_COUNT[] = {0, 0, 2, 2, 2, 2, 2, 3, 3, 3, 3};

	void setMod(QVector<QVector<bool> > &mat, QVector<QVector<bool> > &func, int r, int c, bool val)
	{
		int n = mat.size();
		if (r >= 0 && r < n && c >= 0 && c < n)
		{
			mat[r][c] = val;
			func[r][c] = true;
		}
	}

	void drawFinderPattern(QVector<QVector<bool> > &mat, QVector<QVector<bool> > &func, int r, int c)
	{
		for (int dr = -1; dr <= 7; dr++)
		{
			for (int dc = -1; dc <= 7; dc++)
			{
				bool black = (dr >= 0 && dr <= 6 && (dc == 0 || dc == 6)) ||
				             (dc >= 0 && dc <= 6 && (dr == 0 || dr == 6)) ||
				             (dr >= 2 && dr <= 4 && dc >= 2 && dc <= 4);
				setMod(mat, func, r + dr, c + dc, black);
			}
		}
	}

	void drawAlignmentPattern(QVector<QVector<bool> > &mat, QVector<QVector<bool> > &func, int r, int c)
	{
		for (int dr = -2; dr <= 2; dr++)
		{
			for (int dc = -2; dc <= 2; dc++)
			{
				bool black = qAbs(dr) == 2 || qAbs(dc) == 2 || (dr == 0 && dc == 0);
				setMod(mat, func, r + dr, c + dc, black);
			}
		}
	}

	void drawFunctionPatterns(QVector<QVector<bool> > &mat, QVector<QVector<bool> > &func, int version)
	{
		int n = mat.size();

		drawFinderPattern(mat, func, 0, 0);
		drawFinderPattern(mat, func, 0, n - 7);
		drawFinderPattern(mat, func, n - 7, 0);

		for (int i = 8; i < n - 8; i++)
		{
			setMod(mat, func, 6, i, i % 2 == 0);
			setMod(mat, func, i, 6, i % 2 == 0);
		}

		int cnt = ALIGN_COUNT[version];
		if (cnt > 0)
		{
			for (int i = 0; i < cnt; i++)
			{
				for (int j = 0; j < cnt; j++)
				{
					int r = ALIGN_POS[version][i];
					int c = ALIGN_POS[version][j];
					if (func[r][c])
					{
						continue;
					}
					drawAlignmentPattern(mat, func, r, c);
				}
			}
		}

		for (int i = 0; i < 8; i++)
		{
			setMod(mat, func, 8, i, false);
			setMod(mat, func, i, 8, false);
			setMod(mat, func, 8, n - 1 - i, false);
			setMod(mat, func, n - 1 - i, 8, false);
		}
		setMod(mat, func, 8, 8, false);

		setMod(mat, func, n - 8, 8, true);
	}

	void placeData(QVector<QVector<bool> > &mat, const QVector<QVector<bool> > &func, const QVector<uint8_t> &data)
	{
		int n = mat.size();
		int bitIdx = 0;
		int totalBits = data.size() * 8;
		for (int right = n - 1; right >= 1; right -= 2)
		{
			if (right == 6)
			{
				right = 5;
			}
			for (int vert = 0; vert < n; vert++)
			{
				for (int j = 0; j < 2; j++)
				{
					int col = right - j;
					bool upward = ((right + 1) & 2) == 0;
					int row = upward ? n - 1 - vert : vert;
					if (!func[row][col])
					{
						bool bit = false;
						if (bitIdx < totalBits)
						{
							bit = ((data[bitIdx / 8] >> (7 - (bitIdx % 8))) & 1) != 0;
							bitIdx++;
						}
						mat[row][col] = bit;
					}
				}
			}
		}
	}

	bool getMaskedBit(bool val, int r, int c, int maskPattern)
	{
		bool mask = false;
		switch (maskPattern)
		{
			case 0: mask = (r + c) % 2 == 0;
				break;
			case 1: mask = r % 2 == 0;
				break;
			case 2: mask = c % 3 == 0;
				break;
			case 3: mask = (r + c) % 3 == 0;
				break;
			case 4: mask = (r / 2 + c / 3) % 2 == 0;
				break;
			case 5: mask = (r * c) % 2 + (r * c) % 3 == 0;
				break;
			case 6: mask = ((r * c) % 2 + (r * c) % 3) % 2 == 0;
				break;
			case 7: mask = ((r + c) % 2 + (r * c) % 3) % 2 == 0;
				break;
		}
		return mask ? !val : val;
	}

	void applyMask(QVector<QVector<bool> > &mat, const QVector<QVector<bool> > &func, int maskPattern)
	{
		int n = mat.size();
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				if (!func[r][c])
				{
					mat[r][c] = getMaskedBit(mat[r][c], r, c, maskPattern);
				}
			}
		}
	}

	int scoreMask(const QVector<QVector<bool> > &mat)
	{
		int n = mat.size();
		int penalty = 0;

		for (int r = 0; r < n; r++)
		{
			int run = 1;
			for (int c = 1; c < n; c++)
			{
				if (mat[r][c] == mat[r][c - 1])
				{
					run++;
				}
				else
				{
					if (run >= 5) penalty += run - 2;
					run = 1;
				}
			}
			if (run >= 5) penalty += run - 2;
		}
		for (int c = 0; c < n; c++)
		{
			int run = 1;
			for (int r = 1; r < n; r++)
			{
				if (mat[r][c] == mat[r - 1][c])
				{
					run++;
				}
				else
				{
					if (run >= 5) penalty += run - 2;
					run = 1;
				}
			}
			if (run >= 5) penalty += run - 2;
		}

		for (int r = 0; r < n - 1; r++)
		{
			for (int c = 0; c < n - 1; c++)
			{
				bool v = mat[r][c];
				if (v == mat[r][c + 1] && v == mat[r + 1][c] && v == mat[r + 1][c + 1])
				{
					penalty += 3;
				}
			}
		}
		return penalty;
	}

	static const uint16_t FORMAT_INFO[4][8] =
	{
		{0x77C4, 0x72F3, 0x7DAA, 0x789D, 0x662F, 0x6318, 0x6C41, 0x6976},
		{0x5412, 0x5125, 0x5E7C, 0x5B4B, 0x45F9, 0x40CE, 0x4F97, 0x4AA0},
		{0x355F, 0x3068, 0x3F31, 0x3A06, 0x24B4, 0x2183, 0x2EDA, 0x2BED},
		{0x1689, 0x13BE, 0x1CE7, 0x19D0, 0x0762, 0x0255, 0x0D0C, 0x083B},
	};

	void drawFormatInfo(QVector<QVector<bool> > &mat, int maskPattern, int ecLevel)
	{
		int n = mat.size();
		uint16_t bits = FORMAT_INFO[ecLevel][maskPattern];

		for (int i = 0; i <= 5; i++)
		{
			mat[8][i] = ((bits >> (14 - i)) & 1) != 0;
		}
		mat[8][7] = ((bits >> 8) & 1) != 0;
		mat[8][8] = ((bits >> 7) & 1) != 0;
		mat[7][8] = ((bits >> 6) & 1) != 0;
		for (int i = 0; i <= 5; i++)
		{
			mat[5 - i][8] = ((bits >> i) & 1) != 0;
		}

		for (int i = 0; i <= 7; i++)
		{
			mat[n - 1 - i][8] = ((bits >> (14 - i)) & 1) != 0;
		}
		for (int i = 8; i <= 14; i++)
		{
			mat[8][n - 15 + i] = ((bits >> (14 - i)) & 1) != 0;
		}
	}
}

ElaQRCodePrivate::ElaQRCodePrivate(QObject *parent)
	: QObject{parent}
{
}

ElaQRCodePrivate::~ElaQRCodePrivate()
{
}

void ElaQRCodePrivate::encode(const QString &text)
{
	if (text.isEmpty())
	{
		_modules.clear();
		_moduleCount = 0;
		return;
	}

	gfInit();

	QByteArray utf8 = text.toUtf8();
	int version = selectVersion(utf8.size(), _ecLevel);
	const VersionInfo &vi = VERSION_INFO[_ecLevel][version];
	_moduleCount = version * 4 + 17;

	QVector<uint8_t> dataCodewords = encodeDataCodewords(utf8, version, _ecLevel);

	int blockSize = vi.dataCodewords / vi.numBlocks;
	int remainder = vi.dataCodewords % vi.numBlocks;

	QVector<QVector<uint8_t> > dataBlocks;
	QVector<QVector<uint8_t> > eccBlocks;
	int offset = 0;
	for (int b = 0; b < vi.numBlocks; b++)
	{
		int thisBlockSize = blockSize + (b >= vi.numBlocks - remainder ? 1 : 0);
		QVector<uint8_t> block(dataCodewords.mid(offset, thisBlockSize));
		dataBlocks.append(block);
		eccBlocks.append(rsEncode(block, vi.eccPerBlock));
		offset += thisBlockSize;
	}

	QVector<uint8_t> allCodewords;
	int maxDataLen = blockSize + (remainder > 0 ? 1 : 0);
	for (int i = 0; i < maxDataLen; i++)
	{
		for (int b = 0; b < vi.numBlocks; b++)
		{
			if (i < dataBlocks[b].size())
			{
				allCodewords.append(dataBlocks[b][i]);
			}
		}
	}
	for (int i = 0; i < vi.eccPerBlock; i++)
	{
		for (int b = 0; b < vi.numBlocks; b++)
		{
			allCodewords.append(eccBlocks[b][i]);
		}
	}

	_modules.resize(_moduleCount);
	for (int i = 0; i < _moduleCount; i++)
	{
		_modules[i].resize(_moduleCount);
		_modules[i].fill(false);
	}

	QVector<QVector<bool> > func(_moduleCount);
	for (int i = 0; i < _moduleCount; i++)
	{
		func[i].resize(_moduleCount);
		func[i].fill(false);
	}

	drawFunctionPatterns(_modules, func, version);
	placeData(_modules, func, allCodewords);

	int bestMask = 0;
	int bestScore = INT_MAX;
	QVector<QVector<bool> > bestModules = _modules;

	for (int mask = 0; mask < 8; mask++)
	{
		QVector<QVector<bool> > trial = _modules;
		applyMask(trial, func, mask);
		drawFormatInfo(trial, mask, _ecLevel);
		int score = scoreMask(trial);
		if (score < bestScore)
		{
			bestScore = score;
			bestMask = mask;
			bestModules = trial;
		}
	}
	_modules = bestModules;
}