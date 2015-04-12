#include "RssFeedItemTreeItem.h"
#include "RssFeedTreeItem.h"
RssFeedItemTreeItem::RssFeedItemTreeItem(RssFeedTreeItem* parent, RssItem* pRssitem) : RssBaseTreeItem(RssBaseTreeItem::FeedItem), m_pRssitem(pRssitem), m_pParent(parent)
{
}


RssFeedItemTreeItem::~RssFeedItemTreeItem()
{
}

RssItem* RssFeedItemTreeItem::FeedItem()
{
	return m_pRssitem;
}

RssFeedTreeItem* RssFeedItemTreeItem::Parent()
{
	return m_pParent;
}
