#include "RssParser.h"
#include <QDebug>
#include <QTextDocument>
#include <QMutexLocker>
#include <QApplication>
#include <QDateTime>
#include "RssFeed.h"
boost::weak_ptr<RssParser> RssParser::m_pInstance;

RssParser::RssParser()
{

}

RssParserPtr RssParser::getInstance()
{
	RssParserPtr instance = m_pInstance.lock();
	if (!instance)
	{
		instance.reset(new RssParser());
		m_pInstance = instance;
	}
	return instance;
}


void RssParser::fillFeed(QIODevice* pData, RssFeed* pFeed, bool& ok, QString& error)
{
	QXmlStreamReader reader(pData);
	bool channelFound = false;
	while (reader.readNextStartElement())
	{
		if (reader.name().compare("rss", Qt::CaseInsensitive) == 0)
		{
			qDebug() << "Found rss start element. Start searching for chanels";
			while (reader.readNextStartElement())
			{
				if (reader.name().compare("channel", Qt::CaseInsensitive) == 0)
				{
					channelFound = true;
					parseRssChannel(reader, pFeed, ok, error);
				}
				else
				{
					qDebug() << "Unexpected element found " << reader.name() << ". Skiping it.";
					reader.skipCurrentElement();
				}
			}

		}
		else if (reader.name().compare("feed", Qt::CaseInsensitive) == 0)
		{
			channelFound = true;
			parseAtomChannel(reader, pFeed, ok, error);
		}
	}
	if (!channelFound)
	{
		ok = false;
		error = qApp->translate("RssParser", "No channel element found");
		return;
	}
	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}
}

void RssParser::parseRssChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error)
{
	while (!reader.atEnd())
	{
		reader.readNext();
		if (reader.isStartElement())
		{
			if (reader.name().compare("title", Qt::CaseInsensitive) == 0)
			{
				pFeed->m_title = reader.readElementText();
			}
			else if (reader.name().compare("link", Qt::CaseInsensitive) == 0)
			{
				pFeed->m_link = reader.readElementText();
			}
			else if (reader.name().compare("description", Qt::CaseInsensitive) == 0)
			{
				pFeed->m_description = reader.readElementText();
			}
			else if (reader.name().compare("ttl", Qt::CaseInsensitive) == 0)
			{
				pFeed->m_ttl = reader.readElementText().toInt();
			}
			else if (reader.name().compare("item", Qt::CaseInsensitive) == 0)
			{
				parseRssItem(reader, pFeed, ok, error);
			}


		}
	}
}

void RssParser::parseRssItem(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error)
{
	RssItem item;
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name().compare("item", Qt::CaseInsensitive) == 0)
			break;

		if (reader.isStartElement())
		{
			qDebug() << "parsing element " << reader.name();
			if (reader.name().compare("title", Qt::CaseInsensitive) == 0)
			{
				QString title = reader.readElementText();
				QTextDocument doc;
				doc.setHtml(title);
				title = doc.toPlainText();
				item["title"] = title;
			}
			else if (reader.name().compare("lastBuildDate", Qt::CaseInsensitive) == 0)
			{
				QString lastBuildDate = reader.readElementText();
				QString feedUrl = pFeed->url().toString();
				if (!lastBuildDate.isEmpty()) 
				{
					QMutexLocker locker(&m_mutex);
					if (m_lastUpdateDates.value(feedUrl, "") == lastBuildDate)
					{
						qDebug() << "The RSS feed has not changed since last time, aborting parsing.";
						return;
					}
					m_lastUpdateDates[feedUrl] = lastBuildDate;
				}
			}
			else if (reader.name().compare("guid", Qt::CaseInsensitive) == 0)
			{
				item["guid"] = reader.readElementText();
			}
			else if (reader.name().compare("category", Qt::CaseInsensitive) == 0)
			{
				item["category"] = reader.readElementText();
			}
			else if (reader.name().compare("link", Qt::CaseInsensitive) == 0)
			{
				item["link"] = reader.readElementText();
			}
			else if (reader.name().compare("description", Qt::CaseInsensitive) == 0)
			{
				item["description"] = reader.readElementText();
			}
			else if (reader.name().compare("enclosure", Qt::CaseInsensitive) == 0)
			{
				qDebug() << "enclosure attributes: " << reader.attributes().value("type");
				if (reader.attributes().value("type") == "application/x-bittorrent")
				{
					item["torrent_url"] = reader.attributes().value("url").toString();
				}
			}
			else if (reader.name().compare("torrent", Qt::CaseInsensitive) == 0)
			{
				parseTorrentSection(reader, item, ok, error);
			}
			else if (reader.name().compare("contentlength", Qt::CaseInsensitive) == 0 ||
				reader.name().compare("size", Qt::CaseInsensitive) == 0)
			{
				item["size"] = reader.readElementText().toULongLong();
			}
			else if (reader.name().compare("infohash", Qt::CaseInsensitive) == 0 || 
				reader.name().compare("info_hash", Qt::CaseInsensitive) == 0)
			{
				item["infohash"] = reader.readElementText();
			}
			else if (reader.name().compare("seeds", Qt::CaseInsensitive) == 0 ||
				reader.name().compare("seeders", Qt::CaseInsensitive) == 0)
			{
				item["seeds"] = reader.readElementText().toLong();
			}
			else if (reader.name().compare("peers", Qt::CaseInsensitive) == 0 ||
				reader.name().compare("leechers", Qt::CaseInsensitive) == 0)
			{
				item["peers"] = reader.readElementText().toLong();
			}
			
		}

	}

	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}
	if (item.isEmpty())
	{
		ok = false;
		error = qApp->translate("RssParser","No valid rss fields found.");
		return;
	}
	qDebug() << "Final item is: " << item;
	if (!item.contains("guid"))
	{
		if (!item.contains("link"))
		{
			if (!item.contains("title"))
			{
				qDebug() << "No unick identity found(link,title,guid) skipping item";
				return;
			}
			else
			{
				item["guid"] = item["title"];
			}
		}
		else
		{
			item["guid"] = item["link"];
		}
	}
	if (pFeed->m_rssItems.contains(item["guid"].toString()))
	{
		return;
	}
	item["unread"] = true;
	pFeed->m_rssItems.insert(item["guid"].toString(), item);
}

