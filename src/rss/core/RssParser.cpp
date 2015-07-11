#include "RssParser.h"
#include <boost/smart_ptr.hpp>
#include <QDebug>
#include <QTextDocument>
#include <QApplication>
#include <QRegExp>
#include <QStringList>
#include "RssFeed.h"
#include "RssItem.h"

static const char shortDay[][4] =
{
	"Mon", "Tue", "Wed",
	"Thu", "Fri", "Sat",
	"Sun"
};
static const char longDay[][10] =
{
	"Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday",
	"Sunday"
};
static const char shortMonth[][4] =
{
	"Jan", "Feb", "Mar", "Apr",
	"May", "Jun", "Jul", "Aug",
	"Sep", "Oct", "Nov", "Dec"
};
static const char longMonth[][10] =
{
	"January", "February", "March",
	"April", "May", "June",
	"July", "August", "September",
	"October", "November", "December"
};

// Ported to Qt4 from KDElibs4
QDateTime RssParser::parseDate(const QString& string)
{
	const QString str = string.trimmed();

	if (str.isEmpty())
	{
		return QDateTime::currentDateTime();
	}

	int nyear  = 6;   // indexes within string to values
	int nmonth = 4;
	int nday   = 2;
	int nwday  = 1;
	int nhour  = 7;
	int nmin   = 8;
	int nsec   = 9;
	// Also accept obsolete form "Weekday, DD-Mon-YY HH:MM:SS ±hhmm"
	QRegExp rx("^(?:([A-Z][a-z]+),\\s*)?(\\d{1,2})(\\s+|-)([^-\\s]+)(\\s+|-)(\\d{2,4})\\s+(\\d\\d):(\\d\\d)(?::(\\d\\d))?\\s+(\\S+)$");
	QStringList parts;

	if (!str.indexOf(rx))
	{
		// Check that if date has '-' separators, both separators are '-'.
		parts = rx.capturedTexts();
		bool h1 = (parts[3] == QLatin1String("-"));
		bool h2 = (parts[5] == QLatin1String("-"));

		if (h1 != h2)
		{
			return QDateTime::currentDateTime();
		}
	}
	else
	{
		// Check for the obsolete form "Wdy Mon DD HH:MM:SS YYYY"
		rx = QRegExp("^([A-Z][a-z]+)\\s+(\\S+)\\s+(\\d\\d)\\s+(\\d\\d):(\\d\\d):(\\d\\d)\\s+(\\d\\d\\d\\d)$");

		if (str.indexOf(rx))
		{
			return QDateTime::currentDateTime();
		}

		nyear  = 7;
		nmonth = 2;
		nday   = 3;
		nwday  = 1;
		nhour  = 4;
		nmin   = 5;
		nsec   = 6;
		parts = rx.capturedTexts();
	}

	bool ok[4];
	const int day    = parts[nday].toInt(&ok[0]);
	int year   = parts[nyear].toInt(&ok[1]);
	const int hour   = parts[nhour].toInt(&ok[2]);
	const int minute = parts[nmin].toInt(&ok[3]);

	if (!ok[0] || !ok[1] || !ok[2] || !ok[3])
	{
		return QDateTime::currentDateTime();
	}

	int second = 0;

	if (!parts[nsec].isEmpty())
	{
		second = parts[nsec].toInt(&ok[0]);

		if (!ok[0])
		{
			return QDateTime::currentDateTime();
		}
	}

	bool leapSecond = (second == 60);

	if (leapSecond)
	{
		second = 59;    // apparently a leap second - validate below, once time zone is known
	}

	int month = 0;

	for ( ;  month < 12  &&  parts[nmonth] != shortMonth[month];  ++month) ;

	int dayOfWeek = -1;

	if (!parts[nwday].isEmpty())
	{
		// Look up the weekday name
		while (++dayOfWeek < 7  &&  shortDay[dayOfWeek] != parts[nwday]) ;

		if (dayOfWeek >= 7)
			for (dayOfWeek = 0;  dayOfWeek < 7  &&  longDay[dayOfWeek] != parts[nwday];  ++dayOfWeek) ;
	}

	//       if (month >= 12 || dayOfWeek >= 7
	//       ||  (dayOfWeek < 0  &&  format == RFCDateDay))
	//         return QDateTime;
	int i = parts[nyear].size();

	if (i < 4)
	{
		// It's an obsolete year specification with less than 4 digits
		year += (i == 2  &&  year < 50) ? 2000 : 1900;
	}

	// Parse the UTC offset part
	int offset = 0;           // set default to '-0000'
	bool negOffset = false;

	if (parts.count() > 10)
	{
		rx = QRegExp("^([+-])(\\d\\d)(\\d\\d)$");

		if (!parts[10].indexOf(rx))
		{
			// It's a UTC offset ±hhmm
			parts = rx.capturedTexts();
			offset = parts[2].toInt(&ok[0]) * 3600;
			int offsetMin = parts[3].toInt(&ok[1]);

			if (!ok[0] || !ok[1] || offsetMin > 59)
			{
				return QDateTime();
			}

			offset += offsetMin * 60;
			negOffset = (parts[1] == QLatin1String("-"));

			if (negOffset)
			{
				offset = -offset;
			}
		}
		else
		{
			// Check for an obsolete time zone name
			QByteArray zone = parts[10].toLatin1();

			if (zone.length() == 1  &&  isalpha(zone[0])  &&  toupper(zone[0]) != 'J')
			{
				negOffset = true;    // military zone: RFC 2822 treats as '-0000'
			}
			else if (zone != "UT" && zone != "GMT")      // treated as '+0000'
			{
				offset = (zone == "EDT")                  ? -4 * 3600
				         : (zone == "EST" || zone == "CDT") ? -5 * 3600
				         : (zone == "CST" || zone == "MDT") ? -6 * 3600
				         : (zone == "MST" || zone == "PDT") ? -7 * 3600
				         : (zone == "PST")                  ? -8 * 3600
				         : 0;

				if (!offset)
				{
					// Check for any other alphabetic time zone
					bool nonalpha = false;

					for (int i = 0, end = zone.size();  i < end && !nonalpha;  ++i)
					{
						nonalpha = !isalpha(zone[i]);
					}

					if (nonalpha)
					{
						return QDateTime();
					}

					// TODO: Attempt to recognize the time zone abbreviation?
					negOffset = true;    // unknown time zone: RFC 2822 treats as '-0000'
				}
			}
		}
	}

	QDate qdate(year, month + 1, day); // convert date, and check for out-of-range

	if (!qdate.isValid())
	{
		return QDateTime::currentDateTime();
	}

	QTime qTime(hour, minute, second);
	QDateTime result(qdate, qTime, Qt::UTC);

	if (offset)
	{
		result = result.addSecs(-offset);
	}

	if (!result.isValid())
	{
		return QDateTime::currentDateTime();    // invalid date/time
	}

	if (leapSecond)
	{
		// Validate a leap second time. Leap seconds are inserted after 23:59:59 UTC.
		// Convert the time to UTC and check that it is 00:00:00.
		if ((hour * 3600 + minute * 60 + 60 - offset + 86400 * 5) % 86400) // (max abs(offset) is 100 hours)
		{
			return QDateTime::currentDateTime();    // the time isn't the last second of the day
		}
	}

	return result;
}

