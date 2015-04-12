#include <QtCore>
#include <QtNetwork>
#include "ServiceCommon.h"
class StyleEngene;
class NetworkDiskCache;
class FaviconDownloader : QObject
{
	Q_OBJECT
public:
	~FaviconDownloader();
	QIcon getFavicon(QString url);
	static FaviconDownloaderPtr getInstance();
private:
	static boost::weak_ptr<FaviconDownloader> m_pInstance;
	FaviconDownloader(QObject* parent = NULL);
	QMutex* m_pSynkMutex;
	StyleEngene* m_pStyleEngine;
	QNetworkAccessManager* m_pNatworkManager;
	NetworkDiskCache* m_pDiskCache;
	QPixmap getFromWeb(QUrl url);
	QList<QString> downloadingList;
	QHash<QString, QString> redirectionMap;
private slots:
	void replyReady(QNetworkReply*);

};