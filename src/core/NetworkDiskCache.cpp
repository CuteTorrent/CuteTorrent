#include  "NetworkDiskCache.h"
#include <QMutex>

NetworkDiskCache::NetworkDiskCache(QObject* parent) : QNetworkDiskCache(parent)
{
	m_pInsertUpdateMutex = new QMutex();
}

QIODevice* NetworkDiskCache::data(const QUrl& url)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::data(url);
}

void NetworkDiskCache::insert(QIODevice* device)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::insert(device);
}

QNetworkCacheMetaData NetworkDiskCache::metaData(const QUrl& url)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::metaData(url);
}

QIODevice* NetworkDiskCache::prepare(const QNetworkCacheMetaData& metaData)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::prepare(metaData);
}

bool NetworkDiskCache::remove(const QUrl& url)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::remove(url);
}

void NetworkDiskCache::updateMetaData(const QNetworkCacheMetaData& metaData)
{
	QMutexLocker lock(m_pInsertUpdateMutex);
	return QNetworkDiskCache::updateMetaData(metaData);
}