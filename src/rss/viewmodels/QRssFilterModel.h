


#include <QSortFilterProxyModel>
#include "RssDownloadRule.h"
class QRssFilterModel : public QSortFilterProxyModel
{
	RssDownloadRule* m_filterRule;
public:
	QRssFilterModel(QObject* parent = 0);
	void setRuleFilter(RssDownloadRule* rule);
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

};