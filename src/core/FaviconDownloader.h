#include <QtCore>
#include <QtNetwork>
#include "ServiceCommon.h"
#include "Singleton.h"
class StyleEngene;
class NetworkDiskCache;
class FaviconDownloader : public QObject, public Singleton<FaviconDownloader>
{
	friend class Singleton < FaviconDownloader > ;
	Q_OBJECT
public:
	~FaviconDownloader();
	QIcon getFavicon(QString url);
//	static FaviconDownloaderPtr getInstance();
private:
//	static boost::weak_ptr<FaviconDownloader> m_pInstance;
	QMutex* m_pSynkMutex;
	StyleEngene* m_pStyleEngine;
	QNetworkAccessManager* m_pNatworkManager;
	NetworkDiskCache* m_pDiskCache;
	QPixmap getFromWeb(QUrl url);
	QList<QString> downloadingList;
	QHash<QString, QString> redirectionMap;
protected:
	FaviconDownloader(QObject* parent = NULL);
private slots:
	void replyReady(QNetworkReply*);

};