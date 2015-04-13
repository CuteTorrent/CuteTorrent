#include <QNetworkDiskCache>
#include <QMutex>

class NetworkDiskCache : public QNetworkDiskCache
{
	Q_OBJECT
	QMutex* m_pInsertUpdateMutex;
public:
	NetworkDiskCache(QObject* parent = 0) : QNetworkDiskCache(parent)
	{
		m_pInsertUpdateMutex = new QMutex();
	}
	QIODevice* data(const QUrl& url) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::data(url);
	}
	void insert(QIODevice* device) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::insert(device);
	}
	QNetworkCacheMetaData metaData(const QUrl& url) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::metaData(url);
	}
	QIODevice* prepare(const QNetworkCacheMetaData& metaData) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::prepare(metaData);
	}
	bool remove(const QUrl& url) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::remove(url);
	}
	void updateMetaData(const QNetworkCacheMetaData& metaData) override
	{
		QMutexLocker lock(m_pInsertUpdateMutex);
		return QNetworkDiskCache::updateMetaData(metaData);
	}
};