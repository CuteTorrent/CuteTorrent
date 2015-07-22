#include "FileSystemTorrentWatcher.h"
#include <QFileSystemWatcher>
#include <QDir>
#include <ServiceCommon.h>
#include <QApplicationSettings.h>
#include <QVector>
#include "OpenTorrentDialog.h"
#include <NotificationSystem.h>
FileSystemTorrentWatcher::FileSystemTorrentWatcher(QObject* parent) 
	: QObject(parent)
	, m_pFileSystemWatcher(new QFileSystemWatcher(this))
{
	
	NotificationSystemPtr pNotificationSystem = NotificationSystem::getInstance();
	connect(this, SIGNAL(Notify(int, QString, QVariant)), pNotificationSystem.get(), SLOT(OnNewNotification(int, QString, QVariant)));
	connect(m_pFileSystemWatcher, SIGNAL(directoryChanged(const QString&)), SLOT(OnDirectoryChanged(const QString&)));
	connect(m_pFileSystemWatcher, SIGNAL(fileChanged(const QString&)), SLOT(OnDirectoryChanged(const QString&)));
}

void FileSystemTorrentWatcher::addPath(QString path)
{
	disable();
	m_pFileSystemWatcher->addPath(path);
	m_currentPath = path;
	m_currentPathSnapshot = QDir(m_currentPath).entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
}

void FileSystemTorrentWatcher::disable()
{
	if (!m_currentPath.isEmpty())
	{
		m_pFileSystemWatcher->removePath(m_currentPath);
	}
}

QStringList FileSystemTorrentWatcher::getNewFiles(QString path)
{
	QStringList newFiles;
	if (path == m_currentPath)
	{
		
		QStringList currentSnapshot = QDir(m_currentPath).entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
		QVector<QString> snapshotDiff(currentSnapshot.size() + m_currentPathSnapshot.size());
		QVector<QString>::iterator it = std::set_difference(currentSnapshot.begin(), currentSnapshot.end(), m_currentPathSnapshot.begin(), m_currentPathSnapshot.end(), snapshotDiff.begin());
		snapshotDiff.resize(it - snapshotDiff.begin());
		QDir watchDir(path);
		for (int i = 0; i < snapshotDiff.size(); i++)
		{
			newFiles << watchDir.absoluteFilePath(snapshotDiff[i]);
		}
	}

	return newFiles;
}

void FileSystemTorrentWatcher::OnDirectoryChanged(const QString& path)
{
	QApplicationSettingsPtr pSettings = QApplicationSettings::getInstance();
	QStringList newFiles = getNewFiles(path);
	TorrentManagerPtr pTorrentManager = TorrentManager::getInstance();
	if (pSettings->valueBool("WatchDir","enabled"))
	{
		bool shouldDeleteFiles = pSettings->valueBool("WatchDir", "should_delete_torrents");
		if (pSettings->valueBool("WatchDir", "show_doalog_on_new_torrent"))
		{
			for (int i = 0; i < newFiles.length();i++)
			{
				boost::scoped_ptr<OpenTorrentDialog> pDialog(new OpenTorrentDialog());
				pDialog->SetData(newFiles[i]);
				pDialog->exec();
				if (shouldDeleteFiles)
				{
					QFile::remove(newFiles[i]);
				}
			}

			
		}
		else 
		{
			for (int i = 0; i < newFiles.length(); i++)
			{
				error_code ec;
				QString torrentFilePath = newFiles[i];
				QString savePath;
				if (pSettings->valueBool("WatchDir", "use_torrent_filtering"))
				{
					boost::scoped_ptr<opentorrent_info> pTorrentInfo(pTorrentManager->GetTorrentInfo(torrentFilePath, ec));

					if (ec)
					{
						emit Notify(NotificationSystem::TORRENT_ERROR, StaticHelpers::translateLibTorrentError(ec), QVariant());
					}
					QList<GroupForFileFiltering> filters = pSettings->GetFileFilterGroups();

					foreach(GroupForFileFiltering filter, filters)
					{
						if (filter.Contains(pTorrentInfo->baseSuffix))
						{
							savePath = filter.SavePath();
						}
					}
					if (savePath.isEmpty())
					{
						savePath = pSettings->valueString("System", "LastSaveTorrentDir");
					}
				}
				else if (pSettings->valueBool("WatchDir", "use_static_save_path"))
				{
					savePath = pSettings->valueString("WatchDir", "static_save_path");
				}
				else
				{
					//Should not normaly be called...
					savePath = pSettings->valueString("System", "LastSaveTorrentDir");
				}
				pTorrentManager->AddTorrent(torrentFilePath, savePath, ec);
				if (ec)
				{
					emit Notify(NotificationSystem::TORRENT_ERROR, StaticHelpers::translateLibTorrentError(ec), QVariant());
				}
				else
				{
					if (shouldDeleteFiles)
					{
						QFile::remove(newFiles[i]);
					}
				}
			}
		}
	}
}