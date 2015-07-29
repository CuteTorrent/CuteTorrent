#include "QTorrentFilterProxyModel.h"
#include "QTorrentDisplayModel.h"
#include <QtCore>
#include <float.h>
QTorrentFilterProxyModel::QTorrentFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent), m_torrentFilter(EMPTY), m_currentFilterType(TORRENT), m_pUpdateLocker(new QMutex())
{
	m_pUpdateTimer = new QTimer(this);
	m_pUpdateTimer->setInterval(400);
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(Update()));
	m_pUpdateTimer->start();
}

bool QTorrentFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	if (index.isValid())
	{
		Torrent* pTorrent = index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>();

		if (pTorrent != NULL)
		{
			switch (m_currentFilterType)
			{
				case GROUP:
					if (m_groupFilter.isEmpty())
					{
						return true;
					}
					return pTorrent->GetGroup().compare(m_groupFilter) == 0;

				case TORRENT:
				{
					switch (m_torrentFilter)
					{
						case ACTIVE:
							return pTorrent->isActive();

						case NOT_ACTIVE:
							return !pTorrent->isActive();

						case SEEDING:
							return pTorrent->isSeeding();

						case DOWNLOADING:
							return pTorrent->isDownloading();

						case COMPLETED:
							
							return abs(pTorrent->GetProgress() - 100.0f) < FLT_EPSILON;

						case EMPTY:
							return true;

						default:
							break;
					}

					break;
				}

				default:
					break;
			}
		}
	}

	return true;
}

void QTorrentFilterProxyModel::setGroupFilter(QString groupName)
{
	QMutexLocker lockMutex(m_pUpdateLocker);
	m_currentFilterType = GROUP;
	m_groupFilter = groupName;
	invalidateFilter();
}

void QTorrentFilterProxyModel::setTorrentFilter(TorrentFilterType activityFilter)
{
	QMutexLocker lockMutex(m_pUpdateLocker);
	m_currentFilterType = TORRENT;
	m_torrentFilter = activityFilter;
	invalidateFilter();
}

typedef QPair<QModelIndex, QModelIndex> IndexInterval;

void QTorrentFilterProxyModel::Update()
{
	QMutexLocker lockMutex(m_pUpdateLocker);
	invalidateFilter();
	//emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
	//QTime timer;
	//timer.start();
	TorrentManagerPtr pTorrentManager = TorrentManager::getInstance();
	QSet<QString> changedTorrents;
	pTorrentManager->getRecentUpdatedTorrents(changedTorrents);
	if (changedTorrents.size() > 0)
	{
		int rowCnt = rowCount();
		if (rowCnt == changedTorrents.size())
		{
			emit dataChanged(index(0, 0), index(rowCnt - 1, 0));
		}
		else
		{
			QList<IndexInterval> changedList;

			for (int i = 1; i < rowCnt; i++)
			{
				QModelIndex torrentIndex = index(i, 0);
				if (torrentIndex.isValid())
				{
					Torrent* pTorrent = torrentIndex.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>();
					if (changedTorrents.contains(pTorrent->GetInfoHash()))
					{
						if (changedList.isEmpty())
						{
							changedList.append(qMakePair(torrentIndex, torrentIndex));
						}
						else
						{
							IndexInterval last = changedList.last();
							if (last.second.row() + 1 == torrentIndex.row())
							{
								last.second = torrentIndex;
								changedList.replace(changedList.size() - 1, last);
							}
							else
							{
								changedList.append(qMakePair(torrentIndex, torrentIndex));
							}

						}
					}
				}
			}
			//	qDebug() << "Changed Intervals count " << changedList.size() << changedList;
			for (int i = 0; i < changedList.size(); i++)
			{
				IndexInterval chagedInterval = changedList[i];
				emit dataChanged(chagedInterval.first, chagedInterval.second);
			}
		}

	}
	//qDebug() << "Update take: " << double(timer.elapsed()) / 1000;
}
