#include "QRssDisplayModel.h"
#include "RssFeed.h"
#include "RssFeedTreeItem.h"
#include "RssFeedTreeItem.h"
#include "RssFeedItemTreeItem.h"
#include "RssManager.h"
#include "messagebox.h"
#include "StyleEngene.h"
#include "OpenTorrentDialog.h"
#include "torrentdownloader.h"
#include <QInputDialog>
#include "RssItem.h"
#include <QtTest/QtTest>
QRssDisplayModel::QRssDisplayModel(QTreeView* pItemsView, QObject* parrent, bool autoUpdate) : QAbstractItemModel(parrent), m_pRssManager(RssManager::getInstance()),
	m_pTorrentDownloader(TorrentDownloader::getInstance())
{
	m_pItemsView = pItemsView;
	m_pUdpateTimer = new QTimer(this);
	setupFeedMenu();
	setupItemMenu();
	UpdateModel();
	connect(m_pRssManager.get(), SIGNAL(FeedChanged(QUuid)), this, SLOT(UpdateModel()));
	connect(m_pTorrentDownloader.get(), SIGNAL(TorrentReady(QUrl, QTemporaryFile*)), SLOT(onTorrentDownloaded(QUrl, QTemporaryFile*)));
	connect(m_pUdpateTimer, SIGNAL(timeout()), this, SLOT(UpdateVisibleData()));
	if (autoUpdate)
	{
		m_pUdpateTimer->start(1000);
	}
}

int QRssDisplayModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return 1;
}

QVariant QRssDisplayModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if (role == RssFeedRole)
	{
		RssBaseTreeItem* pBaseItem = static_cast<RssBaseTreeItem*>(index.internalPointer());
		if (pBaseItem->GetType() == RssBaseTreeItem::Feed)
		{
			return qVariantFromValue(m_pRssManager->feeds().at(index.row()));
		}
		
	}
	else if (role == RssItemRole)
	{
		RssBaseTreeItem* pBaseItem = static_cast<RssBaseTreeItem*>(index.internalPointer());
		if (pBaseItem->GetType() == RssBaseTreeItem::FeedItem)
		{
			RssFeedItemTreeItem* pFeedItemTreeitem = reinterpret_cast<RssFeedItemTreeItem*>(pBaseItem);
			return qVariantFromValue(pFeedItemTreeitem->FeedItem());
		}
	}

	return QVariant();
}

int QRssDisplayModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
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




QModelIndex QRssDisplayModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
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

QModelIndex QRssDisplayModel::parent(const QModelIndex& child) const
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
	qDebug() << Q_FUNC_INFO;
	qDeleteAll(m_rootItems);
	m_rootItems.clear();
	QList<RssFeed*> feeds = m_pRssManager->feeds();
	qDebug() << "m_feeds" << feeds.count();

	for each (RssFeed * var in feeds)
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

QList<RssFeed*> QRssDisplayModel::SelectedFeeds()
{
	QSet<RssFeed*> res;
	QModelIndexList selectedIndexes = m_pItemsView->selectionModel()->selectedIndexes();

	for each (QModelIndex selectedIndex in selectedIndexes)
	{
		if (selectedIndex.isValid())
		{
			QVariant data = selectedIndex.data(RssFeedRole);

			if (data.isValid())
			{
				RssFeed* pFeed = data.value<RssFeed*>();
				res.insert(pFeed);
			}
			else
			{
				data = selectedIndex.data(RssItemRole);
				if (data.isValid())
				{
					RssItem* pItem = data.value<RssItem*>();
					res.insert(pItem->rssFeed());
				}
			}
			
		}
	}

	return res.toList();
}

RssItem* QRssDisplayModel::SelectedRssItem()
{
	RssItem* res = nullptr;
	QModelIndex selectedIndex = m_pItemsView->selectionModel()->currentIndex();

	if (selectedIndex.isValid())
	{
		QVariant data = selectedIndex.data(RssItemRole);
		if (data.isValid())
		{
			res = data.value<RssItem*>();
			bool prevUnerad = res->unread();
			if (prevUnerad)
			{
				res->setUread(false);
				res->rssFeed()->UpdateUnreadCount();
			}
			
		}
	}

	return res;
}
RssFeed* QRssDisplayModel::SelectedFeed()
{
	RssFeed* res = nullptr;
	QModelIndex selectedIndex = m_pItemsView->selectionModel()->currentIndex();

	if (selectedIndex.isValid())
	{
		QVariant data = selectedIndex.data(RssFeedRole);
		if (data.isValid())
		{
			res = data.value<RssFeed*>();
		}
	}

	return res;
}

QRssDisplayModel::~QRssDisplayModel()
{
	qDeleteAll(m_rootItems);
	m_rootItems.clear();
}

void QRssDisplayModel::retranslate()
{
	updateAction->setText(tr("UPDATE_FEED_ITEM"));
	renameAction->setText(tr("RENAME_FEED_ITEM"));
	removeAction->setText(tr("REMOVE_FEED_ITEM"));
	downloadTorrent->setText(tr("DOWNLOAD_FEED_ITEM"));
	openLink->setText(tr("OPEN_DESC_FEED_ITEM"));
	markAsReeded->setText(tr("MARK_AS_READ"));
	markAsUnread->setText(tr("MARK_AS_UNREAD"));
	markAllAsReeded->setText(tr("MARK_ALL_AS_READ"));
	markAllAsUnread->setText(tr("MARK_ALL_AS_UNREAD"));
}

