#pragma once
#include "SearchEngine.h"
#include <Singleton.h>

class SearchItemsStorrage : public QObject, public Singleton<SearchItemsStorrage>
{
	friend class Singleton<SearchItemsStorrage>;
	Q_OBJECT
protected:
	SearchItemsStorrage();

private:

	QList<SearchResult*> m_items;
public:
	~SearchItemsStorrage();
	void append(SearchResult*);
	void append(QList<SearchResult*>&);
	void remove(SearchResult*);
	bool contains(SearchResult*);
	SearchResult* operator [](int index);
	void clear();
	int length();
	signals:
	void reset();
};

