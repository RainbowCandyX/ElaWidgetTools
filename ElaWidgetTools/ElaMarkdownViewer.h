#ifndef ELAMARKDOWNVIEWER_H
#define ELAMARKDOWNVIEWER_H

#include <QWidget>

#include "ElaProperty.h"

class ElaMarkdownViewerPrivate;
class ELA_EXPORT ElaMarkdownViewer : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaMarkdownViewer)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Markdown)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit ElaMarkdownViewer(QWidget* parent = nullptr);
    ~ElaMarkdownViewer() override;
};

#endif // ELAMARKDOWNVIEWER_H
