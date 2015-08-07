/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "FileTreeModel.h"
#include <QDebug>
#include <QDir>
#include <QCheckBox>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QQueue>
#include <QDesktopServices>
#include <helpers/StaticHelpers.h>

FileTreeModel::~FileTreeModel()
{
	delete rootItem;
}

FileTreeModel::FileTreeModel(QObject* parent)
	: QAbstractItemModel(parent)
{
	QPair<QString, QVariant> rootData = qMakePair(tr("FILE"), QVariant::fromValue(tr("SIZE")));
	rootItem = new FileTreeItem(rootData);
}
QMap<QString, quint8> FileTreeModel::getFilePiorites()
{
	QMap<QString, quint8> res;
	FileTreeItem* iterator = rootItem;
	GetFilePrioritiesInternal(iterator, &res);
	return res;
}

void FileTreeModel::GetFilePrioritiesInternal(FileTreeItem* current, QMap<QString, quint8>* priorities)
{
	for(int i = 0; i < current->childCount(); i++)
	{
		FileTreeItem* curChild = current->child(i);
		qint8 prioryty = 0;

		if(curChild->Checked() == Qt::Checked)
		{
			prioryty = 1;
		}

		priorities->insert(curChild->getPath(), prioryty);
		GetFilePrioritiesInternal(curChild, priorities);
	}
}
QModelIndex FileTreeModel::index(int row, int column, const QModelIndex& parent)
const
{
	if(!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	FileTreeItem* parentItem;

	if(!parent.isValid())
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<FileTreeItem*>(parent.internalPointer());
	}

	FileTreeItem* childItem = parentItem->child(row);

	if(childItem)
	{
		return createIndex(row, column, childItem);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex FileTreeModel::parent(const QModelIndex& index) const
{
	if(!index.isValid())
	{
		return QModelIndex();
	}

	FileTreeItem* childItem = static_cast<FileTreeItem*>(index.internalPointer());
	FileTreeItem* parentItem = childItem->parent();

	if(parentItem == rootItem)
	{
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}
int FileTreeModel::rowCount(const QModelIndex& parent) const
{
	FileTreeItem* parentItem;

	if(parent.column() > 0)
	{
		return 0;
	}

	if(!parent.isValid())
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<FileTreeItem*>(parent.internalPointer());
	}

	return parentItem->childCount();
}
int FileTreeModel::columnCount(const QModelIndex& parent) const
{
	if(parent.isValid())
	{
		return static_cast<FileTreeItem*>(parent.internalPointer())->columnCount();
	}
	else
	{
		return rootItem->columnCount();
	}
}

void FileTreeModel::UpdateChildren(const QModelIndex& modelIndex, FileTreeItem* item, Qt::CheckState state)
{
	int childCount = item->childCount();

	if (childCount > 0)
	{
		for (int i = 0; i < childCount; i++)
		{
			FileTreeItem* child = item->child(i);
			child->setChecked(state);
			int subChildrenCount = child->childCount();

			if (subChildrenCount > 0)
			{
				UpdateChildren(index(i, 0, modelIndex), child, state);
			}
		}

		emit dataChanged(index(0, 0, modelIndex), index(childCount - 1, 0 , modelIndex));
	}
}

void FileTreeModel::UpdateParents(const QModelIndex& modelIndex, FileTreeItem* item)
{
	FileTreeItem* parent = item->parent();

	if (parent == NULL)
	{
		return;
	}

	bool allUnchecked = true, allChecked = true;

	for(int i = 0; i < parent->childCount(); i++)
	{
		allUnchecked = allUnchecked && (parent->child(i)->Checked() == Qt::Unchecked);
		allChecked = allChecked && (parent->child(i)->Checked() == Qt::Checked);
	}

	QModelIndex parentIndex = modelIndex.parent();

	if(allUnchecked)
	{
		parent->setChecked(Qt::Unchecked);
		emit dataChanged(parentIndex, parentIndex);
		parent = parent->parent();

		if (parent != NULL)
		{
			parent->setChecked(Qt::Checked);
			UpdateParents(parentIndex, parent);
		}
	}
	else if(allChecked)
	{
		parent->setChecked(Qt::Checked);
		emit dataChanged(parentIndex, parentIndex);
		parent = parent->parent();

		if(parent != NULL)
		{
			parent->setChecked(Qt::Checked);
			UpdateParents(parentIndex, parent);
		}
	}
	else
	{
		while (parent != rootItem && parentIndex.isValid())
		{
			parent->setChecked(Qt::PartiallyChecked);
			emit dataChanged(parentIndex, parentIndex);
			parent = parent->parent();
			parentIndex = parentIndex.parent();
		}
	}
}

bool FileTreeModel::setData(const QModelIndex& modelIndex, const QVariant& value, int role)
{
	if(!modelIndex.isValid())
	{
		return false;
	}

	FileTreeItem* item = static_cast<FileTreeItem*>(modelIndex.internalPointer());

	if(role == Qt::CheckStateRole && modelIndex.column() == 0)
	{
		Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
		item->setChecked(state);
		UpdateChildren(modelIndex, item, state);
		UpdateParents(modelIndex, item);
		return true;
	}

	return QAbstractItemModel::setData(modelIndex, value, role);
}
QVariant FileTreeModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}

	FileTreeItem* item = static_cast<FileTreeItem*>(index.internalPointer());

	if(role == Qt::CheckStateRole && index.column() == 0)
	{
		return item->Checked();
	}

	if(role == Qt::DecorationRole && index.column() == 0)
	{
		QString pathCur = item->getPath();
		QIcon icon;
		QFileInfo info(pathCur);
		QFileIconProvider iPorv;
		QString suffix = info.suffix();

		if(!suffix.isEmpty() && item->childCount() == 0)
		{
			QString fileTemplate = StaticHelpers::CombinePathes(QDesktopServices::storageLocation(QDesktopServices::TempLocation), "tempFileXXXXXX." + suffix);
			QTemporaryFile tmpfile(fileTemplate);
			tmpfile.open();
			tmpfile.close();
			QFileInfo info2(tmpfile.fileName());
			icon = iPorv.icon(info2);
			tmpfile.remove();
		}
		else
		{
			icon = iPorv.icon(QFileIconProvider::Folder);
		}

		return icon;
	}

	if(role != Qt::DisplayRole)
	{
		return QVariant();
	}

	return item->data(index.column());
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex& index) const
{
	if(!index.isValid())
	{
		return 0;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable ;
}

QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
	if (role == Qt::InitialSortOrderRole)
	{
		return Qt::DescendingOrder;
	}

	if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return rootItem->data(section);
	}

	return QVariant();
}

