#include "FiltersViewModel.h"
#include "FilterTreeItem.h"
#include <StyleEngene.h>
#include <viewmodels/QTorrentFilterProxyModel.h>
#include <GroupForFileFiltering.h>
#include <QApplicationSettings.h>
#include <core/ISerachProvider.h>
#include <core/SearchEngine.h>
#include "Torrent.h"
#include <core/TorrentStorrage.h>
#include "TorrentCommon.h"
#include <TorrentManager.h>
#include <float.h>
#include <TorrentGroupsManager.h>
#include <AddRssDwonloadRuleDialog.h>
FiltersViewModel::FiltersViewModel(Mode mode,QObject* parent) 
	: QAbstractItemModel(parent)
	, m_mode(mode)
{
	BuildTree();
	m_updateTimerID = startTimer(400);
	TorrentManagerPtr pTorrentManager = TorrentManager::getInstance();
	TorrentGroupsManagerPtr pGroupsManager = TorrentGroupsManager::getInstance();
	connect(pGroupsManager.get(), SIGNAL(GroupsChanged()), SLOT(UpdateGroupItems()));
	connect(pGroupsManager.get(), SIGNAL(TorrentChangedGroup()), SLOT(UpdateGroupCounters()));
	connect(pTorrentManager.get(), SIGNAL(TorrentAdded(Torrent*)), SLOT(UpdateGroupCounters()));
	connect(pTorrentManager.get(), SIGNAL(TorrentRemoved(QString)), SLOT(UpdateGroupCounters()));
}

FiltersViewModel::~FiltersViewModel()
{
	qDeleteAll(m_rootItems);
	m_rootItems.clear();
}

QModelIndex FiltersViewModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	if (parent.isValid())
	{
		FilterTreeItem* parentItem = static_cast<FilterTreeItem*>(parent.internalPointer());
		return createIndex(row, column, parentItem->Child(row));
	}

	return createIndex(row, column, m_rootItems[row]);
}

QModelIndex FiltersViewModel::index(QUuid groupUid)
{
	if (m_groupIndexToUid.contains(groupUid))
	{
		return m_groupIndexToUid[groupUid];
	}

	return QModelIndex();
}

QModelIndex FiltersViewModel::parent(const QModelIndex& child) const
{
	if (!child.isValid())
	{
		return QModelIndex();
	}

	FilterTreeItem* childItem = static_cast<FilterTreeItem*>(child.internalPointer());
	FilterTreeItem* parentItem = childItem->Parent();

	if (parentItem != NULL)
	{
		int row = 0;

		if (parentItem->Parent() != NULL)
		{
			row = parentItem->Parent()->Children().indexOf(parentItem);
		}
		else
		{
			row = m_rootItems.indexOf(parentItem);
		}

		return createIndex(row, 0, parentItem);
	}

	return QModelIndex();
}

int FiltersViewModel::rowCount(const QModelIndex& parent) const
{
	if (!parent.isValid())
	{
		return m_rootItems.size();
	}

	FilterTreeItem* parentItem = static_cast<FilterTreeItem*>(parent.internalPointer());
	return parentItem->Children().size();
}

int FiltersViewModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant FiltersViewModel::data(const QModelIndex& index, int role) const
{
	FilterTreeItem* filterItem = static_cast<FilterTreeItem*>(index.internalPointer());

	switch (role)
	{
		case Qt::DisplayRole:
		{
			return filterItem->DisplayText();
		}

		case Qt::DecorationRole:
		{
			return filterItem->Icon();
		}

		case FilterTypeRole:
		{
			return filterItem->FilterType();
		}

		case FilterRole:
		{
			return filterItem->Filter();
		}
	}

	return QVariant();
}

