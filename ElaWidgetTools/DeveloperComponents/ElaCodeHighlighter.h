#ifndef ELACODEHIGHLIGHTER_H
#define ELACODEHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#include "ElaTheme.h"

class ElaCodeHighlighter : public QSyntaxHighlighter
{
public:
    explicit ElaCodeHighlighter(QTextDocument* parent, ElaThemeType::ThemeMode themeMode, int language = 0)
        : QSyntaxHighlighter(parent), _themeMode(themeMode), _language(language)
    {
        setupRules();
    }

    void setThemeMode(ElaThemeType::ThemeMode themeMode)
    {
        _themeMode = themeMode;
        setupRules();
        rehighlight();
    }

    void setLanguage(int language)
    {
        _language = language;
        setupRules();
        rehighlight();
    }

protected:
    void highlightBlock(const QString& text) override
    {
        for (const HighlightRule& rule : qAsConst(_rules))
        {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext())
            {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }

        if (_hasMultiLineComment())
        {
            QTextCharFormat commentFormat;
            commentFormat.setForeground(ElaThemeColor(_themeMode, BasicDetailsText));

            setCurrentBlockState(0);
            int startIndex = 0;
            if (previousBlockState() != 1)
            {
                startIndex = text.indexOf(QRegularExpression(QStringLiteral("/\\*")));
            }
            while (startIndex >= 0)
            {
                QRegularExpressionMatch endMatch;
                int endIndex = text.indexOf(QRegularExpression(QStringLiteral("\\*/")), startIndex, &endMatch);
                int commentLength;
                if (endIndex == -1)
                {
                    setCurrentBlockState(1);
                    commentLength = text.length() - startIndex;
                }
                else
                {
                    commentLength = endIndex - startIndex + endMatch.capturedLength();
                }
                setFormat(startIndex, commentLength, commentFormat);
                startIndex = text.indexOf(QRegularExpression(QStringLiteral("/\\*")), startIndex + commentLength);
            }
        }
    }

private:
    struct HighlightRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QStringList _getKeywords() const
    {
        switch (_language)
        {
        case 1: // C
            return {"auto","break","case","char","const","continue","default","do","double","else",
                "enum","extern","float","for","goto","if","int","long","register","return",
                "short","signed","sizeof","static","struct","switch","typedef","union",
                "unsigned","void","volatile","while","NULL","inline","restrict"};
        case 2: // CSharp
            return {"abstract","as","base","bool","break","byte","case","catch","char","checked",
                "class","const","continue","decimal","default","delegate","do","double","else",
                "enum","event","explicit","extern","false","finally","fixed","float","for",
                "foreach","goto","if","implicit","in","int","interface","internal","is","lock",
                "long","namespace","new","null","object","operator","out","override","params",
                "private","protected","public","readonly","ref","return","sbyte","sealed",
                "short","sizeof","static","string","struct","switch","this","throw","true",
                "try","typeof","uint","ulong","unchecked","unsafe","ushort","using","var",
                "virtual","void","volatile","while","async","await","get","set","yield"};
        case 3: // Python
            return {"False","None","True","and","as","assert","async","await","break","class",
                "continue","def","del","elif","else","except","finally","for","from","global",
                "if","import","in","is","lambda","nonlocal","not","or","pass","raise",
                "return","try","while","with","yield","self","print","range","len","int",
                "float","str","list","dict","tuple","set","bool","type"};
        case 4: // JavaScript
            return {"await","break","case","catch","class","const","continue","debugger","default",
                "delete","do","else","export","extends","false","finally","for","function","if",
                "import","in","instanceof","let","new","null","of","return","super","switch",
                "this","throw","true","try","typeof","undefined","var","void","while","with",
                "yield","async","static","get","set","constructor","console","window","document"};
        case 5: // Lua
            return {"and","break","do","else","elseif","end","false","for","function","goto",
                "if","in","local","nil","not","or","repeat","return","then","true","until",
                "while","self","require","print","pairs","ipairs","type","tostring","tonumber",
                "table","string","math","io","os","coroutine","setmetatable","getmetatable"};
        case 6: // Rust
            return {"as","async","await","break","const","continue","crate","dyn","else","enum",
                "extern","false","fn","for","if","impl","in","let","loop","match","mod","move",
                "mut","pub","ref","return","self","Self","static","struct","super","trait",
                "true","type","unsafe","use","where","while","bool","char","f32","f64","i8",
                "i16","i32","i64","i128","isize","str","u8","u16","u32","u64","u128","usize",
                "Vec","String","Option","Result","Some","None","Ok","Err","Box","println","macro_rules"};
        case 7: // PHP
            return {"abstract","and","array","as","break","callable","case","catch","class","clone",
                "const","continue","declare","default","die","do","echo","else","elseif","empty",
                "enddeclare","endfor","endforeach","endif","endswitch","endwhile","eval","exit",
                "extends","false","final","finally","fn","for","foreach","function","global","goto",
                "if","implements","include","include_once","instanceof","insteadof","interface",
                "isset","list","match","namespace","new","null","or","print","private","protected",
                "public","readonly","require","require_once","return","static","switch","this",
                "throw","trait","true","try","unset","use","var","while","xor","yield"};
        default: // CPP
            return {"alignas","alignof","auto","bool","break","case","catch","char","char8_t",
                "char16_t","char32_t","class","concept","const","consteval","constexpr",
                "constinit","const_cast","continue","co_await","co_return","co_yield",
                "decltype","default","delete","do","double","dynamic_cast","else","emit",
                "enum","explicit","export","extern","false","float","for","friend","goto",
                "if","inline","int","long","mutable","namespace","new","noexcept","nullptr",
                "operator","override","private","protected","public","register",
                "reinterpret_cast","requires","return","short","signed","signal","sizeof",
                "slot","static","static_assert","static_cast","struct","switch","template",
                "this","thread_local","throw","true","try","typedef","typeid","typename",
                "union","unsigned","using","virtual","void","volatile","wchar_t","while",
                "Q_OBJECT","Q_PROPERTY","Q_SIGNAL","Q_SLOT","Q_EMIT","Q_INVOKABLE",
                "connect","disconnect","qDebug","qWarning","qCritical","qFatal",
                "include","define","ifdef","ifndef","endif","pragma"};
        }
    }