void QRssDisplayModel::setCurrentFeedUnread(bool val)
{
	RssFeed* current = SelectedFeed();

	if (current != nullptr)
	{
		QList<RssItem*> rssItems = current->GetFeedItems();
		foreach(RssItem* pItem, rssItems)
		{
			pItem->setUread(val);
		}
		current->UpdateUnreadCount();
	}
}

void QRssDisplayModel::onMarkAllUnread()
{
	if (m_pItemsView->model() == this)
	{
		setCurrentFeedUnread(true);
	}
}

void QRssDisplayModel::setCurrentItemUnread(bool val)
{
	RssItem* currentItem = SelectedRssItem();
	RssFeed* currentFeed = SelectedFeed();
	currentItem->setUread(val);
	currentFeed->UpdateUnreadCount();
}

void QRssDisplayModel::onMarkRead()
{
	if (m_pItemsView->model() == this)
	{
		setCurrentItemUnread(false);
	}
}

void QRssDisplayModel::onMarkUnread()
{
	if (m_pItemsView->model() == this)
	{
		setCurrentItemUnread(true);
	}
}

void QRssDisplayModel::contextualMenu(const QPoint& point)
{
	if (m_pItemsView->model() != this)
	{
		return;
	}

	QModelIndex index = m_pItemsView->indexAt(point);

	if (index.isValid())
	{
		RssBaseTreeItem* pBaseItem = static_cast<RssBaseTreeItem*>(index.internalPointer());
		RssBaseTreeItem::ItemType type = pBaseItem->GetType();

		if (type == RssBaseTreeItem::Feed)
		{
			m_pFeedMenu->exec(m_pItemsView->mapToGlobal(point));
		}
		else if (type == RssBaseTreeItem::FeedItem)
		{
			RssFeedItemTreeItem* pFeedItemTreeItem = reinterpret_cast<RssFeedItemTreeItem*>(pBaseItem);
			RssItem* item = pFeedItemTreeItem->FeedItem();
			downloadTorrent->setEnabled(!item->torrentUrl().isEmpty());
			openLink->setEnabled(!item->describtionLink().isEmpty());
			m_pItemMenu->exec(m_pItemsView->mapToGlobal(point));
		}
	}
}

void QRssDisplayModel::setupFeedMenu()
{
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	m_pFeedMenu = new QMenu(m_pItemsView);
	updateAction = new QAction(pStyleEngine->getIcon("update_trackers"), tr("UPDATE_FEED_ITEM"), m_pFeedMenu);
	updateAction->setObjectName("ACTION_RSSLIST_UPDATE");
	updateAction->setShortcut(Qt::Key_F5);
	connect(updateAction, SIGNAL(triggered()), SLOT(onFeedUpdate()));
	renameAction = new QAction(pStyleEngine->getIcon("move_folder"), tr("RENAME_FEED_ITEM"), m_pFeedMenu);
	renameAction->setObjectName("ACTION_RSSLIST_RENAME");
	renameAction->setShortcut(Qt::Key_F2);
	connect(renameAction, SIGNAL(triggered()), SLOT(onFeedRename()));
	removeAction = new QAction(pStyleEngine->getIcon("delete"), tr("REMOVE_FEED_ITEM"), m_pFeedMenu);
	removeAction->setObjectName("ACTION_RSSLIST_REMOVE");
	removeAction->setShortcut(Qt::Key_Delete);
	connect(removeAction, SIGNAL(triggered()), SLOT(onFeedRemove()));
	markAllAsReeded = new QAction(pStyleEngine->getIcon("mark_read"), tr("MARK_ALL_AS_READ"), m_pFeedMenu);
	markAllAsReeded->setObjectName("ACTION_RSSLIST_MARK_ALL_READ");
	connect(markAllAsReeded, SIGNAL(triggered()), SLOT(onMarkAllRead()));
	markAllAsUnread = new QAction(pStyleEngine->getIcon("mark_unread"),tr("MARK_ALL_AS_UNREAD"), m_pFeedMenu);
	markAllAsUnread->setObjectName("ACTION_RSSLIST_MARK_ALL_UNREAD");
	connect(markAllAsUnread, SIGNAL(triggered()), SLOT(onMarkAllUnread()));
	m_pFeedMenu->addAction(updateAction);
	m_pFeedMenu->addAction(renameAction);
	m_pFeedMenu->addAction(removeAction);
	m_pFeedMenu->addSeparator();
	m_pFeedMenu->addAction(markAllAsReeded);
	m_pFeedMenu->addAction(markAllAsUnread);
}

void QRssDisplayModel::onFeedUpdate()
{
	if (m_pItemsView->model() == this)
	{
		RssFeed* current = SelectedFeed();

		if (current != nullptr)
		{
			current->Update();
		}
	}
}

