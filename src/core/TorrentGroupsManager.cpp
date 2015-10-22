#include "TorrentGroupsManager.h"
#include "Torrent.h"
#include <QDesktopServices>
#include "StaticHelpers.h"
#include <QQueue>
#include "QApplicationSettings.h"

TorrentGroupsManager::TorrentGroupsManager() 
{
	LoadGroups();
}

void TorrentGroupsManager::FillMaps()
{
	QQueue<TorrentGroup*> traverseQueue;
	for (int i = 0; i < m_torrentGroups.length(); i++)
	{
		traverseQueue.enqueue(m_torrentGroups[i]);
	}

	while (!traverseQueue.isEmpty())
	{
		TorrentGroup* group = traverseQueue.dequeue();
		m_torrenTgroupsToUid.insert(group->uid(), group);
		QStringList extentions = group->extentions();
		for (int i = 0; i < extentions.length(); i++)
		{
			m_torrenTgroupsToExtention.insertMulti(extentions[i], group);
		}
		QList<TorrentGroup*> children = group->Children();
		for (int i = 0; i <children.length(); i++)
		{
			traverseQueue.enqueue(children[i]);
		}
	}
}

void TorrentGroupsManager::LoadGroups()
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString groupsPath = StaticHelpers::CombinePathes(dataDir, "groups.dat");
	QFile groupsDat(groupsPath);

	if (groupsDat.open(QIODevice::ReadOnly))
	{
		QDataStream dataStream(&groupsDat);
		dataStream.setVersion(QDataStream::Qt_4_8);
		int size = 0;
		dataStream >> size;
		for (int i = 0; i < size; i++)
		{
			TorrentGroup* pGroup = new TorrentGroup();
			dataStream >> *pGroup;
			m_torrentGroups.append(pGroup);
		}
		groupsDat.close();
	}
	else
	{
		QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
		QList<GroupForFileFiltering> oldGroups = settings->GetFileFilterGroups();
		for (int i = 0; i < oldGroups.length(); i++)
		{
			TorrentGroup* group = new TorrentGroup(oldGroups[i].Name(), oldGroups[i].Extensions().split('|'), oldGroups[i].SavePath());
			m_torrentGroups.append(group);
		}
	}
	if (m_torrentGroups.length() > 0)
	{
		FillMaps();
	}
}

void TorrentGroupsManager::customEvent(QEvent* event)
{
	if (event->type() == TorrentChangedGroupEvent::TORRENT_CHANGED_GROUP_EVENT)
	{
		emit TorrentChangedGroup();
	}
}


QList<TorrentGroup*> TorrentGroupsManager::GetTorrentGroups()
{
	return m_torrentGroups;
}

void TorrentGroupsManager::AddGroup(TorrentGroup* group, QUuid parent)
{
	if (parent.isNull())
	{
		m_torrentGroups.append(group);
	}
	else
	{
		if (m_torrenTgroupsToUid.contains(parent))
		{
			m_torrenTgroupsToUid[parent]->addChild(group);
		}
	}
	m_torrenTgroupsToUid.insert(group->uid(), group);
	QStringList extentions = group->extentions();
	for (int i = 0; i < extentions.length(); i++)
	{
		m_torrenTgroupsToExtention.insertMulti(extentions[i], group);
	}
	emit GroupsChanged();
}




TorrentGroup* TorrentGroupsManager::GetGroup(QUuid uid)
{
	if (m_torrenTgroupsToUid.contains(uid))
	{
		return m_torrenTgroupsToUid.value(uid);
	}
	return NULL;
}

TorrentGroup* TorrentGroupsManager::GetGroupByExtentions(QString ext)
{
	if (m_torrenTgroupsToExtention.contains(ext))
	{
		return m_torrenTgroupsToExtention.value(ext);
	}
	return NULL;
}

void TorrentGroupsManager::RemoveGroup(TorrentGroup* group, QUuid parent)
{
	if (parent.isNull())
	{
		m_torrentGroups.removeAll(group);
	}
	else
	{
		if (m_torrenTgroupsToUid.contains(parent))
		{
			m_torrenTgroupsToUid[parent]->removeChild(group);
		}
	}
	m_torrenTgroupsToUid.remove(group->uid());
	QStringList extentions = group->extentions();
	for (int i = 0; i < extentions.length(); i++)
	{
		m_torrenTgroupsToExtention.remove(extentions[i], group);
	}
	emit GroupsChanged();
}

void TorrentGroupsManager::RefreshFilteringGroups(QList<TorrentGroup*>& groups)
{
	if (m_torrentGroups != groups)
	{
		qDeleteAll(m_torrentGroups);
		m_torrentGroups = groups;
		FillMaps();
		emit GroupsChanged();
	}
	
}

TorrentGroup* TorrentGroupsManager::GetTorrentGroupForTorrent(Torrent* pTorrent)
{
	if (pTorrent != NULL)
	{
		QUuid groupUid = pTorrent->GetGroupUid();
		if (!groupUid.isNull())
		{
			return m_torrenTgroupsToUid.value(groupUid);
		}
	}
	return NULL;
}

void TorrentGroupsManager::SaveGroups()
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString groupsPath = StaticHelpers::CombinePathes(dataDir, "groups.dat");
	QFile groupsDat(groupsPath);

	if (groupsDat.open(QIODevice::WriteOnly))
	{
		QDataStream dataStream(&groupsDat);
		dataStream.setVersion(QDataStream::Qt_4_8);
		dataStream << m_torrentGroups.size();

		for (int i = 0; i < m_torrentGroups.size(); i++)
		{
			dataStream << *m_torrentGroups[i];
		}
		groupsDat.close();
	}
}

TorrentGroup* TorrentGroupsManager::GetGroupByName(QString groupName)
{
	if (m_torrenTgroupsToName.contains(groupName))
	{
		m_torrenTgroupsToName.value(groupName);
	}
	return NULL;
}