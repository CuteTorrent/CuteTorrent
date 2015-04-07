#include "RssFeed.h"
#include "RssManager.h"
#include "RssParser.h"
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include "StaticHelpers.h"
#include <QTimer>
#include <QDataStream>
RssFeed::RssFeed(QUrl url, QUuid uid) : m_url(url), m_uid(uid), m_ttl(0)
{
	m_pNetManager = new QNetworkAccessManager(this);
	m_pDiskCache = StaticHelpers::GetGLobalWebCache();
	m_pNetManager->setCache(m_pDiskCache);
	m_pUpdateTimer = new QTimer(this);
	m_elapsedTime.start();
	connect(m_pNetManager, SIGNAL(finished(QNetworkReply *)), SLOT(resourceLoaded(QNetworkReply*)));
	connect(m_pUpdateTimer, SIGNAL(timout()), SLOT(Update()));
	if (!url.isEmpty())
	{
		Update();
	}
}

QUrl RssFeed::url()
{
	return m_url;
}

QUuid RssFeed::uid()
{
	return m_uid;
}

void RssFeed::resourceLoaded(QNetworkReply* pReply)
{
	QVariant fromCache = pReply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
	qDebug() << "feed from cache?" << fromCache.toBool();
	RssParserPtr parser = RssParser::getInstance();
	bool ok;
	QString error;
	parser->fillFeed(pReply, this, ok, error);
	if (!ok)
	{
		m_errorString = error;
	}
	else
	{
		m_errorString = "";
	}
	m_isUpdating = false;
	if (m_pUpdateTimer->isActive())
	{
		m_pUpdateTimer->stop();
	}
	
	if (m_ttl == 0)
	{
		m_ttl = 30;
	}
	qDebug() << "TTl value " << m_ttl << " minutes";
	m_pUpdateTimer->setInterval(m_ttl * 60l * 1000l);
	m_pUpdateTimer->start();
	m_elapsedTime.start();
	emit FeedChanged(m_uid);
}


QString RssFeed::title()
{
	return m_title;
}

QList<RssItem> RssFeed::GetFeedItems()
{
	return m_rssItems.values();
}

int RssFeed::ttl()
{
	return m_ttl;
}

QString RssFeed::description() const
{
	return m_description;
}

void RssFeed::Update()
{
	m_elapsedTime.restart();
	QNetworkRequest request(m_url);
	request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork);
	m_pNetManager->get(request);
	m_isUpdating = true;
}

int RssFeed::next_update()
{
	return (m_pUpdateTimer->interval() - m_elapsedTime.elapsed()) / 1000;
}

void RssFeed::setItemUnread(bool unreadValue, QString guid)
{
	if (m_rssItems.contains(guid))
	{
		m_rssItems[guid]["unread"] = unreadValue;
	}
}

RssItem RssFeed::GetFeedItem(QString guid)
{
	if (m_rssItems.contains(guid))
	{
		return m_rssItems[guid];
	}
	RssItem invalidItem;
	return invalidItem;
}


QDataStream& operator<<(QDataStream& stream, const RssFeed& any)
{
	stream << any.m_uid;
	stream << any.m_url;
	stream << any.m_title;
	stream << any.m_description;
	stream << any.m_link;
	stream << any.m_rssItems;
	return stream;
}

QDataStream& operator>>(QDataStream& stream, RssFeed& any)
{
	stream >> any.m_uid;
	stream >> any.m_url;
	stream >> any.m_title;
	stream >> any.m_description;
	stream >> any.m_link;
	stream >> any.m_rssItems;
	return stream;
}
