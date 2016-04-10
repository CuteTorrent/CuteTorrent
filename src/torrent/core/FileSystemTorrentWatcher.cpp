#include "FileSystemTorrentWatcher.h"
#include <QFileSystemWatcher>
#include <QDir>
#include <ServiceCommon.h>
#include <QApplicationSettings.h>
#include <QVector>
#include "OpenTorrentDialog.h"
#include <NotificationSystem.h>
#include <helpers/StaticHelpers.h>
#include "TorrentManager.h"
#include <TorrentGroupsManager.h>

FileSystemTorrentWatcher::FileSystemTorrentWatcher(QObject* parent)
	: QObject(parent)
	  , m_pFileSystemWatcher(new QFileSystemWatcher(this))
	  , m_pSettings(QApplicationSettings::getInstance())
	  , m_pTorrentGroupManager(TorrentGroupsManager::getInstance())
{
	NotificationSystemPtr pNotificationSystem = NotificationSystem::getInstance();
	connect(this, SIGNAL(Notify(int, QString, QVariant)), pNotificationSystem.get(), SLOT(OnNewNotification(int, QString, QVariant)));
	connect(m_pFileSystemWatcher, SIGNAL(directoryChanged(const QString&)), SLOT(OnDirectoryChanged(const QString&)));
	connect(m_pSettings.get(), SIGNAL(PropertyChanged(QString, QString)), SLOT(OnSettngsChnaged(QString, QString)));
}

void FileSystemTorrentWatcher::addPath(QString path)
{
	disable();
	m_pFileSystemWatcher->addPath(path);
	m_currentPath = path;
	m_currentPathSnapshot = QDir(m_currentPath).entryList(QStringList() << "*.torrent", QDir::AllEntries | QDir::NoDotAndDotDot);
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
		QStringList currentSnapshot = QDir(m_currentPath).entryList(QStringList() << "*.torrent", QDir::AllEntries | QDir::NoDotAndDotDot);
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

	if (pSettings->valueBool("WatchDir", "enabled"))
	{
		bool shouldDeleteFiles = pSettings->valueBool("WatchDir", "should_delete_torrents");

		if (pSettings->valueBool("WatchDir", "show_doalog_on_new_torrent"))
		{
			for (int i = 0; i < newFiles.length(); i++)
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

					TorrentGroup* group = m_pTorrentGroupManager->GetGroupByExtentions(pTorrentInfo->baseSuffix);

					if (group != NULL)
					{
						savePath = group->savePath();
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

void FileSystemTorrentWatcher::OnSettngsChnaged(QString group, QString key)
{
	if (group == "WatchDir")
	{
		if (m_pSettings->valueBool("WatchDir", "enabled"))
		{
			QString watchDir = m_pSettings->valueString("WatchDir", "dir_to_watch");

			if (!watchDir.isEmpty())
			{
				addPath(watchDir);
			}
		}
		else
		{
			disable();
		}
	}
}

