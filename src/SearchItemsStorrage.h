#pragma once
#include "SearchEngine.h"

class SearchItemsStorrage : public QObject
{
	Q_OBJECT
protected:
	SearchItemsStorrage();
	~SearchItemsStorrage();
private:
	static SearchItemsStorrage* m_pInstance;
	static int m_nInstanceCount;
	QList<SearchResult*> m_items;
	QList<SearchResult*> m_filteredItems;
	QString m_sEngineName;
	void filterData();
public:
	void setFilter(QString engineName);
	static SearchItemsStorrage* getInstance();
	static void freeInstance();
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