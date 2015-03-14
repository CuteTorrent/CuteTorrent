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
#ifndef _FILETREEITEM_H
#define _FILETREEITEM_H
#include <QPair>
#include <QVariant>

class FileTreeItem
{
public:
	FileTreeItem(const QPair<QString, QString>& data, FileTreeItem* parent = 0);
	~FileTreeItem();

	void appendChild(FileTreeItem* child);

	FileTreeItem* child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	FileTreeItem* parent();
	Qt::CheckState Checked();
	QString getPath();
	void setChecked(Qt::CheckState checked);
private:
	Qt::CheckState checkedState;
	QList<FileTreeItem*> childItems;
	QPair<QString, QString> itemData;
	FileTreeItem* parentItem;
};

#endif