RssParser::RssParser()
{
}

void RssParser::fillFeed(QIODevice* pData, RssFeed* pFeed, bool& ok, QString& error)
{
	QByteArray data = pData->readAll();
	QXmlStreamReader reader(data);
	bool channelFound = false;

	while (reader.readNextStartElement())
	{
		if (reader.name().compare("rss", Qt::CaseInsensitive) == 0)
		{
			while (reader.readNextStartElement())
			{
				if (reader.name().compare("channel", Qt::CaseInsensitive) == 0)
				{
					channelFound = true;
					parseRssChannel(reader, pFeed, ok, error);
				}
				else
				{
					qWarning() << "Unexpected element found " << reader.name() << ". Skiping it.";
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

	if (reader.hasError() && reader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
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
			else if (reader.name().compare("lastBuildDate", Qt::CaseInsensitive) == 0)
			{
				QString lastBuildDate = reader.readElementText();
				QString feedUrl = pFeed->url().toString();

				if (!lastBuildDate.isEmpty())
				{
					QMutexLocker locker(&m_mutex);

					if (m_lastUpdateDates.value(feedUrl, "") == lastBuildDate)
					{
						qWarning() << "The RSS feed has not changed since last time, aborting parsing.";
						return;
					}

					m_lastUpdateDates[feedUrl] = lastBuildDate;
				}
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
	boost::scoped_ptr<RssItem> pItem(new RssItem(pFeed));

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name().compare("item", Qt::CaseInsensitive) == 0)
		{
			break;
		}

		if (reader.isStartElement())
		{
			if (reader.name().compare("title", Qt::CaseInsensitive) == 0)
			{
				QString title = reader.readElementText();
				QTextDocument doc;
				doc.setHtml(title);
				title = doc.toPlainText();
				pItem->setTitle(title);
			}
			else if (reader.name().compare("pubDate", Qt::CaseInsensitive) == 0)
			{
				pItem->setPubDate(parseDate(reader.readElementText()));
			}
			else if (reader.name().compare("guid", Qt::CaseInsensitive) == 0)
			{
				pItem->setGuid(reader.readElementText());
			}
			else if (reader.name().compare("category", Qt::CaseInsensitive) == 0)
			{
				pItem->setCategory(reader.readElementText());
			}
			else if (reader.name().compare("link", Qt::CaseInsensitive) == 0)
			{
				pItem->setDescribtionLink(reader.readElementText());
			}
			else if (reader.name().compare("description", Qt::CaseInsensitive) == 0)
			{
				pItem->setDescription(reader.readElementText());
			}
			else if (reader.name().compare("enclosure", Qt::CaseInsensitive) == 0)
			{
				if (reader.attributes().value("type") == "application/x-bittorrent")
				{
					pItem->setTorrentUrl(reader.attributes().value("url").toString());
				}
			}
			else if (reader.name().compare("content", Qt::CaseInsensitive) == 0)
			{
				if (!reader.attributes().value("url").isEmpty())
				{
					pItem->setTorrentUrl(reader.attributes().value("url").toString());
				}
			}
			else if (reader.name().compare("magneturi", Qt::CaseInsensitive) == 0)
			{
				pItem->setTorrentUrl(reader.readElementText());
			}
			else if (reader.name().compare("torrent", Qt::CaseInsensitive) == 0)
			{
				parseTorrentSection(reader, pItem.get(), ok, error);
			}
			else if (reader.name().compare("contentlength", Qt::CaseInsensitive) == 0 ||
			         reader.name().compare("size", Qt::CaseInsensitive) == 0)
			{
				pItem->setSize(reader.readElementText().toULongLong());
			}
			else if (reader.name().compare("infohash", Qt::CaseInsensitive) == 0 ||
			         reader.name().compare("info_hash", Qt::CaseInsensitive) == 0 ||
			         reader.name().compare("hash", Qt::CaseInsensitive) == 0)
			{
				pItem->setInfoHash(reader.readElementText());
			}
			else if (reader.name().compare("seeds", Qt::CaseInsensitive) == 0 ||
			         reader.name().compare("seeders", Qt::CaseInsensitive) == 0)
			{
				pItem->setSeeds(reader.readElementText().toLong());
			}
			else if (reader.name().compare("peers", Qt::CaseInsensitive) == 0 ||
			         reader.name().compare("leechers", Qt::CaseInsensitive) == 0)
			{
				pItem->setPeers(reader.readElementText().toLong());
			}
		}
	}

	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}

	if (!pItem->isValid())
	{
		ok = false;
		error = qApp->translate("RssParser", "No valid rss fields found.");
		return;
	}

	if (pItem->guid().isEmpty())
	{
		if (pItem->describtionLink().isEmpty())
		{
			if (pItem->title().isEmpty())
			{
				qWarning() << "No unick id. Skiping item";
				return;
			}

			pItem->setGuid(pItem->title());
		}
		else
		{
			pItem->setGuid(pItem->describtionLink());
		}
	}

	if (pItem->torrentUrl().isEmpty() && !pItem->describtionLink().isEmpty())
	{
		pItem->setTorrentUrl(pItem->describtionLink());
	}
	else if (pItem->torrentUrl().isEmpty() && !pItem->guid().isEmpty())
	{
		QUrl tempUrl(pItem->guid());

		if (tempUrl.isValid())
		{
			pItem->setTorrentUrl(pItem->guid());
		}
	}

	QString guid = pItem->guid();

	if (pFeed->m_rssItems.contains(guid))
	{
		if (pItem->pubDate().isValid())
		{
			QDateTime newItemDate = pItem->pubDate();
			QDateTime oldItemDate = pFeed->m_rssItems[guid]->pubDate();

			if (newItemDate > oldItemDate)
			{
				boost::scoped_ptr<RssItem> pOldItem(pFeed->m_rssItems[guid]);
				pFeed->m_rssItems.remove(guid);
				pFeed->m_rssItems.insert(guid, new RssItem(*pItem.get()));
				return;
			}
		}

		qWarning() << "RssItem already exists" << pItem->guid() << pItem->title();
		return;
	}

	qWarning() << "New RssItem" << pItem->guid() << pItem->title();
	pFeed->m_rssItems.insert(guid, new RssItem(*pItem.get()));
}

void RssParser::parseTorrentSection(QXmlStreamReader& reader, RssItem* item, bool& ok, QString& error)
{
	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name().compare("torrent", Qt::CaseInsensitive) == 0)
		{
			break;
		}

		if (reader.isStartElement())
		{
			if (reader.name().compare("contentlength", Qt::CaseInsensitive) == 0)
			{
				item->setSize(reader.readElementText().toULongLong());
			}
			else if (reader.name().compare("magneturi", Qt::CaseInsensitive) == 0)
			{
				item->setMagnetUrl(reader.readElementText());
			}
			else if (reader.name().compare("infohash", Qt::CaseInsensitive) == 0)
			{
				item->setInfoHash(reader.readElementText());
			}
			else if (reader.name().compare("seeds", Qt::CaseInsensitive) == 0)
			{
				item->setSeeds(reader.readElementText().toLong());
			}
			else if (reader.name().compare("peers", Qt::CaseInsensitive) == 0)
			{
				item->setPeers(reader.readElementText().toLong());
			}
		}
	}

	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}
}

