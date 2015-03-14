#include "RssFeedTreeItem.h"
#include "RssFeed.h"
#include "RssFeedItemTreeItem.h"

RssFeedTreeItem::RssFeedTreeItem(RssFeed* pFeed)
{
	m_type = RssBaseTreeItem::Feed;
	m_pFeed = pFeed;
}


RssFeedTreeItem::~RssFeedTreeItem()
{
	qDeleteAll(m_children);
	m_children.clear();
}

RssFeed* RssFeedTreeItem::GetFeed()
{
	return m_pFeed;
}

void RssFeedTreeItem::BuildChildren()
{
	if (!m_children.isEmpty())
	{
		qDeleteAll(m_children);
		m_children.clear();
	}
	
	RssFeedItemList feedItems = m_pFeed->GetFeedItems();
	for each (RssFeedItem item in feedItems)
	{
		m_children.append(new RssFeedItemTreeItem(this, item));
	}
}

QList<RssFeedItemTreeItem*> RssFeedTreeItem::Children()
{
	if (!m_children.isEmpty())
	{
		BuildChildren();
	}
	return m_children;
}
