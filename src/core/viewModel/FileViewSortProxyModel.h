#pragma once
#include <QtGui>

class FileViewSortProxyModel : public QSortFilterProxyModel
{
public:
	FileViewSortProxyModel(QObject* parrent = NULL);
	~FileViewSortProxyModel();

	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
};

