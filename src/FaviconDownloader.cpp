
#include "FaviconDownloader.h"
#include "StaticHelpers.h"
#include "StyleEngene.h"
QMap<QString, QString> FaviconDownloader::downloadingList;
FaviconDownloader::FaviconDownloader(QObject* parent) : QObject(parent), m_pStyleEngine(StyleEngene::getInstance())
{
	m_pNatworkManager = new QNetworkAccessManager(this);
	QObject::connect(m_pNatworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyReady(QNetworkReply*)));
}

FaviconDownloader::~FaviconDownloader()
{
	delete m_pNatworkManager;
	m_pNatworkManager = NULL;
}

QIcon FaviconDownloader::getFavicon(QString urlStr)
{
	QUrl url(urlStr);
	if (hasCached(url))
	{
		return getFromCache(url);
	}
	else
	{
		QString faviconUrl = QString("%1://%2/favicon.ico").arg(url.scheme(), url.host());
		return getFromWeb(faviconUrl);
	}
	
}

void FaviconDownloader::replyReady(QNetworkReply* pReply)
{
	QUrl url = pReply->url();
	downloadingList.remove(url.host());
	if (pReply->error() == QNetworkReply::NoError)
	{
		QByteArray data = pReply->readAll();
		QString str = QString::fromUtf8(data);
		if (str.contains("<html", Qt::CaseInsensitive)) 
		{
			QString linkFavicon;
			QRegExp rx("<link[^>]+rel=\"shortcut icon\"[^>]+>",
				Qt::CaseInsensitive, QRegExp::RegExp2);
			int pos = rx.indexIn(str);
			if (pos == -1) 
			{
				rx = QRegExp("<link[^>]+rel=\"icon\"[^>]+>",
					Qt::CaseInsensitive, QRegExp::RegExp2);
				pos = rx.indexIn(str);
			}
			if (pos > -1) 
			{
				str = rx.cap(0);
				rx.setPattern("href=\"([^\"]+)");
				pos = rx.indexIn(str);
				if (pos > -1) {
					linkFavicon = rx.cap(1);
					QUrl urlFavicon(linkFavicon);
					if (urlFavicon.host().isEmpty()) {
						urlFavicon.setHost(url.host());
					}
					if (urlFavicon.scheme().isEmpty()) {
						urlFavicon.setScheme(url.scheme());
					}
					linkFavicon = urlFavicon.toString();
					qDebug() << "Favicon URL:" << linkFavicon;
					getFromWeb(linkFavicon);
				}
			}
		}
		else
		{
			QUrl redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
			if (redirectionTarget.isValid()) 
			{
				if (redirectionTarget.host().isNull())
					redirectionTarget.setUrl("http://" + url.host() + redirectionTarget.toString());
				getFromWeb(redirectionTarget);
				
			}
			else
			{
				QString webCacheDirStr = StaticHelpers::CombinePathes(qApp->applicationDirPath(), "WebCache");
				QDir webCacheDir(webCacheDirStr);
				QString filePath = webCacheDir.filePath(url.host() + ".ico");
				QFile file(filePath);
				if (file.open(QIODevice::WriteOnly))
				{
					file.write(data);
					file.close();
				}
			}
		}
	}
}

bool FaviconDownloader::hasCached(QUrl url)
{
	QString webCacheDirStr = StaticHelpers::CombinePathes(qApp->applicationDirPath(),"WebCache");
	QDir webCacheDir(webCacheDirStr);
	if (!webCacheDir.exists())
	{
		webCacheDir.mkpath(".");
		return false;
	}
	QString filePath = webCacheDir.filePath(url.host() + ".ico");
	QFile iconFile(filePath);
	return iconFile.exists();
}

QIcon FaviconDownloader::getFromCache(QUrl url)
{
	QString webCacheDirStr = StaticHelpers::CombinePathes(qApp->applicationDirPath(), "WebCache");
	QDir webCacheDir(webCacheDirStr);
	QString filePath = webCacheDir.filePath(url.host() + ".ico");
	QIcon icon(filePath);
	return icon;
}

QIcon FaviconDownloader::getFromWeb(QUrl url)
{
	if (!downloadingList.contains(url.host()))
	{
		downloadingList.insert(url.host(), "");
		m_pNatworkManager->get(QNetworkRequest(url));
	}
	return m_pStyleEngine->getIcon("toolbar_download");
}

