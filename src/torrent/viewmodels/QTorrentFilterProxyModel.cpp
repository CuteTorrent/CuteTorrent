#include "QTorrentFilterProxyModel.h"
#include "QTorrentDisplayModel.h"
#include <QtCore>
QTorrentFilterProxyModel::QTorrentFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent), m_torrentFilter(EMPTY), m_currentFilterType(TORRENT), m_pUpdateLocker(new QMutex())
{
	m_pUpdateTimer = new QTimer(this);
	m_pUpdateTimer->setInterval(1000);
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
							
							return fabs(pTorrent->GetProgress() - 100.0) < DBL_EPSILON;

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

void QTorrentFilterProxyModel::Update()
{
	QMutexLocker lockMutex(m_pUpdateLocker);
	invalidateFilter();
	emit dataChanged(index(0, 0), index(rowCount() - 1, 0));
}
