#pragma once

#include <QObject>
#include <QFileInfo>

class FileIconProvider : public QObject
{
	Q_OBJECT

public:
	FileIconProvider(QObject *parent);
	~FileIconProvider();

	QIcon getIcon(QFileInfo fileInfo) const;
private:
#ifdef Q_OS_WIN
	QIcon FileIconProvider::getWinIcon(const QFileInfo &fileInfo) const;
#endif
};
