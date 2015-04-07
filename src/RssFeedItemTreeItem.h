#pragma once

#include "RssBaseTreeItem.h"
#include "RssFeed.h"
class RssFeedTreeItem;
class RssFeedItemTreeItem : public RssBaseTreeItem
{
public:
	RssFeedItemTreeItem(RssFeedTreeItem* parent, QString guid);
	~RssFeedItemTreeItem();
	RssItem FeedItem();
	RssFeedTreeItem* Parent();
private:
	QString m_guid;
	RssFeedTreeItem* m_pParent;
};

