#ifndef FilterTreeItem_INCLUDED
#define FilterTreeItem_INCLUDED
#include <QString>
#include <QIcon>
#include <defs.h>

class FilterTreeItem
{
	QString m_text;
	QIcon m_icon;
	FilterType m_filterType;
	QVariant m_filter;
	FilterTreeItem* m_parent;
	QList<FilterTreeItem*> m_children;
	int m_itemsCount;
public:

	FilterTreeItem(const QString& text, const QIcon& icon, FilterType filterType, const QVariant& filter, FilterTreeItem* parent = NULL)
		: m_text(text),
		  m_icon(icon),
		  m_filterType(filterType),
		  m_filter(filter),
		  m_parent(parent),
		  m_itemsCount(-1)
	{
	}

	void setIcon(const QIcon& icon)
	{
		m_icon = icon;
	}

	~FilterTreeItem()
	{
		ClearChildren();
	}

	FilterType FilterType() const
	{
		return m_filterType;
	}

	QVariant Filter() const
	{
		return m_filter;
	}

	FilterTreeItem* Parent() const
	{
		return m_parent;
	}

	QString DisplayText()
	{
		if (m_itemsCount == -1)
		{
			return m_text;
		}

		return QString("%1 (%2)").arg(m_text, QString::number(m_itemsCount));
	}

	void setItemsCount(int itemsCount)
	{
		m_itemsCount = itemsCount;
	}

	int ItemsCount() const
	{
		return m_itemsCount;
	}

	FilterTreeItem* Child(int index) const
	{
		return m_children[index];
	}

	void ClearChildren()
	{
		qDeleteAll(m_children);
		m_children.clear();
	}

	void setChildren(const QList<FilterTreeItem*>& filterTreeItems)
	{
		m_children = filterTreeItems;
	}

	QList<FilterTreeItem*> Children() const
	{
		return m_children;
	}

	void AddChlld(FilterTreeItem* pChild)
	{
		m_children.append(pChild);
	}

	QString Text() const
	{
		return m_text;
	}

	void setText(const QString& text)
	{
		m_text = text;
	}

	QIcon Icon() const
	{
		return m_icon;
	}

};

#endif