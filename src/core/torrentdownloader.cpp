#include "torrentdownloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>


TorrentDownloader::TorrentDownloader(QObject* parent)
	: QObject(parent)
{
	m_pNetManager = new QNetworkAccessManager(this);
	connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyReady(QNetworkReply*)));
}

TorrentDownloader::~TorrentDownloader()
{
}

void TorrentDownloader::replyReady(QNetworkReply* pReply)
{
	if (pReply->error() == QNetworkReply::NoError)
	{
		QUrl redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

		if (redirectionTarget.isValid())
		{
			m_redirectionMap.insert(redirectionTarget, pReply->url());
			downloadTorrent(redirectionTarget);
			return;
		}

		QTemporaryFile* pFile = new QTemporaryFile(this);

		if (pFile->open())
		{
			pFile->write(pReply->readAll());
			pFile->close();
			QUrl replyUrl = pReply->url();

			while (m_redirectionMap.contains(replyUrl))
			{
				QUrl newReplyUrl = m_redirectionMap[replyUrl];
				m_redirectionMap.remove(replyUrl);
				replyUrl = newReplyUrl;
			}

			emit TorrentReady(replyUrl, pFile);
		}
		else
		{
			emit TorrentError(pReply->url(), tr("UNABLE_TO_SAVE_DOWNLOADED_FILE"));
		}
	}
	else
	{
		emit TorrentError(pReply->url(), tr("NETWORK_ERROR: %1").arg(pReply->errorString()));
	}
}

void TorrentDownloader::downloadTorrent(QUrl url, QList<QNetworkCookie> cookies)
{
	QNetworkRequest request(url);

	if (cookies.size() > 0)
	{
		m_pNetManager->cookieJar()->setCookiesFromUrl(cookies, url);
	}

	m_pNetManager->get(request);
}
