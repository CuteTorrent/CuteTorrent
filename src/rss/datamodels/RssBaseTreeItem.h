#pragma once
class RssBaseTreeItem
{
public:
	enum ItemType {Feed, FeedItem};
	RssBaseTreeItem(ItemType type);
	~RssBaseTreeItem();
	ItemType GetType();
private:
protected:
	ItemType m_type;
};

