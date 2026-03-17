#ifndef ELAMARKDOWNVIEWERPRIVATE_H
#define ELAMARKDOWNVIEWERPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class QTextBrowser;
class ElaMarkdownViewer;
class ElaMarkdownViewerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaMarkdownViewer)
    Q_PROPERTY_CREATE_D(QString, Markdown)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit ElaMarkdownViewerPrivate(QObject* parent = nullptr);
    ~ElaMarkdownViewerPrivate() override;

private:
    ElaThemeType::ThemeMode _themeMode;
    QTextBrowser* _textBrowser{nullptr};
    void _applyThemeStyle();
};

#endif // ELAMARKDOWNVIEWERPRIVATE_H
