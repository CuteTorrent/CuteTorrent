#ifndef _RSS_PARSER_INCLUDED_
#define _RSS_PARSER_INCLUDED_

#include "RssCommon.h"
#include <QMutex>
#include <QXmlStreamReader>
#include <QHash>
#include <QDateTime>
#include "Singleton.h"
class RssItem;
class RssFeed;

class RssParser : public Singleton<RssParser>
{
	friend class Singleton<RssParser>;
protected:

	QMutex m_mutex;
	/*feedUrl*//*lastBuildDate*/
	QHash<QString, QString> m_lastUpdateDates;
	void parseRssChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseRssItem(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseTorrentSection(QXmlStreamReader& reader, RssItem* item, bool& ok, QString& error);
	void parseAtomChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseAtomArticle(QXmlStreamReader& xml, QString baseURL, RssFeed* pFeed, bool& ok, QString& error);
	QDateTime parseDate(const QString& string);
public:
	RssParser();
	void fillFeed(QIODevice* pData, RssFeed* pFeed, bool& ok, QString& error);
};


#endif