void FiltersViewModel::Retranslate()
{
	for (int i = 0; i < m_rootItems.size(); i++)
	{
		FilterTreeItem* rootItem = m_rootItems[i];

		switch (rootItem->FilterType())
		{
			case GROUP_FILTER_TYPE:
			{
				rootItem->setText(tr("TORRENT_GROUPS"));
				break;
			}

			case SEARCH:
			{
				rootItem->setText(tr("TAB_SEARCH"));
				break;
			}

			case RSS_FEED:
			{
				rootItem->setText(tr("RSS_CHANELS"));
				break;
			}

			case TORRENT:
			{
				rootItem->setText(tr("TORRENTS_ACTIVITY"));
				QList<FilterTreeItem*> children = rootItem->Children();

				for (int j = 0; j < children.size(); j++)
				{
					FilterTreeItem* torrentItem = children[j];
					TorrentFilterType torrentFilter = static_cast<TorrentFilterType>(torrentItem->Filter().toInt());

					switch (torrentFilter)
					{
						case ACTIVE:
						{
							torrentItem->setText(tr("ACTIVE_FLTR"));
							break;
						}

						case NOT_ACTIVE:
						{
							torrentItem->setText(tr("NOT_ACTIVE_FLTR"));
							break;
						}

						case SEEDING:
						{
							torrentItem->setText(tr("SEEDING_FLTR"));
							break;
						}

						case DOWNLOADING:
						{
							torrentItem->setText(tr("DOWNLOADING_FLTR"));
							break;
						}

						case COMPLETED:
						{
							torrentItem->setText(tr("COMPLETED_FLTR"));
							break;
						}
					}
				}

				break;
			}
		}
	}
}

void FiltersViewModel::UpdateGroupCounters()
{
	QList<QUuid> groupKeys = m_groupFiltersToUid.keys();
	for (int i = 0; i < m_groupFiltersToUid.size(); i++)
	{
		m_groupFiltersToUid[groupKeys[i]]->setItemsCount(-1);
	}
	m_pGroupsItem->setItemsCount(-1);
	TorrentStorragePtr torrents = TorrentStorrage::getInstance();

	for (int j = 0; j < torrents->size(); j++)
	{
		Torrent* pTorrent = torrents->at(j);

		QUuid groupUid = pTorrent->GetGroupUid();

		if (!groupUid.isNull())
		{
			if (m_groupFiltersToUid.contains(groupUid))
			{
				FilterTreeItem* filterItem = m_groupFiltersToUid[groupUid];
				uint itemsCount = filterItem->ItemsCount();
				if (itemsCount == -1)
				{
					filterItem->setItemsCount(1);
				}
				else
				{
					itemsCount++;
					filterItem->setItemsCount(itemsCount);
				}
			}

		}
	}
	QList<FilterTreeItem*> treeItems = m_groupFiltersToUid.values();
	int length = treeItems.length();
	for (int i = 0; i < length; i++)
	{
		FilterTreeItem * current = treeItems[i];
		int itemsCount = current->ItemsCount();
		if (itemsCount > 0)
		{
			do
			{
				FilterTreeItem* parent = current->Parent();
				if (parent != NULL)
				{
					int parentItemsCount = parent->ItemsCount();
					if (parentItemsCount > 0)
					{
						parent->setItemsCount(parentItemsCount + itemsCount);
					}
					else
					{
						parent->setItemsCount(itemsCount);
					}
				}
				else
				{
					break;
				}
				current = parent;
			} while (current != NULL);
		}
		
		
	}
}

void FiltersViewModel::UpdateGroupItems()
{
	beginResetModel();

	m_pGroupsItem->ClearChildren();
	QList<TorrentGroup*> groups = TorrentGroupsManager::getInstance()->GetTorrentGroups();
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	m_groupFiltersToUid.clear();
	m_groupIndexToUid.clear();
	AddGroups(pStyleEngine, m_pGroupsItem, groups, index(m_rootItems.indexOf(m_pGroupsItem),0, QModelIndex()));

	UpdateGroupCounters();
	endResetModel();
}

void FiltersViewModel::UpdateIcons()
{
	StyleEngene* pStyleEngine = StyleEngene::getInstance();

	for (int i = 0; i < m_rootItems.size(); i++)
	{
		FilterTreeItem* rootItem = m_rootItems[i];

		switch (rootItem->FilterType())
		{
			case GROUP_FILTER_TYPE:
			{
				rootItem->setIcon(pStyleEngine->getIcon("groups"));
				break;
			}

			case SEARCH:
			{
				rootItem->setIcon(pStyleEngine->getIcon("settings_search"));
				break;
			}

			case RSS_FEED:
			{
				rootItem->setIcon(pStyleEngine->getIcon("rss"));
				break;
			}

			case TORRENT:
			{
				rootItem->setIcon(pStyleEngine->getIcon("torrents"));
				QList<FilterTreeItem*> children = rootItem->Children();

				for (int j = 0; j < children.size(); j++)
				{
					FilterTreeItem* torrentItem = children[j];
					TorrentFilterType torrentFilter = static_cast<TorrentFilterType>(torrentItem->Filter().toInt());

					switch (torrentFilter)
					{
						case ACTIVE:
						{
							torrentItem->setIcon(pStyleEngine->getIcon("active"));
							break;
						}

						case NOT_ACTIVE:
						{
							QIcon activeIcon = pStyleEngine->getIcon("active");
							torrentItem->setIcon(activeIcon.pixmap(QSize(16, 16), QIcon::Disabled, QIcon::On));
							break;
						}

						case SEEDING:
						{
							torrentItem->setIcon(pStyleEngine->getIcon("uploading"));
							break;
						}

						case DOWNLOADING:
						{
							torrentItem->setIcon(pStyleEngine->getIcon("downloading"));
							break;
						}

						case COMPLETED:
						{
							torrentItem->setIcon(pStyleEngine->getIcon("completed"));
							break;
						}
					}
				}

				break;
			}
		}
	}
}

