
#include "QRssDisplayModel.h"
#include "TorrentManager.h"
#include "RssFeedTreeItem.h"
#include "RssFeedItemTreeItem.h"
QRssDisplayModel::QRssDisplayModel(TorrentManager* pTorrentManager, QObject* parrent) : QAbstractItemModel(parrent)
{
	m_pTorrentsManager = pTorrentManager;
	m_feeds = pTorrentManager->GetRssFeedList();
}

int QRssDisplayModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 1;
}

QVariant QRssDisplayModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (role == RssFeedItemRole && index.row() < m_feeds.count())
	{
		return qVariantFromValue(m_feeds.at(index.row()));
	}
	return QVariant();
}

int QRssDisplayModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (!parent.isValid())
	{
		return m_rootItems.count();
	}
	RssFeedTreeItem* parentItem = static_cast<RssFeedTreeItem*>(parent.internalPointer());
	if (parentItem != NULL)
	{
		return parentItem->Children().count();
	}
	return 0;
}




QModelIndex QRssDisplayModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	RssFeedTreeItem* parentItem;

	if (!parent.isValid())
	{
		parentItem = m_rootItems.at(row);
	}
	else
	{
		parentItem = static_cast<RssFeedTreeItem*>(parent.internalPointer());
	}

	RssFeedItemTreeItem* childItem = parentItem->Children().at(row);

	if (childItem != NULL)
	{
		return createIndex(row, column, childItem);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex QRssDisplayModel::parent(const QModelIndex &child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	RssBaseTreeItem* pBaseChildItem = static_cast<RssBaseTreeItem*>(child.internalPointer());
	if (pBaseChildItem->GetType() != RssBaseTreeItem::FeedItem)
	{
		return QModelIndex();
	}
	RssFeedItemTreeItem* pChildItem = reinterpret_cast<RssFeedItemTreeItem*>(pBaseChildItem);
	RssFeedTreeItem* pParentItem = pChildItem->Parent();
	int row = m_rootItems.indexOf(pParentItem);

	return createIndex(row, 0, pParentItem);
}

