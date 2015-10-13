#pragma once
#include <QList>
#include "TorrentGroup.h"
#include "Singleton.h"
#include <QMap>
class Torrent;

class TorrentGroupsManager : public QObject, public Singleton<TorrentGroupsManager>
{
	Q_OBJECT
	friend class Singleton<TorrentGroupsManager>;
	TorrentGroup* m_emptyGroup;
	QList<TorrentGroup*> m_torrentGroups;
	QMultiMap<QString, TorrentGroup*> m_torrenTgroupsToExtention;
	QMultiMap<QString, TorrentGroup*> m_torrenTgroupsToName;
	QMap<QUuid, TorrentGroup*> m_torrenTgroupsToUid;
signals:
	void GroupsChanged();
public:
	TorrentGroupsManager();
	void FillMaps();
	void LoadGroups();
	QList<TorrentGroup*> GetTorrentGroups();
	void AddGroup(TorrentGroup* group, QUuid parent = QUuid());
	void RemoveGroup(TorrentGroup* group, QUuid parent = QUuid());
	void RefreshFilteringGroups(QList<TorrentGroup*>& groups);
	TorrentGroup* GetGroup(QUuid name);
	TorrentGroup* GetGroupByExtentions(QString ext);
	TorrentGroup* GetTorrentGroupForTorrent(Torrent* pTorrent);
	void SaveGroups();
	TorrentGroup* GetGroupByName(QString groupName);
};

DEFINE_PTR_CALSS(TorrentGroupsManager)