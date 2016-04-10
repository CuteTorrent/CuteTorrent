#include "RssItem.h"

RssItem::RssItem(const RssItem& other): QObject(),
                                        m_title(other.m_title),
                                        m_description(other.m_description),
                                        m_category(other.m_category),
                                        m_descriptionLink(other.m_descriptionLink),
                                        m_torrentUrl(other.m_torrentUrl),
                                        m_magnetUrl(other.m_magnetUrl),
                                        m_guid(other.m_guid),
                                        m_infoHash(other.m_infoHash),
                                        m_downloadingTorrent(other.m_downloadingTorrent),
                                        m_author(other.m_author),
                                        m_pubDate(other.m_pubDate),
                                        m_pRssFeed(other.m_pRssFeed),
                                        m_size(other.m_size),
                                        m_peers(other.m_peers),
                                        m_seeds(other.m_seeds),
                                        m_unread(other.m_unread)
{
}

RssItem::RssItem(RssFeed* parent) : QObject(), m_pRssFeed(parent), m_size(0), m_peers(0), m_seeds(0), m_unread(true)
{
}

RssFeed* RssItem::rssFeed() const
{
	return m_pRssFeed;
}

QString RssItem::title() const
{
	return m_title;
}

void RssItem::setTitle(const QString& value)
{
	m_title = value;
}

QString RssItem::description() const
{
	return m_description;
}

void RssItem::setDescription(const QString& value)
{
	m_description = value;
}

QString RssItem::category() const
{
	return m_category;
}

void RssItem::setCategory(const QString& value)
{
	m_category = value;
}

QString RssItem::describtionLink() const
{
	return m_descriptionLink;
}

void RssItem::setDescribtionLink(const QString& value)
{
	m_descriptionLink = value;
}

QString RssItem::torrentUrl() const
{
	return m_torrentUrl;
}

void RssItem::setTorrentUrl(const QString& value)
{
	m_torrentUrl = value;
}

QString RssItem::magnetUrl() const
{
	return m_magnetUrl;
}

void RssItem::setMagnetUrl(const QString& value)
{
	m_magnetUrl = value;
}

QString RssItem::guid() const
{
	return m_guid;
}

void RssItem::setGuid(const QString& value)
{
	m_guid = value;
}

QString RssItem::infoHash() const
{
	return m_infoHash;
}

void RssItem::setInfoHash(const QString& value)
{
	m_infoHash = value;
}

QString RssItem::downloadingTorrent() const
{
	return m_downloadingTorrent;
}

void RssItem::setDownloadingTorrent(const QString& value)
{
	m_downloadingTorrent = value;
}

QDateTime RssItem::pubDate() const
{
	return m_pubDate;
}

void RssItem::setPubDate(const QDateTime& value)
{
	m_pubDate = value;
}

quint64 RssItem::size() const
{
	return m_size;
}

void RssItem::setSize(quint64 value)
{
	m_size = value;
}

quint32 RssItem::peers() const
{
	return m_peers;
}

void RssItem::setPeers(quint32 value)
{
	m_peers = value;
}

quint32 RssItem::seeds() const
{
	return m_seeds;
}

void RssItem::setSeeds(quint32 value)
{
	m_seeds = value;
}

bool RssItem::unread() const
{
	return m_unread;
}

void RssItem::setUread(bool value)
{
	m_unread = value;
}

QString RssItem::author() const
{
	return m_author;
}

void RssItem::setAuthor(const QString& value)
{
	m_author = value;
}

bool RssItem::isValid()
{
	return !m_title.isEmpty();
}

RssItem* RssItem::clone()
{
	RssItem* pItem = new RssItem(*this);
	return pItem;
}

QDataStream& operator<<(QDataStream& out, const RssItem* any)
{
	out << any->m_title;
	out << any->m_author;
	out << any->m_category;
	out << any->m_descriptionLink;
	out << any->m_description;
	out << any->m_downloadingTorrent;
	out << any->m_torrentUrl;
	out << any->m_magnetUrl;
	out << any->m_guid;
	out << any->m_infoHash;
	out << any->m_peers;
	out << any->m_seeds;
	out << any->m_size;
	out << any->m_pubDate;
	out << any->m_unread;
	return out;
}

QDataStream& operator>>(QDataStream& out, RssItem* any)
{
	out >> any->m_title;
	out >> any->m_author;
	out >> any->m_category;
	out >> any->m_descriptionLink;
	out >> any->m_description;
	out >> any->m_downloadingTorrent;
	out >> any->m_torrentUrl;
	out >> any->m_magnetUrl;
	out >> any->m_guid;
	out >> any->m_infoHash;
	out >> any->m_peers;
	out >> any->m_seeds;
	out >> any->m_size;
	out >> any->m_pubDate;
	out >> any->m_unread;
	return out;
}

