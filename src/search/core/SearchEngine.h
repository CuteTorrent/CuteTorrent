
#pragma once
#include <QtCore>
#include "KickassSearchProvider.h"
#include "SearchItemsStorrage.h"
#include "SearchCommon.h"

class SearchEngine : public QObject
{
	Q_OBJECT
public:
    static SearchEnginePtr getInstance();
    ~SearchEngine();
	void DoSerach(QString& token, ISerachProvider::SearchCategories category, int page);
	SearchItemsStorrage* GetResults();
	QList<ISerachProvider*> GetSearchProviders();
signals:
	void GotResults();
private:
    SearchEngine();
	SearchItemsStorrage* m_result;
	QList<ISerachProvider*> m_pSearchProviders;
    static boost::weak_ptr<SearchEngine> m_pInstance;
private slots:
	void OnSearchReady(QList<SearchResult*> result);
};
