#include "QRssFilterModel.h"
#include "RssBaseTreeItem.h"
#include "RssFeedTreeItem.h"
#include "RssItem.h"
#include "RssFeedItemTreeItem.h"
#include "QRssDisplayModel.h"
#include "RssFeed.h"
#include <QDebug>
QRssFilterModel::QRssFilterModel(QObject* parent) : QSortFilterProxyModel(parent), m_filterRule(NULL)
{
	//setDynamicSortFilter(true);
}


bool QRssFilterModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

	if (index.isValid() && m_filterRule != NULL && m_filterRule->RuleType() != RssDownloadRule::SELECT_FILE_RULE)
	{
		QVariant data = index.data(QRssDisplayModel::RssFeedRole);

		if (data.isValid())
		{
			RssFeed* pFeed = data.value<RssFeed*>();
			bool res = m_filterRule->Match(pFeed);
			return res;
		}

		data = index.data(QRssDisplayModel::RssItemRole);

		if (data.isValid())
		{
			RssItem* item = data.value<RssItem*>();
			bool res = m_filterRule->Match(item);
			return res;
		}
	}

	return false;
}

void QRssFilterModel::setRuleFilter(RssDownloadRule* rule)
{
	m_filterRule = rule;
	invalidateFilter();
}

