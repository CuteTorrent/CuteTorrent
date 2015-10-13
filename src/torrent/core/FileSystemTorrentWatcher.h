#ifndef _FS_TORRENT_WATCHEDR_INCLUDED
#define _FS_TORRENT_WATCHEDR_INCLUDED
#include <QObject>
#include <QStringList>
#include "Singleton.h"
#include <QVariant>
#include <QApplicationSettings.h>
#include <TorrentGroupsManager.h>
class QFileSystemWatcher;
class FileSystemTorrentWatcher : public QObject, public Singleton<FileSystemTorrentWatcher>
{
	Q_OBJECT
	friend class Singleton<FileSystemTorrentWatcher>;
public:

	void addPath(QString path);
	void disable();
private:
	explicit FileSystemTorrentWatcher(QObject* parent = NULL);
	Q_DISABLE_COPY(FileSystemTorrentWatcher)
	QFileSystemWatcher* m_pFileSystemWatcher;
	QString m_currentPath;
	QStringList m_currentPathSnapshot;
	QApplicationSettingsPtr m_pSettings;
	QStringList getNewFiles(QString path);
	TorrentGroupsManagerPtr m_pTorrentGroupManager;
signals:
	void Notify(int notificationType, QString message, QVariant data);
private slots:
	void OnDirectoryChanged(const QString& path);
	void OnSettngsChnaged(QString group, QString key);
};

#endif