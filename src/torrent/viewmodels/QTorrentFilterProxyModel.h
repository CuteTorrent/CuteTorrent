#pragma once
#include "defs.h"
#include "TorrentGroupsManager.h"
#include <QSortFilterProxyModel>
#include <QMutex>

enum TorrentFilterType
{
	ACTIVE,
	NOT_ACTIVE,
	SEEDING,
	DOWNLOADING,
	COMPLETED,
	EMPTY
};

class QTorrentFilterProxyModel : public QSortFilterProxyModel
{
private:
	Q_OBJECT
public:
	QTorrentFilterProxyModel(QObject* parent = 0);
	void setGroupFilter(QUuid groupName);
	void setTorrentFilter(TorrentFilterType activityFilter);
	void setTorrentSearchFilter(QString filter);
private:
	enum InternalFilterType
	{
		GROUP,
		TORRENT
	};

	TorrentGroupsManagerPtr m_pTorrentGroupsManager;
	QMutex* m_pUpdateLocker;
	InternalFilterType m_currentFilterType;
	QUuid m_groupFilter;
	QRegExp m_torrentSearchFilter;
	TorrentFilterType m_torrentFilter;
protected:
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

