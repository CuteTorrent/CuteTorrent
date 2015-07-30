#include "SearchItemsStorrage.h"
#include "SearchResult.h"
SearchItemsStorrage* SearchItemsStorrage::m_pInstance = NULL;

int SearchItemsStorrage::m_nInstanceCount = 0;

SearchItemsStorrage::SearchItemsStorrage() : m_sEngineName("")
{
	m_items.clear();
	m_filteredItems.clear();
}

SearchItemsStorrage::~SearchItemsStorrage()
{
	qDeleteAll(m_items);
}

void SearchItemsStorrage::filterData()
{
	if(m_sEngineName.isEmpty())
	{
		m_filteredItems = m_items;
	}
	else
	{
		m_filteredItems.clear();
		int nCount = m_items.length();

		for(int i = 0; i < nCount; i++)
		{
			SearchResult* item = m_items.at(i);

			if(item->Engine().compare(m_sEngineName, Qt::CaseInsensitive) == 0)
			{
				m_filteredItems.append(item);
			}
		}
	}

	emit reset();
}

void SearchItemsStorrage::setFilter(QString engineName)
{
	m_sEngineName = engineName;
	filterData();
}

SearchItemsStorrage* SearchItemsStorrage::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new SearchItemsStorrage();
	}

	m_nInstanceCount++;
	return m_pInstance;
}

void SearchItemsStorrage::freeInstance()
{
	m_nInstanceCount--;

	if(m_nInstanceCount == 0)
	{
		delete m_pInstance;
	}
}

void SearchItemsStorrage::append(SearchResult* item)
{
	if(m_items.contains(item))
	{
		return;
	}

	m_items.append(item);

	if(m_sEngineName.isEmpty())
	{
		m_filteredItems.append(item);
	}
	else
	{
		if(item->Engine().compare(m_sEngineName, Qt::CaseInsensitive))
		{
			m_filteredItems.append(item);
		}
	}
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

	int iterIndex = m_filteredItems.indexOf(item);

	if(iterIndex >= 0)
	{
		m_filteredItems.removeAt(iterIndex);
	}

	m_items.removeAt(itemIndex);
}

bool SearchItemsStorrage::contains(SearchResult* item)
{
	int itemIndex = m_items.indexOf(item);

	if(itemIndex < 0)
	{
		return false;
	}

	int iterIndex = m_filteredItems.indexOf(item);

	if(iterIndex >= 0)
	{
		return true;
	}

	return false;
}

SearchResult* SearchItemsStorrage::operator[](int index)
{
	SearchResult* pItem = m_filteredItems.at(index);
	return pItem;
}

void SearchItemsStorrage::clear()
{
	qDeleteAll(m_items);
	m_items.clear();
	m_filteredItems.clear();
}

int SearchItemsStorrage::length()
{
	return m_filteredItems.length();
}
