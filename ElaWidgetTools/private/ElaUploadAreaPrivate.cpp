#include "ElaUploadAreaPrivate.h"

#include <QFileDialog>
#include <QFileInfo>

#include "ElaUploadArea.h"
#include "ElaTheme.h"

ElaUploadAreaPrivate::ElaUploadAreaPrivate(QObject *parent)
	: QObject{parent}
{
}

ElaUploadAreaPrivate::~ElaUploadAreaPrivate()
{
}

void ElaUploadAreaPrivate::onThemeChanged(ElaThemeType::ThemeMode themeMode)
{
	Q_Q(ElaUploadArea);
	_themeMode = themeMode;
	q->update();
}

void ElaUploadAreaPrivate::_openFileDialog()
{
	Q_Q(ElaUploadArea);
	QString dialogTitle = _pDialogTitle.isEmpty() ? "选择文件" : _pDialogTitle;
	QString filter;
	if (!_pAcceptedSuffixes.isEmpty())
	{
		QStringList patterns;
		for (const QString &suffix: _pAcceptedSuffixes)
		{
			patterns.append("*." + suffix);
		}
		filter = QString("允许的文件") + " (" + patterns.join(" ") + ")";
	}
	else if (!_mimeFilter.isEmpty())
	{
		filter = _mimeFilter;
	}

	QStringList files;
	if (_pIsMultiple)
	{
		files = QFileDialog::getOpenFileNames(q, dialogTitle, QString(), filter);
	}
	else
	{
		QString file = QFileDialog::getOpenFileName(q, dialogTitle, QString(), filter);
		if (!file.isEmpty())
		{
			files.append(file);
		}
	}
	if (!files.isEmpty())
	{
		_addFiles(files);
	}
}

bool ElaUploadAreaPrivate::_validateFile(const QString &filePath, QString &reason) const
{
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists())
	{
		reason = "文件不存在";
		return false;
	}
	if (!_pAcceptedSuffixes.isEmpty())
	{
		QString suffix = fileInfo.suffix().toLower();
		bool accepted = false;
		for (const QString &s: _pAcceptedSuffixes)
		{
			if (s.toLower() == suffix)
			{
				accepted = true;
				break;
			}
		}
		if (!accepted)
		{
			reason = "不支持的文件类型: " + suffix;
			return false;
		}
	}
	if (_pMaxFileSize > 0 && fileInfo.size() > _pMaxFileSize)
	{
		reason = "文件过大";
		return false;
	}
	if (_pMaxFileCount > 0 && _filePaths.size() >= _pMaxFileCount)
	{
		reason = "已达到最大文件数量";
		return false;
	}
	if (_filePaths.contains(filePath))
	{
		reason = "文件已存在";
		return false;
	}
	return true;
}

void ElaUploadAreaPrivate::_addFiles(const QStringList &filePaths)
{
	Q_Q(ElaUploadArea);
	QStringList addedFiles;
	for (const QString &filePath: filePaths)
	{
		QString reason;
		if (_validateFile(filePath, reason))
		{
			_filePaths.append(filePath);
			addedFiles.append(filePath);
			Q_EMIT q->fileAdded(filePath);
		}
		else
		{
			Q_EMIT q->fileRejected(filePath, reason);
		}
	}
	if (!addedFiles.isEmpty())
	{
		Q_EMIT q->filesSelected(_filePaths);
		q->update();
	}
}

void ElaUploadAreaPrivate::_removeFile(int index)
{
	Q_Q(ElaUploadArea);
	if (index >= 0 && index < _filePaths.size())
	{
		QString removedPath = _filePaths.takeAt(index);
		Q_EMIT q->fileRemoved(removedPath);
		Q_EMIT q->filesSelected(_filePaths);
		q->update();
	}
}