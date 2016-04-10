#include <QNetworkDiskCache>

class QMutex;

class NetworkDiskCache : public QNetworkDiskCache
{
	Q_OBJECT
	QMutex* m_pInsertUpdateMutex;
public:
	NetworkDiskCache(QObject* parent = 0);

	QIODevice* data(const QUrl& url) override;

	void insert(QIODevice* device) override;

	QNetworkCacheMetaData metaData(const QUrl& url) override;

	QIODevice* prepare(const QNetworkCacheMetaData& metaData) override;

	bool remove(const QUrl& url) override;

	void updateMetaData(const QNetworkCacheMetaData& metaData) override;
};

