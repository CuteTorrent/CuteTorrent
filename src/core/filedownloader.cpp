#include "filedownloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>


FileDownloader::FileDownloader(QObject* parent)
	: QObject(parent)
{
	m_pNetManager = new QNetworkAccessManager(this);
	connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyReady(QNetworkReply*)));
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::replyReady(QNetworkReply* pReply)
{
	if (pReply->error() == QNetworkReply::NoError)
	{
		QUrl redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

		if (redirectionTarget.isValid())
		{
			m_redirectionMap.insert(redirectionTarget, pReply->url());
			download(redirectionTarget);
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

			emit DownloadReady(replyUrl, pFile);
		}
		else
		{
			emit DownloadError(pReply->url(), tr("UNABLE_TO_SAVE_DOWNLOADED_FILE"));
		}
	}
	else
	{
		emit DownloadError(pReply->url(), tr("NETWORK_ERROR: %1").arg(pReply->errorString()));
	}
}

void FileDownloader::download(QUrl url, QList<QNetworkCookie> cookies)
{
	QNetworkRequest request(url);

	if (cookies.size() > 0)
	{
		m_pNetManager->cookieJar()->setCookiesFromUrl(cookies, url);
	}

	m_pNetManager->get(request);
}

FileDownloaderPtr FileDownloader::getInstance()
{
	boost::shared_ptr<FileDownloader> instance = boost::make_shared<FileDownloader>();
	return instance;
}