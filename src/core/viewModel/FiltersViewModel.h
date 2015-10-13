#ifndef _FILETERS_VIEW_MODEL_INCLUDED
#define _FILETERS_VIEW_MODEL_INCLUDED

#include <QAbstractItemModel>
#include <TorrentGroup.h>
class StyleEngene;
class FilterTreeItem;
class FiltersViewModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum FilterRoles
	{
		FilterTypeRole = Qt::UserRole + 1,
		FilterRole
	};
	explicit FiltersViewModel(QObject* parent = NULL);
	~FiltersViewModel();
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	void Retranslate();

	void UpdateIcons();
public slots:
	void UpdateGroupCounters();
	void UpdateGroupItems();
protected:

	void timerEvent(QTimerEvent*) override;
private:
	FilterTreeItem* m_pGroupsItem;
	int m_updateTimerID;
	QMap<QUuid, FilterTreeItem*> m_filtersToUid;
	QList<FilterTreeItem*> m_rootItems;
	void AddGroups(StyleEngene* pStyleEngine, FilterTreeItem* groupsItem, QList<TorrentGroup*> groups);
	void BuildTree();
	void UpdateCounters();
};
#endif