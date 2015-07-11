#include "RssFeedTreeItem.h"
#include "RssFeed.h"
#include "RssFeedItemTreeItem.h"

RssFeedTreeItem::RssFeedTreeItem(RssFeed* pFeed) : RssBaseTreeItem(RssBaseTreeItem::Feed), m_pFeed(pFeed)
{
	m_children.clear();
	BuildChildren();
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

	QList<RssItem*> feedItems = m_pFeed->GetFeedItems();

	for (int i = 0; i < feedItems.size(); i++)
	{
		m_children.append(new RssFeedItemTreeItem(this, feedItems[i]));
	}
}

QList<RssFeedItemTreeItem*> RssFeedTreeItem::Children()
{
	return m_children;
}
