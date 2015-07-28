#ifndef PerrSortModel_INCLUDED
#define PerrSortModel_INCLUDED
#include <QSortFilterProxyModel>

class PeerSortModel : public QSortFilterProxyModel
{
protected:
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
public:
	PeerSortModel(QObject* parent = NULL);
};

#endif