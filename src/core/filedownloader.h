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

class FileDownloader : public QObject
{
	Q_OBJECT

	signals:
	void DownloadReady(QUrl, QTemporaryFile*);
	void DownloadError(QUrl, QString);
public:
	FileDownloader(QObject* parent = NULL);
	void download(QUrl url, QList<QNetworkCookie> cookies = QList<QNetworkCookie>());
	static FileDownloaderPtr getNewInstance();
	~FileDownloader();
private slots:
	void replyReady(QNetworkReply* pReply);
private:
	QMap<QUrl/*redirection ulr*/, QUrl/*original url*/> m_redirectionMap;


	QNetworkAccessManager* m_pNetManager;
};

#endif // TORRENTDOWNLOADER_H


