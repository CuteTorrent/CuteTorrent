#pragma once
#include "TorrentManager.h"
#include <QVector>

class FileViewTreeItem
{
	friend class FileViewModel;
public:
	enum ItemType
	{
		NONE,
		FOLDER,
		FILE
	};

	FileViewTreeItem();
	FileViewTreeItem(const file_entry& fe, ItemType type, const QString& name, FileViewTreeItem* parrent = NULL);
	~FileViewTreeItem();
	const QString& GetName();
	const file_entry GetFileEntery();
	ItemType GetType();
	void AddChild(FileViewTreeItem* child);
	FileViewTreeItem* GetNthChild(int n);
	FileViewTreeItem* GetParent();
	int GetChildrenCount();
private:
	file_entry m_fileEntery;
	ItemType m_type;
	QString m_name;
	QVector<FileViewTreeItem*> m_pChildren;
	FileViewTreeItem* m_pParrent;
};

