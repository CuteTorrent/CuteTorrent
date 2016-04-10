#include "FileViewTreeItem.h"

FileViewTreeItem::FileViewTreeItem()
{
	m_type = NONE;
	m_pParrent = NULL;
	m_name = "";
	m_pChildren = QVector<FileViewTreeItem*>();
}

FileViewTreeItem::FileViewTreeItem(const file_entry& fe, ItemType type, const QString& name, FileViewTreeItem* parrent)
{
	m_fileEntery = fe;
	m_type = type;
	m_name = name;
	m_pParrent = parrent;
}

const QString& FileViewTreeItem::GetName()
{
	return m_name;
}

const file_entry FileViewTreeItem::GetFileEntery()
{
	return m_fileEntery;
}

FileViewTreeItem::ItemType FileViewTreeItem::GetType()
{
	return m_type;
}

void FileViewTreeItem::AddChild(FileViewTreeItem* child)
{
	m_pChildren.append(child);
}

int FileViewTreeItem::GetChildrenCount()
{
	return m_pChildren.size();
}

FileViewTreeItem* FileViewTreeItem::GetNthChild(int n)
{
	if (m_pChildren.size() > n)
	{
		return m_pChildren[n];
	}

	return NULL;
}

FileViewTreeItem* FileViewTreeItem::GetParent()
{
	return m_pParrent;
}

FileViewTreeItem::~FileViewTreeItem()
{
	qDeleteAll(m_pChildren);
}

