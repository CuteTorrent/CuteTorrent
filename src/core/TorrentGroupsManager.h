#pragma once
#include <QList>
#include "TorrentGroup.h"
#include "Singleton.h"
#include <QMap>
#include <QMouseEvent>
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
protected:
	void customEvent(QEvent * event) override;
signals:
	void GroupsChanged();
	void TorrentChangedGroup();
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

class TorrentChangedGroupEvent : public QEvent
{
public:
	explicit TorrentChangedGroupEvent()
		: QEvent(TORRENT_CHANGED_GROUP_EVENT)
	{
	}
	const static Type  TORRENT_CHANGED_GROUP_EVENT = static_cast<Type>(User + 1);
};

DEFINE_PTR_CALSS(TorrentGroupsManager)