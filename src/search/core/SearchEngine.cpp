#include "SearchEngine.h"
#include <QMessageBox>
#include "ExtratorrentSearchProvider.h"
#include "RutorSearchProvider.h"
SearchEngine::SearchEngine()
{
	m_pSearchProviders.append(new KickassSearchProvider());
	m_pSearchProviders.append(new ExtratorrentSearchProvider());
	m_pSearchProviders.append(new RutorSearchProvider());

    for (int i = 0; i< m_pSearchProviders.size(); i++)
	{
        ISerachProvider* searchProvider = m_pSearchProviders[i];
		QObject::connect(dynamic_cast<QObject*>(searchProvider), SIGNAL(SearchReady(QList<SearchResult*>)), this, SLOT(OnSearchReady(QList<SearchResult*>)));
	}

	m_result = SearchItemsStorrage::getInstance();
}

SearchEngine::~SearchEngine()
{
	SearchItemsStorrage::freeInstance();
	qDeleteAll(m_pSearchProviders);
}

void SearchEngine::DoSerach(QString& token, ISerachProvider::SearchCategories category, int page)
{
	m_result->clear();

    for (int i = 0; i< m_pSearchProviders.size(); i++)
    {
        ISerachProvider* searchProvider = m_pSearchProviders[i];
		searchProvider->PeformSearch(token, category, page);
	}
}

void SearchEngine::OnSearchReady(QList<SearchResult*> result)
{
	m_result->append(result);
	emit GotResults();
}

SearchItemsStorrage* SearchEngine::GetResults()
{
	return m_result;
}

QList<ISerachProvider*> SearchEngine::GetSearchProviders()
{
	return m_pSearchProviders;
}
