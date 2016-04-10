#pragma once

#include "RssBaseTreeItem.h"
class RssFeedTreeItem;
class RssItem;

class RssFeedItemTreeItem : public RssBaseTreeItem
{
public:
	RssFeedItemTreeItem(RssFeedTreeItem* parent, RssItem* pRssitem);
	~RssFeedItemTreeItem();
	RssItem* FeedItem();
	RssFeedTreeItem* Parent();
private:
	RssItem* m_pRssitem;
	RssFeedTreeItem* m_pParent;
};

