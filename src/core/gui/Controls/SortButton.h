#ifndef SORT_BUTTON_H_INCLUDED
#define SORT_BUTTON_H_INCLUDED
#include <QToolButton>
class QAbstractItemModel;
class QMenu;
class QSignalMapper;
class SortButton : public QToolButton
{
	Q_OBJECT
	Q_PROPERTY(int sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged)
	Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
public:
	explicit SortButton(QWidget *parent = 0);
	void setModel(QAbstractItemModel* pModel);
	int sortRole();
	Qt::SortOrder sortOrder();
public slots:
	void setSortRole(int newSortRole);
	void setSortOrder(Qt::SortOrder newSortOrder);
	void setDefaultAction(QAction*);
private:
	int m_sortRole;
	Qt::SortOrder m_sortOrder;
	QMenu* m_pActionsMenu;
	QSignalMapper* m_pSignalMapper;
signals:
	void sortRoleChanged(int);
	void sortOrderChanged(Qt::SortOrder);
};



#endif
