#include "KickassSearchProvider.h"
#include "json/json.h"
#include <QMessageBox>
QString KickassSearchProvider::Name()
{
	return "Kickass";
}

QString KickassSearchProvider::Url()
{
	return "https://kickass.so";
}

int KickassSearchProvider::SupportedCategories()
{
	return All;
}

void KickassSearchProvider::PeformSearch(QString token, SearchCategories category, int page)
{
	QNetworkRequest request;
	request.setUrl(BuildUrl(token, category, page));
	m_pNetworkManager->get(request);
}
QString KickassSearchProvider::BuildUrl(QString token, SearchCategories category, int page)
{
	if(category != All)
	{
		return QString("https://kickass.to/json.php?q=%1+category:%2&field=seeders&order=desc&page=%3").arg(token, m_categoryMap[category], QString::number(page));
	}
	else
	{
		return QString("https://kickass.to/json.php?q=%1&field=seeders&order=desc&page=%2").arg(token, QString::number(page));
	}
}

KickassSearchProvider::KickassSearchProvider()
{
	m_pNetworkManager = new QNetworkAccessManager(this);
	QObject::connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)),
	                 this, SLOT(replyReady(QNetworkReply*)));
	m_categoryMap[Anime] = "anime";
	m_categoryMap[Music] = "music";
	m_categoryMap[TV] = "tv";
	m_categoryMap[Porn] = "xxx";
	m_categoryMap[Software] = "applications";
	m_categoryMap[Games] = "games";
	m_categoryMap[Books] = "books";
	m_categoryMap[Movie] = "movies";
}

void KickassSearchProvider::replyReady(QNetworkReply* pReply)
{
	if(pReply->error() == QNetworkReply::NoError)
	{
		QList<SearchResult*> results;
		QString contentType = pReply->header(QNetworkRequest::ContentTypeHeader).toString();
		QString encoding = detectEncoding(contentType);
		QTextCodec* codec = QTextCodec::codecForName(encoding.toLocal8Bit());
		QString data = codec->makeDecoder()->toUnicode(pReply->readAll());
		bool isValidJson = false;
		QtJson::JsonObject json = QtJson::parse(data, isValidJson).toMap();

		if(isValidJson)
		{
			QtJson::JsonArray resultList = json["list"].toList();

			for(int i = 0; i < resultList.length(); i++)
			{
				QtJson::JsonObject torrentInfo = resultList.at(i).toMap();
				SearchResult* result = new SearchResult;
				result->Engine = Name();
				result->Name = torrentInfo["title"].toString();
				result->TorrentFileUrl = torrentInfo["torrentLink"].toString();
				result->TorrentDescUrl = torrentInfo["link"].toString();
				result->leechers = torrentInfo["leechs"].toInt();
				result->seeders = torrentInfo["seeds"].toInt();
				result->size = torrentInfo["size"].toLongLong();
				results.append(result);
			}

			emit SearchReady(results);
		}
	}
	else
	{
		emit Error(pReply->errorString());
	}
}

QString KickassSearchProvider::detectEncoding(QString contentType)
{
	QString encoding = "UTF-8";
	QStringList parts = contentType.split(';');
	for each (QString part in parts)
	{
		if (part.contains("charset"))
		{
			QStringList charsetParts = part.split('=');
			encoding = charsetParts.last();
		}
	}
	return encoding;
}


