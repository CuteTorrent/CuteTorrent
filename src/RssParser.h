#ifndef _RSS_PARSER_INCLUDED_
#define _RSS_PARSER_INCLUDED_

#include "RssCommon.h"
#include <QMutex>
#include <QXmlStreamReader>
#include <QHash>
#include  <QDateTime>
class RssItem;
class RssParser
{
protected:

	QMutex m_mutex;
	/*feedUrl*//*lastBuildDate*/
	QHash<QString, QString> m_lastUpdateDates;
	static boost::weak_ptr<RssParser> m_pInstance;
	void parseRssChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseRssItem(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseTorrentSection(QXmlStreamReader& reader, RssItem* item, bool& ok, QString& error);
	void parseAtomChannel(QXmlStreamReader& reader, RssFeed* pFeed, bool& ok, QString& error);
	void parseAtomArticle(QXmlStreamReader& xml, QString baseURL, RssFeed* pFeed, bool& ok, QString& error);
	QDateTime parseDate(const QString& string);
public:
	RssParser();
	static RssParserPtr getInstance();
	void fillFeed(QIODevice* pData, RssFeed* pFeed, bool& ok, QString& error);

};


#endif