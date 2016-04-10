#include "RssBaseTreeItem.h"


RssBaseTreeItem::RssBaseTreeItem(ItemType type)
{
	m_type = type;
}


RssBaseTreeItem::~RssBaseTreeItem()
{
}

RssBaseTreeItem::ItemType RssBaseTreeItem::GetType()
{
	return m_type;
}

