#include "SortButton.h"
#include <QAbstractItemModel>
#include <QMenu>
#include <QSignalMapper>
#include <StyleEngene.h>

SortButton::SortButton(QWidget* parent)
	: m_pActionsMenu(new QMenu(this))
	, m_pSignalMapper(new QSignalMapper(this))
{
	setPopupMode(MenuButtonPopup);
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(this, SIGNAL(triggered(QAction*)),
		this, SLOT(setDefaultAction(QAction*)));
	connect(m_pSignalMapper, SIGNAL(mapped(int)), SLOT(setSortRole(int)));
	setMenu(m_pActionsMenu);
}

void SortButton::setModel(QAbstractItemModel* pModel)
{
	for (int i = 0; i < m_pActionsMenu->actions().size(); i++)
	{
		m_pSignalMapper->removeMappings(m_pActionsMenu->actions()[i]);
	}
	m_pActionsMenu->clear();
	
	int rowCount = pModel->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		QModelIndex index = pModel->index(i, 0);
		QAction* action = m_pActionsMenu->addAction(pModel->data(index).toString());
		connect(action, SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(action, pModel->data(index, Qt::UserRole).toInt());
	}
}

void SortButton::setSortRole(int newSortRole)
{
	m_sortRole = newSortRole;
	QAction* action = qobject_cast<QAction*>(m_pSignalMapper->mapping(m_sortRole));
	if (action != nullptr && defaultAction() != action)
	{
		setDefaultAction(action);
	}
	emit sortRoleChanged(m_sortRole);
}

int SortButton::sortRole()
{
	return m_sortRole;
}

void SortButton::setSortOrder(Qt::SortOrder newSortOrder)
{
	m_sortOrder = newSortOrder;
	emit sortOrderChanged(m_sortOrder);
	if (defaultAction() != nullptr)
	{
		defaultAction()->setIcon(StyleEngene::getInstance()->getIcon(m_sortOrder == Qt::AscendingOrder ? "sorter-asc" : "sorter-desc"));
	}
}

void SortButton::setDefaultAction(QAction* action)
{

	
	if (defaultAction() != nullptr)
	{
		defaultAction()->setIcon(QIcon());
	}
	QToolButton::setDefaultAction(action);
	setSortOrder(m_sortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
	
}

Qt::SortOrder SortButton::sortOrder()
{
	return m_sortOrder;
}