void RssParser::parseTorrentSection(QXmlStreamReader& reader, RssItem& item, bool& ok, QString& error)
{
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name().compare("torrent", Qt::CaseInsensitive) == 0)
			break;
		if (reader.isStartElement())
		{
			if (reader.name().compare("contentlength", Qt::CaseInsensitive) == 0)
			{
				item["size"] = reader.readElementText().toULongLong();
			}
			else if (reader.name().compare("magneturi", Qt::CaseInsensitive) == 0)
			{
				item["magneturi"] = reader.readElementText();
			}
			else if (reader.name().compare("infohash", Qt::CaseInsensitive) == 0)
			{
				item["infohash"] = reader.readElementText();
			}
			else if (reader.name().compare("seeds", Qt::CaseInsensitive) == 0)
			{
				item["seeds"] = reader.readElementText().toLong();
			}
			else if (reader.name().compare("peers", Qt::CaseInsensitive) == 0)
			{
				item["peers"] = reader.readElementText().toLong();
			}
		}
	}
}

void RssParser::parseAtomChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error)
{
	QString baseURL = reader.attributes().value("xml:base").toString();

	while (!reader.atEnd()) {
		reader.readNext();

		if (reader.isStartElement()) {
			if (reader.name() == "title") {
				pFeed->m_title = reader.readElementText();
				
			}
			else if (reader.name() == "updated") {
				QString lastBuildDate = reader.readElementText();
				if (!lastBuildDate.isEmpty()) {
					QMutexLocker locker(&m_mutex);
					QString feedUrl = pFeed->url().toString();
					if (m_lastUpdateDates.value(feedUrl) == lastBuildDate) {
						qDebug() << "The RSS feed has not changed since last time, aborting parsing.";
						return;
					}
					m_lastUpdateDates[feedUrl] = lastBuildDate;
				}
			}
			else if (reader.name() == "entry") {
				parseAtomArticle(reader, baseURL, pFeed, ok, error);
			}
		}
	}
}

void RssParser::parseAtomArticle(QXmlStreamReader& reader, QString baseURL, RssFeed* pFeed, bool& ok, QString& error)
{
	RssItem item;
	bool double_content = false;
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name() == "entry")
			break;

		if (reader.isStartElement()) 
		{
			if (reader.name() == "title") 
			{
				// Workaround for CDATA (QString cannot parse html escapes on it's own)
				QTextDocument doc;
				doc.setHtml(reader.readElementText());
				item["title"] = doc.toPlainText();
			}
			else if (reader.name() == "link") {
				QString theLink = (reader.attributes().isEmpty() ?
					reader.readElementText() :
					reader.attributes().value("href").toString());

				// Atom feeds can have relative links, work around this and
				// take the stress of figuring article full URI from UI

				// Assemble full URI
				item["link"] = (baseURL.isEmpty() ?
				theLink :
						baseURL + theLink);
			}
			else if (reader.name() == "summary" || reader.name() == "content"){
				if (double_content) { // Duplicate content -> ignore
					reader.readNext();

					while (reader.name() != "summary" && reader.name() != "content")
						reader.readNext();

					continue;
				}

				// Try to also parse broken articles, which don't use html '&' escapes
				// Actually works great for non-broken content too
				QString feedText = reader.readElementText(QXmlStreamReader::IncludeChildElements);
				if (!feedText.isEmpty())
					item["description"] = feedText;

				double_content = true;
			}
			else if (reader.name() == "updated"){
				// ATOM uses standard compliant date, don't do fancy stuff
				QDateTime articleDate = QDateTime::fromString(reader.readElementText(), Qt::ISODate);
				item["date"] = (articleDate.isValid() ?
				articleDate :
							QDateTime::currentDateTime());
			}
			else if (reader.name() == "author") {
				reader.readNext();
				while (reader.name() != "author") {
					if (reader.name() == "name")
						item["author"] = reader.readElementText();
					reader.readNext();
				}
			}
			else if (reader.name() == "id")
				item["guid"] = reader.readElementText();
		}
	}

	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}

	if (item.isEmpty())
	{
		ok = false;
		error = qApp->translate("RssParser", "No valid rss fields found.");
		return;
	}
	if (pFeed->m_rssItems.contains(item["guid"].toString()))
	{
		return;
	}
	item["unread"] = true;
	pFeed->m_rssItems.insert(item.value("guid").toString(),item);
}