void FiltersViewModel::timerEvent(QTimerEvent* timerEvent)
{
	if (timerEvent->timerId() == m_updateTimerID)
	{
		UpdateCounters();
	}
}

void FiltersViewModel::AddGroups(StyleEngene* pStyleEngine, FilterTreeItem* groupsItem, QList<TorrentGroup*> groups, QModelIndex groupIndex)
{
	for (int i = 0; i < groups.count(); i++)
	{
		TorrentGroup* group = groups[i];
		QString extension = group->extentions()[0];
		FilterTreeItem* groupItem = new FilterTreeItem(group->name(), pStyleEngine->guessMimeIcon(extension), GROUP_FILTER_TYPE, QVariant::fromValue(group->uid()), groupsItem);
		m_groupFiltersToUid.insert(group->uid(), groupItem);
		groupsItem->AddChlld(groupItem);
		QModelIndex currentGroupIndex = index(i, 0, groupIndex);
		m_groupIndexToUid.insert(group->uid(), currentGroupIndex);
		if (group->Children().size() > 0)
		{			
			AddGroups(pStyleEngine, groupItem, group->Children(), currentGroupIndex);
		}
		
	}
}

void FiltersViewModel::BuildTree()
{
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	QIcon torrentsIcon = pStyleEngine->getIcon("torrents");
	QIcon downloadingIcon = pStyleEngine->getIcon("downloading");
	QIcon seedingIcon = pStyleEngine->getIcon("uploading");
	QIcon completedIcon = pStyleEngine->getIcon("completed");
	QIcon activeIcon = pStyleEngine->getIcon("active");
	QIcon groupsIcon = pStyleEngine->getIcon("groups");
	QIcon inactiveIcon(activeIcon.pixmap(QSize(16, 16), QIcon::Disabled, QIcon::On));
	QIcon rssIcon = pStyleEngine->getIcon("rss");
	QIcon searchIcon = pStyleEngine->getIcon("settings_search");
	if ((m_mode & Torrents) == Torrents)
	{
		FilterTreeItem* torrentsItem = new FilterTreeItem(tr("TORRENTS_ACTIVITY"), torrentsIcon, TORRENT, EMPTY);
		torrentsItem->setItemsCount(-1);
		m_rootItems.append(torrentsItem);
		FilterTreeItem* downloadingItem = new FilterTreeItem(tr("DOWNLOADING_FLTR"), downloadingIcon, TORRENT, DOWNLOADING, torrentsItem);
		torrentsItem->AddChlld(downloadingItem);
		FilterTreeItem* seedingItem = new FilterTreeItem(tr("SEEDING_FLTR"), seedingIcon, TORRENT, SEEDING, torrentsItem);
		torrentsItem->AddChlld(seedingItem);
		FilterTreeItem* completedItem = new FilterTreeItem(tr("COMPLETED_FLTR"), completedIcon, TORRENT, COMPLETED, torrentsItem);
		torrentsItem->AddChlld(completedItem);
		FilterTreeItem* activeItem = new FilterTreeItem(tr("ACTIVE_FLTR"), activeIcon, TORRENT, ACTIVE, torrentsItem);
		torrentsItem->AddChlld(activeItem);
		FilterTreeItem* inactiveItem = new FilterTreeItem(tr("NOT_ACTIVE_FLTR"), inactiveIcon, TORRENT, NOT_ACTIVE, torrentsItem);
		torrentsItem->AddChlld(inactiveItem);
	}

	if ((m_mode & Groups) == Groups)
	{
		m_pGroupsItem = new FilterTreeItem(tr("TORRENT_GROUPS"), groupsIcon, GROUP_FILTER_TYPE, QString());
		m_pGroupsItem->setItemsCount(-1);
		m_rootItems.append(m_pGroupsItem);
		UpdateGroupItems();
	}
	if ((m_mode & Rss) == Rss)
	{
		FilterTreeItem* rssItem = new FilterTreeItem(tr("RSS_CHANELS"), rssIcon, RSS_FEED, QString());
		rssItem->setItemsCount(-1);
		m_rootItems.append(rssItem);
	}

	if ((m_mode & Search) == Search)
	{
		FilterTreeItem* searchRootItem = new FilterTreeItem(tr("TAB_SEARCH"), searchIcon, SEARCH, QString());
		searchRootItem->setItemsCount(-1);
		m_rootItems.append(searchRootItem);
		QList<ISerachProvider*> searchProviders = SearchEngine::getInstance()->GetSearchProviders();
		int nSearchProvidersCount = searchProviders.size();

		for (int i = 0; i < nSearchProvidersCount; i++)
		{
			ISerachProvider* current = searchProviders.at(i);
			QString engineName = current->Name();
			FilterTreeItem* searchItem = new FilterTreeItem(engineName, current->getIcon(), SEARCH, engineName, searchRootItem);
			searchRootItem->AddChlld(searchItem);
		}
	}
}

