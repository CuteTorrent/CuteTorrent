#include "RssFeedItem.h"

RssFeedItem::RssFeedItem(feed_item feed)
{
	m_feedItem = feed;
}

QString RssFeedItem::Title()
{
	return QString::fromStdString(m_feedItem.title);
}

QString RssFeedItem::Description()
{
	return QString::fromStdString(m_feedItem.description);
}

QString RssFeedItem::URL()
{
	return QString::fromStdString(m_feedItem.url);
}

QString RssFeedItem::Coment()
{
	return QString::fromStdString(m_feedItem.comment);
}

quint64 RssFeedItem::Size()
{
	return m_feedItem.size;
}

QString RssFeedItem::Category()
{
	return QString::fromStdString(m_feedItem.category);
}

QString RssFeedItem::InfoHash()
{
	return QString::fromStdString(to_hex(m_feedItem.info_hash.to_string()));
}

