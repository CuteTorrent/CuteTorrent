#ifndef TORRENTDOWNLOADER_H
#define TORRENTDOWNLOADER_H

#include <QObject>
#include "ServiceCommon.h"
#include <QMap>
#include <QUrl>
#include <QNetworkCookie>
#include "Singleton.h"
class QNetworkAccessManager;
class QNetworkReply;
class QTemporaryFile;
class TorrentDownloader : public QObject, public Singleton<TorrentDownloader>
{
	friend class Singleton<TorrentDownloader>;
	Q_OBJECT
signals:
	void TorrentReady(QUrl, QTemporaryFile*);
	void TorrentError(QUrl, QString error);
public:
	void downloadTorrent(QUrl url, QList<QNetworkCookie> cookies = QList<QNetworkCookie>());
	//static TorrentDownloaderPtr getInstance();
	~TorrentDownloader();
private slots:
	void replyReady(QNetworkReply* pReply);
private:
	QMap<QUrl/*redirection ulr*/, QUrl/*original url*/> m_redirectionMap;
	//static boost::weak_ptr<TorrentDownloader> m_pInstance;
	TorrentDownloader(QObject* parent=NULL);

	QNetworkAccessManager* m_pNetManager;
};

#endif // TORRENTDOWNLOADER_H
