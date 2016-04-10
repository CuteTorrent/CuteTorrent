#include "TorrentStorrage.h"
#include "Torrent.h"

bool LessThan(Torrent* left, Torrent* right)
{
	return * (left) < * (right);
}

void TorrentStorrage::append(Torrent* torrent)
{
	QString infoHash = torrent->GetInfoHash();

	if (hasTorrent(infoHash))
	{
		return;
	}

	QMutexLocker mutexLocker(m_pMapSynkMutex);
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.insert(infoHash, torrent);
	QList::append(torrent);
}

void TorrentStorrage::remove(Torrent* torrent)
{
	remove(torrent->GetInfoHash());
}

Torrent* TorrentStorrage::getTorrent(QString infoHash)
{
	QMutexLocker mutexLocker(m_pMapSynkMutex);
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.find(infoHash);

	if (it == m_torrentsMap.end())
	{
		return NULL;
	}

	return it.value();
}

void TorrentStorrage::remove(QString infoHash)
{
	QMutexLocker mutexLocker(m_pMapSynkMutex);
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.find(infoHash);
	int index = indexOf(it.value());

	if (index >= 0)
	{
		QList<Torrent*>::removeAt(index);
		m_torrentsMap.remove(infoHash);
	}
}

bool TorrentStorrage::hasTorrent(Torrent* tor)
{
	return hasTorrent(tor->GetInfoHash());
}

bool TorrentStorrage::hasTorrent(QString infoHash)
{
	QMutexLocker mutexLocker(m_pMapSynkMutex);
	return m_torrentsMap.contains(infoHash);
}

TorrentStorrage::~TorrentStorrage(void)
{
	qDeleteAll(m_torrentsMap);
}

void TorrentStorrage::clear()
{
	m_torrentsMap.clear();
	QList::clear();
}

TorrentStorrage::TorrentStorrage(QObject* parrent/*=NULL*/) : QList<Torrent *>(), m_pMapSynkMutex(new QMutex())
{
}

Torrent* TorrentStorrage::operator[](QString infoHash)
{
	return getTorrent(infoHash);
}

void TorrentStorrage::sort()
{
	qSort(begin(), end(), LessThan);
}

