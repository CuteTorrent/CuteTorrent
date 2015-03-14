#pragma once
#include <RssFeedItem.h>
#include "RssBaseTreeItem.h"
using namespace libtorrent;
class RssFeedTreeItem;
class RssFeedItemTreeItem : public RssBaseTreeItem
{
public:
	RssFeedItemTreeItem(RssFeedTreeItem* parent, RssFeedItem pFeedItem);
	~RssFeedItemTreeItem();
	RssFeedItem FeedItem();
	RssFeedTreeItem* Parent();
private:
	RssFeedItem m_pFeedItem;
	RssFeedTreeItem* m_pParent;
};