void FileTreeModel::addPath(QString path, uint64_t size)
{
	path = QDir::toNativeSeparators(path);
	QStringList pathparts = path.split(QDir::separator());
	FileTreeItem* iterator = rootItem, *save = rootItem;

	if(rootItem->childCount() == 0)
	{
		FileTreeItem* curitem = rootItem;

		for(int i = 0; i < pathparts.count(); i++)
		{
			curitem->appendChild(new FileTreeItem(qMakePair(pathparts.at(i), QVariant::fromValue(size)), curitem));
			curitem = curitem->child(0);
		}

		rootItem = save;
		return;
	}

	for(int i = 0; i < pathparts.count(); i++)
	{
		int foundnum = -1;

		for(int j = 0; j < iterator->childCount(); j++)
		{
			if(iterator->child(j)->data(0).toString().compare(pathparts.at(i)) == 0)
			{
				foundnum = j;
				break;
			}
		}

		if(foundnum >= 0)
		{
			iterator = iterator->child(foundnum);
			iterator->setData(1, QVariant::fromValue(iterator->data(1).toULongLong() + size));
		}
		else
		{
			iterator->appendChild(new FileTreeItem(qMakePair(pathparts.at(i), QVariant::fromValue(size)), iterator));
			iterator = iterator->child(iterator->childCount() - 1);
		}
	}

	rootItem = save;
}
