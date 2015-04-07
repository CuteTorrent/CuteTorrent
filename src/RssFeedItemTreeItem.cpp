#include "RssFeedItemTreeItem.h"
#include "RssFeedTreeItem.h"
RssFeedItemTreeItem::RssFeedItemTreeItem(RssFeedTreeItem* parent, QString guid) : RssBaseTreeItem(RssBaseTreeItem::FeedItem), m_guid(guid), m_pParent(parent)
{
}


RssFeedItemTreeItem::~RssFeedItemTreeItem()
{
}

RssItem RssFeedItemTreeItem::FeedItem()
{
	return m_pParent->GetFeed()->GetFeedItem(m_guid);
}

RssFeedTreeItem* RssFeedItemTreeItem::Parent()
{
	return m_pParent;
}
