
#pragma once
#include <QtCore>
#include "KickassSearchProvider.h"
#include "SearchItemsStorrage.h"
class SearchItemsStorrage;
class SearchEngine : public QObject
{
	Q_OBJECT
public:
	SearchEngine();
	~SearchEngine();
	void DoSerach(QString& token, ISerachProvider::SearchCategories category, int page);
	SearchItemsStorrage* GetResults();
	QList<ISerachProvider*> GetSearchProviders();
signals:
	void GotResults();
private:
	SearchItemsStorrage* m_result;
	QList<ISerachProvider*> m_pSearchProviders;

private slots:
	void OnSearchReady(QList<SearchResult*> result);
};