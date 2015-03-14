#pragma once
#include <QtCore>
#include <libtorrent/rss.hpp>
#include "RssFeed.h"
#include "RssFeedTreeItem.h"
class TorrentManager;
class QRssDisplayModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum Role{ RssFeedRole = Qt::UserRole, RssFeedItemRole = Qt::UserRole + 1 };
	QRssDisplayModel(TorrentManager* pTorrentManager, QObject* parrent = NULL);
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;

private:
	RssFeedList m_feeds;
	TorrentManager* m_pTorrentsManager;
	QList<RssFeedTreeItem*> m_rootItems;
};
