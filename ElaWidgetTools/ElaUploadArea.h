#ifndef ELAUPLOADAREA_H
#define ELAUPLOADAREA_H

#include <QWidget>

#include "ElaDef.h"
#include "ElaProperty.h"

class ElaUploadAreaPrivate;
class ELA_EXPORT ElaUploadArea : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaUploadArea)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_REF_CREATE_Q_H(QString, Title)
    Q_PROPERTY_REF_CREATE_Q_H(QString, SubTitle)
    Q_PROPERTY_REF_CREATE_Q_H(QStringList, AcceptedSuffixes)
    Q_PROPERTY_CREATE_Q_H(int, MaxFileCount)
    Q_PROPERTY_CREATE_Q_H(qint64, MaxFileSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsMultiple)
    Q_PROPERTY_REF_CREATE_Q_H(QString, DialogTitle)

public:
    explicit ElaUploadArea(QWidget* parent = nullptr);
    ~ElaUploadArea() override;

    QStringList getSelectedFiles() const;
    void clearFiles();

    void setAcceptedMimeFilter(const QString& filter);
    QString getAcceptedMimeFilter() const;

Q_SIGNALS:
    Q_SIGNAL void filesSelected(const QStringList& filePaths);
    Q_SIGNAL void fileAdded(const QString& filePath);
    Q_SIGNAL void fileRemoved(const QString& filePath);
    Q_SIGNAL void fileRejected(const QString& filePath, const QString& reason);

protected:
    void paintEvent(QPaintEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;
};

#endif // ELAUPLOADAREA_H
