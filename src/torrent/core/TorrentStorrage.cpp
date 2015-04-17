#include "TorrentStorrage.h"

TorrentStorrage* TorrentStorrage::m_pInstance = NULL;

int TorrentStorrage::m_nInstanceCount = 0;

TorrentStorrage* TorrentStorrage::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new TorrentStorrage();
	}

	m_nInstanceCount++;
	return m_pInstance;
}

void TorrentStorrage::freeInstance()
{
	m_nInstanceCount--;

	if(m_nInstanceCount == 0)
	{
		delete m_pInstance;
	}
}
bool LessThan(Torrent* left, Torrent* right)
{
	return * (left) < * (right);
}
void TorrentStorrage::append(Torrent* torrent)
{
	QString infoHash = torrent->GetInfoHash();

	if(hasTorrent(infoHash))
	{
		//	delete torrent;
		return;
	}

	QMutexLocker mutexLocker(m_pMapSynkMutex);
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.insert(infoHash, torrent);
	ObservableList::append(torrent);
}

void TorrentStorrage::remove(Torrent* torrent)
{
	remove(torrent->GetInfoHash());
}

Torrent* TorrentStorrage::getTorrent(QString infoHash)
{
	QMutexLocker mutexLocker(m_pMapSynkMutex);
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.find(infoHash);

	if(it == m_torrentsMap.end())
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

	if(index >= 0)
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
	ObservableList::clear();
}

TorrentStorrage::TorrentStorrage(QObject* parrent/*=NULL*/) : ObservableList<Torrent * >(), m_pMapSynkMutex(new QMutex())
{}

Torrent* TorrentStorrage::operator[](QString index)
{
	QMutexLocker mutexLocker(m_pMapSynkMutex);
	return m_torrentsMap[index];
}

void TorrentStorrage::sort()
{
	qSort(begin(), end(), LessThan);
}
