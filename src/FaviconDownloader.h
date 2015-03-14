#include <QtCore>
#include <QtNetwork>
class StyleEngene;
class FaviconDownloader : QObject
{
	Q_OBJECT
public:
	FaviconDownloader(QObject* parent = NULL);
	~FaviconDownloader();
	QIcon getFavicon(QString url);
private:
	StyleEngene* m_pStyleEngine;
	QNetworkAccessManager* m_pNatworkManager;
	bool hasCached(QUrl url);
	QIcon getFromCache(QUrl url);
	QIcon getFromWeb(QUrl url);
	static QMap<QString, QString> downloadingList;
private slots:
	void replyReady(QNetworkReply *);
	
};