#include "FileTreeSortProxyModel.h"
#include <TreeItem.h>

FileTreeSortProxyModel::FileTreeSortProxyModel(QObject* parrent) : QSortFilterProxyModel(parrent)
{
}

bool FileTreeSortProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	FileTreeItem* leftItem = static_cast<FileTreeItem*>(left.internalPointer());
	FileTreeItem* rightItem = static_cast<FileTreeItem*>(right.internalPointer());
	bool leftIsFolder = leftItem->childCount() > 0;
	bool rightIsFolder = rightItem->childCount() > 0;

	if (leftIsFolder != rightIsFolder)
	{
		return leftIsFolder;
	}

	int sortCol = sortColumn();

	switch (sortCol)
	{
		case 0:
		{
			return leftItem->data(sortCol).toString().compare(rightItem->data(sortCol).toString(), Qt::CaseInsensitive) < 0;
		}

		case 1:
		{
			return leftItem->data(sortCol).toULongLong() < rightItem->data(sortCol).toULongLong();
		}

		default:
		{
			return true;
		}
	}
}