void FiltersViewModel::UpdateCounters()
{
	int completedCount = 0;
	int activeCount = 0;
	int inactiveCount = 0;
	int seedCount = 0;
	int downloadingCount = 0;
	TorrentStorragePtr torrents = TorrentStorrage::getInstance();
	
	FilterTreeItem* torrentsItem = NULL;
	int rootTorrentRow = -1;

	for (int i = 0; i < m_rootItems.size(); i++)
	{
		if (m_rootItems[i]->FilterType() == TORRENT)
		{
			torrentsItem = m_rootItems[i];
			rootTorrentRow = i;
			break;
		}
	}

	for (int j = 0; j < torrents->size(); j++)
	{
		Torrent* pTorrent = torrents->at(j);

		if (100.f - pTorrent->GetProgress() < FLT_EPSILON)
		{
			completedCount++;
		}

		if (pTorrent->isActive())
		{
			activeCount++;
		}
		else
		{
			inactiveCount++;
		}

		if (pTorrent->isSeeding())
		{
			seedCount++;
		}
		else if (pTorrent->isDownloading())
		{
			downloadingCount++;
		}
	}



	if (torrentsItem != NULL)
	{
		torrentsItem->setItemsCount(torrents->size());
		QModelIndex rootIndex = index(rootTorrentRow, 0, QModelIndex());
		QList<FilterTreeItem*> children = torrentsItem->Children();

		for (int i = 0; i < children.size(); i++)
		{
			FilterTreeItem* child = children[i];
			TorrentFilterType filter = static_cast<TorrentFilterType>(child->Filter().toInt());

			switch (filter)
			{
				case ACTIVE:
				{
					if (child->ItemsCount() != activeCount)
					{
						child->setItemsCount(activeCount);
						QModelIndex childIndex = index(i, 0, rootIndex);
						emit dataChanged(childIndex, childIndex);
					}

					break;
				}

				case NOT_ACTIVE:
				{
					if (child->ItemsCount() != inactiveCount)
					{
						child->setItemsCount(inactiveCount);
						QModelIndex childIndex = index(i, 0, rootIndex);
						emit dataChanged(childIndex, childIndex);
					}

					break;
				}

				case SEEDING:
				{
					if (child->ItemsCount() != seedCount)
					{
						child->setItemsCount(seedCount);
						QModelIndex childIndex = index(i, 0, rootIndex);
						emit dataChanged(childIndex, childIndex);
					}

					break;
				}

				case DOWNLOADING:
				{
					if (child->ItemsCount() != downloadingCount)
					{
						child->setItemsCount(downloadingCount);
						QModelIndex childIndex = index(i, 0, rootIndex);
						emit dataChanged(childIndex, childIndex);
					}

					break;
				}

				case COMPLETED:
				{
					if (child->ItemsCount() != completedCount)
					{
						child->setItemsCount(completedCount);
						QModelIndex childIndex = index(i, 0, rootIndex);
						emit dataChanged(childIndex, childIndex);
					}

					break;
				}
			}
		}
	}
}
