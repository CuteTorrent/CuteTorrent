#include <QtCore>
#include <QtNetwork>
class StyleEngene;
class FaviconDownloader : QObject
{
	Q_OBJECT
public:
	FaviconDownloader(QObject* parent = NULL);
	~FaviconDownloader();
	QPixmap getFavicon(QString url);
private:
	StyleEngene* m_pStyleEngine;
	QNetworkAccessManager* m_pNatworkManager;
	QNetworkDiskCache* m_pDiskCache;
	QPixmap getFromWeb(QUrl url);
	QList<QString> downloadingList;
	QHash<QString, QString> redirectionMap;
private slots:
	void replyReady(QNetworkReply *);
	
};