    QString _getSingleLineComment() const
    {
        switch (_language)
        {
        case 3: return "#";
        case 5: return "--";
        case 7: return "//|#";
        default: return "//";
        }
    }

    bool _hasMultiLineComment() const
    {
        return _language != 3 && _language != 5;
    }

    bool _hasPreprocessor() const
    {
        return _language <= 1 || _language == 7;
    }

    void setupRules()
    {
        _rules.clear();

        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(ElaThemeColor(_themeMode, PrimaryNormal));
        keywordFormat.setFontWeight(QFont::Bold);
        for (const QString& kw : _getKeywords())
        {
            _rules.append({QRegularExpression(QString("\\b%1\\b").arg(QRegularExpression::escape(kw))), keywordFormat});
        }

        QTextCharFormat stringFormat;
        stringFormat.setForeground(QColor(0x0F, 0x7B, 0x0F));
        _rules.append({QRegularExpression("\"[^\"]*\""), stringFormat});
        _rules.append({QRegularExpression("'[^']*'"), stringFormat});
        if (_language == 3 || _language == 4 || _language == 5)
        {
            _rules.append({QRegularExpression("`[^`]*`"), stringFormat});
        }

        QTextCharFormat numberFormat;
        numberFormat.setForeground(QColor(0xF7, 0x93, 0x0E));
        _rules.append({QRegularExpression("\\b[0-9]+\\.?[0-9]*[fFlLuU]?\\b"), numberFormat});
        _rules.append({QRegularExpression("\\b0[xX][0-9a-fA-F]+\\b"), numberFormat});

        QTextCharFormat commentFormat;
        commentFormat.setForeground(ElaThemeColor(_themeMode, BasicDetailsText));
        _rules.append({QRegularExpression(_getSingleLineComment() + "[^\n]*"), commentFormat});

        if (_hasPreprocessor())
        {
            QTextCharFormat ppFormat;
            ppFormat.setForeground(ElaThemeColor(_themeMode, PrimaryNormal));
            ppFormat.setFontItalic(true);
            _rules.append({QRegularExpression("^\\s*#[^\n]*"), ppFormat});
        }

        if (_language == 3)
        {
            QTextCharFormat decoFormat;
            decoFormat.setForeground(ElaThemeColor(_themeMode, PrimaryNormal));
            decoFormat.setFontItalic(true);
            _rules.append({QRegularExpression("@\\w+"), decoFormat});
        }

        if (_language == 6)
        {
            QTextCharFormat macroFormat;
            macroFormat.setForeground(ElaThemeColor(_themeMode, PrimaryNormal));
            macroFormat.setFontItalic(true);
            _rules.append({QRegularExpression("\\b\\w+!"), macroFormat});
        }

        if (_language == 7)
        {
            QTextCharFormat varFormat;
            varFormat.setForeground(QColor(0xF7, 0x93, 0x0E));
            _rules.append({QRegularExpression("\\$\\w+"), varFormat});
        }
    }

    QVector<HighlightRule> _rules;
    ElaThemeType::ThemeMode _themeMode;
    int _language{0};
};

#endif // ELACODEHIGHLIGHTER_H
