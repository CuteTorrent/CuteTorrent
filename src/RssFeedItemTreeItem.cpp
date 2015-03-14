#include "RssFeedItemTreeItem.h"


RssFeedItemTreeItem::RssFeedItemTreeItem(RssFeedTreeItem* parent, RssFeedItem pFeedItem) : m_pFeedItem(RssFeedItem(feed_item()))
{
	m_pFeedItem = pFeedItem;
	m_type = RssBaseTreeItem::FeedItem;
	m_pParent = parent;
}


RssFeedItemTreeItem::~RssFeedItemTreeItem()
{
}

RssFeedItem RssFeedItemTreeItem::FeedItem()
{
	return m_pFeedItem;
}

RssFeedTreeItem* RssFeedItemTreeItem::Parent()
{
	return m_pParent;
}
