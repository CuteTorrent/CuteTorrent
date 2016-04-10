#pragma once
#include <QSortFilterProxyModel>

class FileTreeSortProxyModel : public QSortFilterProxyModel
{
public:
	FileTreeSortProxyModel(QObject* parrent = NULL);

	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

