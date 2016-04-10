#pragma once
#include <QtCore>
#include "RssBaseTreeItem.h"
class RssFeed;
class RssFeedItemTreeItem;

class RssFeedTreeItem : public RssBaseTreeItem
{
public:
	RssFeedTreeItem(RssFeed*);
	~RssFeedTreeItem();
	RssFeed* GetFeed();
	QList<RssFeedItemTreeItem*> Children();
private:
	RssFeed* m_pFeed;
	void BuildChildren();
	QList<RssFeedItemTreeItem*> m_children;
};

