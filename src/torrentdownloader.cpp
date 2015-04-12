#include "torrentdownloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>

boost::weak_ptr<TorrentDownloader> TorrentDownloader::m_pInstance;
TorrentDownloader::TorrentDownloader(QObject* parent)
	: QObject(parent)
{
	m_pNetManager = new QNetworkAccessManager(this);
	connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyReady(QNetworkReply*)));
}

TorrentDownloader::~TorrentDownloader()
{
}

TorrentDownloaderPtr TorrentDownloader::getInstance()
{
	TorrentDownloaderPtr instance = m_pInstance.lock();

	if (!instance)
	{
		instance.reset(new TorrentDownloader(NULL));
		m_pInstance = instance;
	}

	return instance;
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

		QTemporaryFile* pFile = new QTemporaryFile();

		if (pFile->open())
		{
			pFile->write(pReply->readAll());
			pFile->close();
			QUrl replyUrl = pReply->url();
			while (m_redirectionMap.contains(replyUrl))
			{
				replyUrl = m_redirectionMap[replyUrl];
				m_redirectionMap.remove(replyUrl);
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

void TorrentDownloader::downloadTorrent(QUrl url)
{
	QNetworkRequest request(url);
	m_pNetManager->get(request);
}
