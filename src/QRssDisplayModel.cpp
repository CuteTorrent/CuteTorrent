
#include "QRssDisplayModel.h"
#include "TorrentManager.h"
#include "RssFeedTreeItem.h"
#include "RssFeedItemTreeItem.h"
QRssDisplayModel::QRssDisplayModel(QTreeView* pItemsView, QObject* parrent) : QAbstractItemModel(parrent), m_pTorrentsManager(TorrentManager::getInstance())
{
	m_pItemsView = pItemsView;
	m_pUdpateTimer = new QTimer(this);
	UpdateModel();
	QObject::connect(m_pTorrentsManager, SIGNAL(OnNewFeed()), this, SLOT(UpdateModel()));
	QObject::connect(m_pTorrentsManager, SIGNAL(OnFeedDeleted()), this, SLOT(UpdateModel()));
	QObject::connect(m_pTorrentsManager, SIGNAL(OnNewFeedItem()), this, SLOT(UpdateModel()));
	QObject::connect(m_pTorrentsManager, SIGNAL(OnFeedChanged()), this, SLOT(UpdateModel()));
	QObject::connect(m_pUdpateTimer, SIGNAL(timeout()), this, SLOT(UpdateVisibleData()));
	m_pUdpateTimer->start(1000);
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
	if (role == Qt::DisplayRole)
	{
		return "DisplayText";
	}
	return QVariant();
}

int QRssDisplayModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (!parent.isValid())
	{
		qDebug() << "rowCount root_items" << m_rootItems.count();
		return m_rootItems.count();
	}
	RssBaseTreeItem* parentBaseItem = static_cast<RssBaseTreeItem*>(parent.internalPointer());
	if (parentBaseItem != NULL && parentBaseItem->GetType() == RssBaseTreeItem::Feed)
	{
		RssFeedTreeItem* pTreeFeedItem = reinterpret_cast<RssFeedTreeItem*>(parentBaseItem);
		qDebug() << "rowCount children_items" << pTreeFeedItem->Children().count();
		return pTreeFeedItem->Children().count();
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
		return createIndex(row, column, parentItem);
	}
	else
	{
		RssBaseTreeItem* parentBaseItem = static_cast<RssBaseTreeItem*>(parent.internalPointer());
		if (parentBaseItem != NULL && parentBaseItem->GetType() == RssBaseTreeItem::Feed)
		{
			parentItem = static_cast<RssFeedTreeItem*>(parent.internalPointer());
		}
		else
		{
			return QModelIndex();
		}
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

void QRssDisplayModel::UpdateModel()
{
	qDeleteAll(m_feeds);
	m_feeds.clear();
	qDeleteAll(m_rootItems);
	m_rootItems.clear();
	m_feeds = m_pTorrentsManager->GetRssFeedList();
	qDebug() << "m_feeds" << m_feeds.count();
	for each (RssFeed* var in m_feeds)
	{
		m_rootItems.append(new RssFeedTreeItem(var));
	}
	reset();
}

void QRssDisplayModel::UpdateVisibleData()
{
	QModelIndex top(index(0, 0)), bot(index(m_rootItems.count() - 1, 0));
	emit dataChanged(top, bot);
}

QList<RssFeed*> QRssDisplayModel::SelectedItems()
{
	QSet<RssFeed*> res;
	QModelIndexList selectedIndexes = m_pItemsView->selectionModel()->selectedIndexes();
	for each (QModelIndex selectedIndex in selectedIndexes)
	{
		RssBaseTreeItem* pBaseItem = static_cast<RssBaseTreeItem*>(selectedIndex.internalPointer());
		switch (pBaseItem->GetType())
		{
			case RssBaseTreeItem::Feed:
			{
				RssFeedTreeItem* pFeedTreeItem = reinterpret_cast<RssFeedTreeItem*>(pBaseItem);
				res.insert(pFeedTreeItem->GetFeed());
				break;
			}
			case RssBaseTreeItem::FeedItem:
			{
				RssFeedItemTreeItem* pFeedTreeItem = reinterpret_cast<RssFeedItemTreeItem*>(pBaseItem);
				res.insert(pFeedTreeItem->Parent()->GetFeed());
				break;
			}
			default:
			{
				qDebug() << "Unknown item type";
				break;
			}
			
		}
	}
	return res.toList();
}

RssFeed* QRssDisplayModel::SelectedItem()
{
	RssFeed* res;
	QModelIndex selectedIndex = m_pItemsView->selectionModel()->currentIndex();
	RssBaseTreeItem* pBaseItem = static_cast<RssBaseTreeItem*>(selectedIndex.internalPointer());
	switch (pBaseItem->GetType())
	{
		case RssBaseTreeItem::Feed:
		{
			RssFeedTreeItem* pFeedTreeItem = reinterpret_cast<RssFeedTreeItem*>(pBaseItem);
			res = pFeedTreeItem->GetFeed();
			break;
		}
		case RssBaseTreeItem::FeedItem:
		{
			RssFeedItemTreeItem* pFeedTreeItem = reinterpret_cast<RssFeedItemTreeItem*>(pBaseItem);
			res = pFeedTreeItem->Parent()->GetFeed();
			break;
		}
		default:
		{
			qDebug() << "Unknown item type";
			break;
		}

	}
	return res;
}

