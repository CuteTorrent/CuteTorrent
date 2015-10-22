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
	enum Mode
	{
		Torrents = 0x001,
		Groups = 0x002,
		Rss = 0x004,
		Search = 0x008,
		All = Torrents| Groups | Rss | Search
	};
	enum FilterRoles
	{
		FilterTypeRole = Qt::UserRole + 1,
		FilterRole
	};
	explicit FiltersViewModel(Mode mode,QObject* parent = NULL);
	~FiltersViewModel();
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex index(QUuid groupUid);
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
	Mode m_mode;
	FilterTreeItem* m_pGroupsItem;
	int m_updateTimerID;
	QMap<QUuid, FilterTreeItem*> m_groupFiltersToUid;
	QMap<QUuid, QModelIndex> m_groupIndexToUid;
	QList<FilterTreeItem*> m_rootItems;
	void AddGroups(StyleEngene* pStyleEngine, FilterTreeItem* groupsItem, QList<TorrentGroup*> groups, QModelIndex groupIndex);
	void BuildTree();
	void UpdateCounters();
};
#endif