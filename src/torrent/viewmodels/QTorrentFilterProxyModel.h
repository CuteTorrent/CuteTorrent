
#pragma once
#include <QtGui>
#include "defs.h"
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
	void setGroupFilter(QString groupName);
	void setTorrentFilter(TorrentFilterType activityFilter);
	void setTorrentSearchFilter(QString filter);
private:
	enum InternalFilterType
	{
		GROUP,
		TORRENT
	};
	
	QMutex* m_pUpdateLocker;
	InternalFilterType m_currentFilterType;
	QString m_groupFilter;
	QRegExp m_torrentSearchFilter;
	TorrentFilterType m_torrentFilter;
protected:
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};