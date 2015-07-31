
#include "FaviconDownloader.h"
#include "StaticHelpers.h"
#include "StyleEngene.h"
FaviconDownloader::FaviconDownloader(QObject* parent) : QObject(parent), m_pStyleEngine(StyleEngene::getInstance())
{
	m_pNatworkManager = new QNetworkAccessManager(this);
	m_pDiskCache = StaticHelpers::GetGLobalWebCache();
	m_pNatworkManager->setCache(m_pDiskCache);
	m_pSynkMutex = new QMutex();
	QObject::connect(m_pNatworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyReady(QNetworkReply*)));
}

FaviconDownloader::~FaviconDownloader()
{
	delete m_pNatworkManager;
	m_pNatworkManager = NULL;
}

QPixmap FaviconDownloader::getFavicon(QString urlStr)
{
	QUrl url(urlStr);
	QString faviconUrlStr = QString("%1://%2/favicon.ico").arg(url.scheme(), url.host());
	QUrl faviconUrl(faviconUrlStr);
	QIODevice* pData = m_pDiskCache->data(faviconUrl);

	if (pData != NULL)
	{
		QByteArray iconData = pData->readAll();
		QPixmap pixmap;
		pixmap.loadFromData(iconData);
		delete pData;
		return pixmap;
	}

	return getFromWeb(faviconUrl);
}

void FaviconDownloader::InsertWarningInCache(QUrl& url)
{
	QNetworkCacheMetaData metaData;
	QNetworkCacheMetaData::AttributesMap atts;
	QString urlStr = url.toString();
	while (redirectionMap.contains(urlStr))
	{
		urlStr = redirectionMap[urlStr];
		redirectionMap.remove(urlStr);
	}

	metaData.setUrl(url);
	metaData.setSaveToDisk(true);
	atts[QNetworkRequest::HttpStatusCodeAttribute] = 200;
	atts[QNetworkRequest::HttpReasonPhraseAttribute] = "Ok";
	metaData.setAttributes(atts);
	metaData.setLastModified(QDateTime::currentDateTime());
	metaData.setExpirationDate(QDateTime::currentDateTime().addDays(1));
	QIODevice* dev = m_pDiskCache->prepare(metaData);

	if (!dev)
	{
		return;
	}

	QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(32, 32).save(dev, "PNG");
	m_pDiskCache->insert(dev);
}

void FaviconDownloader::replyReady(QNetworkReply* pReply)
{
	QUrl url = pReply->url();
	QVariant fromCache = pReply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
	downloadingList.removeOne(url.host());

	if (pReply->error() == QNetworkReply::NoError || pReply->error() == QNetworkReply::ContentNotFoundError)
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

				if (pos > -1)
				{
					linkFavicon = rx.cap(1);
					QUrl urlFavicon(linkFavicon);

					if (urlFavicon.host().isEmpty())
					{
						urlFavicon.setHost(url.host());
					}

					if (urlFavicon.scheme().isEmpty())
					{
						urlFavicon.setScheme(url.scheme());
					}

					linkFavicon = urlFavicon.toString();
					redirectionMap.insert(linkFavicon, url.toString());
					getFromWeb(linkFavicon);
				}
			}
			else
			{
				InsertWarningInCache(url);
			}
		}
		else
		{
			QUrl redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

			if (redirectionTarget.isValid())
			{
				if (redirectionTarget.host().isNull())
				{
					redirectionTarget.setUrl("http://" + url.host() + redirectionTarget.toString());
				}

				redirectionMap.insert(redirectionTarget.toString(), url.toString());
				getFromWeb(redirectionTarget);
			}
			else
			{
				QString urlStr = url.toString();
				if (redirectionMap.contains(urlStr))
				{
					while (redirectionMap.contains(urlStr))
					{
						urlStr = redirectionMap[urlStr];
						redirectionMap.remove(urlStr);
					}


					QNetworkCacheMetaData metaData;
					QNetworkCacheMetaData::AttributesMap atts;
					metaData.setUrl(url);
					metaData.setSaveToDisk(true);
					atts[QNetworkRequest::HttpStatusCodeAttribute] = 200;
					atts[QNetworkRequest::HttpReasonPhraseAttribute] = "Ok";
					metaData.setAttributes(atts);
					metaData.setLastModified(QDateTime::currentDateTime());
					metaData.setExpirationDate(QDateTime::currentDateTime().addDays(1));
					QIODevice* dev = m_pDiskCache->prepare(metaData);

					if (!dev)
					{
						return;
					}

					dev->write(pReply->readAll());
					m_pDiskCache->insert(dev);
				}
			}
		}
	}
	else
	{
		qCritical() << "FaviconDownloader error:" << pReply->errorString();
		InsertWarningInCache(url);
	}
}


QPixmap FaviconDownloader::getFromWeb(QUrl url)
{
	if (!downloadingList.contains(url.host()))
	{
		downloadingList.append(url.host());
		QNetworkRequest request(url);
		request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
		m_pNatworkManager->get(request);
	}

	return m_pStyleEngine->getIcon("toolbar_download").pixmap(16);
}



