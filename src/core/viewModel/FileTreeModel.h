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
#ifndef _FILETREEMODEL_H
#define _FILETREEMODEL_H
#include "TreeItem.h"
#include  <QAbstractItemModel>
class FileTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	FileTreeModel(QObject* parent = NULL);
	~FileTreeModel();
	bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
	QVariant data(const QModelIndex& index, int role) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
	                    int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
	                  const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	void addPath(QString path, QString size);
	QMap<QString, quint8> getFilePiorites();
	QStringList getUnicPathes();
	void getUnicPathes(FileTreeItem*, QList<FileTreeItem*>&);
private:
	void GetFilePrioritiesInternal(FileTreeItem* current, QMap<QString, quint8>* priorities);

	FileTreeItem* rootItem;
};
#endif

