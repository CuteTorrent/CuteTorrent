#ifndef QSearchFilterModel_INCLUDED
#define QSearchFilterModel_INCLUDED
#include <QSortFilterProxyModel>

class QSearchFilterModel : public QSortFilterProxyModel
{
	QString m_engineFilter;
public:
	void setSearchFilter(QString engine);
protected:
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
};

#endif