void QRssDisplayModel::onFeedRename()
{
	if (m_pItemsView->model() == this)
	{
		RssFeed* current = SelectedFeed();

		if (current != nullptr)
		{
			bool ok;
			QString newFeedDisplayName = QInputDialog::getText(m_pItemsView, tr("RSS_FEED_RENAME"), tr("RSS_FEED_NAME:"), QLineEdit::Normal, current->displayName(true), &ok);

			if (ok && !newFeedDisplayName.isEmpty())
			{
				current->setDisplayName(newFeedDisplayName);
			}
		}
	}
}

void QRssDisplayModel::onFeedRemove()
{
	if (m_pItemsView->model() == this)
	{
		QList<RssFeed*> selectedItems = SelectedFeeds();

		if (!selectedItems.isEmpty())
		{
			//beginResetModel();
			RssManagerPtr pRssManager = RssManager::getInstance();
			bool yesToAll = false;

			foreach(RssFeed* pFeed, selectedItems)
			{
				QMessageBox::StandardButton button;

				if (!yesToAll)
				{
					QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

					if (selectedItems.length() > 1)
					{
						buttons |= QMessageBox::YesToAll;
						buttons |= QMessageBox::NoToAll;
					}

					button = CustomMessageBox::warning(m_pItemsView, tr("RSS_FEED_DELETE"), tr("RSS_FEED_DELETE_MSG").arg(pFeed->displayName()), buttons);

					if (button == QMessageBox::YesToAll)
					{
						yesToAll = true;
					}
					else if (button == QMessageBox::NoToAll)
					{
						break;
					}
				}
				else
				{
					button = QMessageBox::YesToAll;
				}

				if (QMessageBox::No != button || yesToAll)
				{
					pRssManager->removeFeed(pFeed->uid());
				}
			}

			//endResetModel();
		}
	}
}

void QRssDisplayModel::setupItemMenu()
{
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	m_pItemMenu = new QMenu(m_pItemsView);
	downloadTorrent = new QAction(pStyleEngine->getIcon("toolbar_download"), tr("DOWNLOAD_FEED_ITEM"), m_pFeedMenu);
	downloadTorrent->setObjectName("ACTION_RSSLIST_DOWNLOAD");
	connect(downloadTorrent, SIGNAL(triggered()), SLOT(onItemDownload()));
	openLink = new QAction(pStyleEngine->getIcon("open_link"), tr("OPEN_DESC_FEED_ITEM"), m_pFeedMenu);
	openLink->setObjectName("ACTION_RSSLIST_OPEN_DESC");
	connect(openLink, SIGNAL(triggered()), SLOT(onItemOpenDesc()));
	markAsReeded = new QAction(pStyleEngine->getIcon("mark_read"), tr("MARK_AS_READ"), m_pFeedMenu);
	markAsReeded->setObjectName("ACTION_RSSLIST_MARK_AS_READ");
	connect(markAsReeded, SIGNAL(triggered()), SLOT(onMarkRead()));
	markAsUnread = new QAction(pStyleEngine->getIcon("mark_unread"), tr("MARK_AS_UNREAD"), m_pFeedMenu);
	markAsUnread->setObjectName("ACTION_RSSLIST_MARK_UNREAD");
	connect(markAsUnread, SIGNAL(triggered()), SLOT(onMarkUnread()));
	m_pItemMenu->addAction(openLink);
	m_pItemMenu->addAction(downloadTorrent);
	m_pItemMenu->addSeparator();
	m_pItemMenu->addAction(markAsReeded);
	m_pItemMenu->addAction(markAsUnread);
}

void QRssDisplayModel::onItemDownload()
{
	if (m_pItemsView->model() == this)
	{
		RssItem* current = SelectedRssItem();
		QString torrentUrl = current->torrentUrl();

		if (torrentUrl.startsWith("magnet"))
		{
			boost::scoped_ptr<OpenTorrentDialog> pDlg(new OpenTorrentDialog(m_pItemsView));
			pDlg->SetData(torrentUrl);
			pDlg->execConditional();
		}
		else
		{
			m_pTorrentDownloader->downloadTorrent(torrentUrl, current->rssFeed()->coookies());
			m_activeTorrentDownloads.append(torrentUrl);
		}
	}
}

void QRssDisplayModel::onItemOpenDesc()
{
	if (m_pItemsView->model() == this)
	{
		RssItem* current = SelectedRssItem();
		QDesktopServices::openUrl(current->describtionLink());
	}
}

void QRssDisplayModel::onTorrentDownloaded(QUrl url, QTemporaryFile* pfile)
{
	if (m_activeTorrentDownloads.contains(url))
	{
		QString fileName = pfile->fileName();
		qDebug() << "Temp FileName is: " << fileName;
		boost::scoped_ptr<OpenTorrentDialog> pDlg(new OpenTorrentDialog(m_pItemsView));
		pDlg->SetData(fileName);
		pDlg->execConditional();
		pfile->setAutoRemove(true);
		delete pfile;
	}
	
}

void QRssDisplayModel::onMarkAllRead()
{
	if (m_pItemsView->model() == this)
	{
		setCurrentFeedUnread(false);
	}
}
