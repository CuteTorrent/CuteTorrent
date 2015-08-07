#include "QSearchFilterModel.h"
#include "QSearchDisplayModel.h"
#include "SearchResult.h"
void QSearchFilterModel::setSearchFilter(QString engine)
{
	qDebug() << "Search Filter" << engine;
	m_engineFilter = engine;
	invalidateFilter();
}

bool QSearchFilterModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	SearchResult* searchResult = sourceModel()->index(source_row, 0, source_parent).data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();

	if (searchResult != NULL)
	{
		if (m_engineFilter.isEmpty())
		{
			return true;
		}

		return searchResult->Engine() == m_engineFilter;
	}

	return false;
}