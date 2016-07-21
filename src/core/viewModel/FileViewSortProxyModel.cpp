#include "FileViewSortProxyModel.h"
#include "FileViewTreeItem.h"

FileViewSortProxyModel::FileViewSortProxyModel(QObject* parrent /* = NULL */) : QSortFilterProxyModel(parrent)
{
}


FileViewSortProxyModel::~FileViewSortProxyModel()
{
}

bool FileViewSortProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	FileViewTreeItem* rightItem = static_cast<FileViewTreeItem*>(right.internalPointer());
	FileViewTreeItem* lefttItem = static_cast<FileViewTreeItem*>(left.internalPointer());

	if (rightItem->GetType() != lefttItem->GetType())
	{
		return lefttItem->GetType() == FileViewTreeItem::FOLDER;
	}

	int sortCol = sortColumn();

	switch (sortCol)
	{
		case 0:
		{
			return lefttItem->GetName().compare(rightItem->GetName(), Qt::CaseInsensitive) < 0;
		}

		case 1:
		{
			int64_t szLeft = sourceModel()->data(left).toLongLong();
			int64_t szRight = sourceModel()->data(right).toLongLong();
			return szLeft < szRight;
		}

		case 2:
		{
			float leftReady = sourceModel()->data(left).toFloat();
			float rightReady = sourceModel()->data(right).toFloat();
			return leftReady < rightReady;
		}

		case 3:
		{
			int leftPriority = sourceModel()->data(left).toInt();
			int rightPriority = sourceModel()->data(right).toInt();
			return leftPriority < rightPriority;
		}
	}

	return true;
}

