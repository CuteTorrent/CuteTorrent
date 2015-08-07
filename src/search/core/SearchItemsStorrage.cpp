#include "SearchItemsStorrage.h"
#include "SearchResult.h"


SearchItemsStorrage::SearchItemsStorrage()
{
	m_items.clear();
}

SearchItemsStorrage::~SearchItemsStorrage()
{
	qDeleteAll(m_items);
}


void SearchItemsStorrage::append(SearchResult* item)
{
	if(m_items.contains(item))
	{
		return;
	}

	m_items.append(item);
}

void SearchItemsStorrage::append(QList<SearchResult*>& items)
{
	int nCount = items.length();

	for(int i = 0; i < nCount; i++)
	{
		append(items.at(i));
	}
}

void SearchItemsStorrage::remove(SearchResult* item)
{
	int itemIndex = m_items.indexOf(item);

	if(itemIndex < 0)
	{
		return;
	}
}

bool SearchItemsStorrage::contains(SearchResult* item)
{
	int itemIndex = m_items.indexOf(item);

	if(itemIndex < 0)
	{
		return false;
	}

	return false;
}

SearchResult* SearchItemsStorrage::operator[](int index)
{
	SearchResult* pItem = m_items.at(index);
	return pItem;
}

void SearchItemsStorrage::clear()
{
	qDeleteAll(m_items);
	m_items.clear();
}

int SearchItemsStorrage::length()
{
	return m_items.length();
}
