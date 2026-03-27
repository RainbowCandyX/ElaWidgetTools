#ifndef ELAUPLOADAREAPRIVATE_H
#define ELAUPLOADAREAPRIVATE_H

#include <QObject>

#include "ElaDef.h"

class ElaUploadArea;
class ElaUploadAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(ElaUploadArea)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, SubTitle)
    Q_PROPERTY_CREATE_D(QStringList, AcceptedSuffixes)
    Q_PROPERTY_CREATE_D(int, MaxFileCount)
    Q_PROPERTY_CREATE_D(qint64, MaxFileSize)
    Q_PROPERTY_CREATE_D(bool, IsMultiple)
    Q_PROPERTY_CREATE_D(QString, DialogTitle)

public:
    explicit ElaUploadAreaPrivate(QObject* parent = nullptr);
    ~ElaUploadAreaPrivate() override;

    Q_SLOT void onThemeChanged(ElaThemeType::ThemeMode themeMode);

private:
    void _openFileDialog();
    bool _validateFile(const QString& filePath, QString& reason) const;
    void _addFiles(const QStringList& filePaths);
    void _removeFile(int index);

    bool _isDragOver{false};
    bool _isHover{false};
    bool _isPressed{false};
    QString _mimeFilter;
    QStringList _filePaths;
    ElaThemeType::ThemeMode _themeMode;

    friend class ElaUploadArea;
};

#endif // ELAUPLOADAREAPRIVATE_H