void RssParser::parseAtomChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error)
{
	QString baseURL = reader.attributes().value("xml:base").toString();

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isStartElement())
		{
			if (reader.name() == "title")
			{
				pFeed->m_title = reader.readElementText();
			}
			else if (reader.name() == "updated")
			{
				QString lastBuildDate = reader.readElementText();

				if (!lastBuildDate.isEmpty())
				{
					QMutexLocker locker(&m_mutex);
					QString feedUrl = pFeed->url().toString();

					if (m_lastUpdateDates.value(feedUrl) == lastBuildDate)
					{
						qWarning() << "The RSS feed has not changed since last time, aborting parsing.";
						return;
					}

					m_lastUpdateDates[feedUrl] = lastBuildDate;
				}
			}
			else if (reader.name() == "entry")
			{
				parseAtomArticle(reader, baseURL, pFeed, ok, error);
			}
		}
	}
}

void RssParser::parseAtomArticle(QXmlStreamReader& reader, QString baseURL, RssFeed* pFeed, bool& ok, QString& error)
{
	boost::scoped_ptr<RssItem> pItem(new RssItem(pFeed));
	bool double_content = false;

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isEndElement() && reader.name() == "entry")
		{
			break;
		}

		if (reader.isStartElement())
		{
			if (reader.name() == "title")
			{
				// Workaround for CDATA (QString cannot parse html escapes on it's own)
				QTextDocument doc;
				doc.setHtml(reader.readElementText());
				pItem->setTitle(doc.toPlainText());
			}
			else if (reader.name() == "link")
			{
				QString theLink = (reader.attributes().isEmpty() ?
				                   reader.readElementText() :
				                   reader.attributes().value("href").toString());
				// Atom feeds can have relative links, work around this and
				// take the stress of figuring article full URI from UI
				// Assemble full URI
				pItem->setDescribtionLink (baseURL.isEmpty() ?
				                           theLink :
				                           baseURL + theLink);
			}
			else if (reader.name() == "summary" || reader.name() == "content")
			{
				if (double_content)   // Duplicate content -> ignore
				{
					reader.readNext();

					while (reader.name() != "summary" && reader.name() != "content")
					{
						reader.readNext();
					}

					continue;
				}

				// Try to also parse broken articles, which don't use html '&' escapes
				// Actually works great for non-broken content too
				QString feedText = reader.readElementText(QXmlStreamReader::IncludeChildElements);

				if (!feedText.isEmpty())
				{
					pItem->setDescription(feedText);
				}

				double_content = true;
			}
			else if (reader.name() == "updated")
			{
				// ATOM uses standard compliant date, don't do fancy stuff
				QDateTime articleDate = QDateTime::fromString(reader.readElementText(), Qt::ISODate);
				pItem->setPubDate(articleDate.isValid() ? articleDate : QDateTime::currentDateTime());
			}
			else if (reader.name() == "author")
			{
				reader.readNext();

				while (reader.name() != "author")
				{
					if (reader.name() == "name")
					{
						pItem->setAuthor(reader.readElementText());
					}

					reader.readNext();
				}
			}
			else if (reader.name() == "id")
			{
				pItem->setGuid(reader.readElementText());
			}
		}
	}

	if (reader.hasError())
	{
		ok = false;
		error = reader.errorString();
		return;
	}

	if (!pItem->isValid())
	{
		ok = false;
		error = qApp->translate("RssParser", "No valid rss fields found.");
		return;
	}

	QString guid = pItem->guid();

	if (pItem->torrentUrl().isEmpty() && !pItem->describtionLink().isEmpty())
	{
		pItem->setTorrentUrl(pItem->describtionLink());
	}
	else if (pItem->torrentUrl().isEmpty() && !guid.isEmpty())
	{
		QUrl tempUrl(guid);

		if (tempUrl.isValid())
		{
			pItem->setTorrentUrl(guid);
		}
	}

	if (pFeed->m_rssItems.contains(guid))
	{
		if (pItem->pubDate().isValid())
		{
			QDateTime newItemDate = pItem->pubDate();
			QDateTime oldItemDate = pFeed->m_rssItems[guid]->pubDate();

			if (newItemDate > oldItemDate)
			{
				boost::scoped_ptr<RssItem> pOldItem(pFeed->m_rssItems[guid]);
				pFeed->m_rssItems.remove(guid);
				RssItem* pNewItem = new RssItem(*pItem.get());
				QString infoHash = pOldItem->infoHash();

				if (!infoHash.isEmpty())
				{
					pNewItem->setInfoHash(infoHash);
				}

				pFeed->m_rssItems.insert(guid, pNewItem);
				return;
			}
		}

		return;
	}

	pFeed->m_rssItems.insert(guid, new RssItem(*pItem.get()));
}


