#ifndef _FILETERS_VIEW_MODEL_INCLUDED
#define _FILETERS_VIEW_MODEL_INCLUDED

#include <QAbstractItemModel>
#include <TorrentGroup.h>
#include <QMovie>
class StyleEngene;
class FilterTreeItem;
class FiltersViewModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum GroupMode
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

	explicit FiltersViewModel(GroupMode mode,QObject* parent = NULL);
	~FiltersViewModel();
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex index(QUuid groupUid);
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	void Retranslate();
	int getDownloadingTorrentsCount();
	void UpdateIcons();
public slots:
	void UpdateGroupCounters();
	void UpdateGroupItems();
private slots:
	void onBusyChanged(bool prevVal, bool curentVal, QString searchProviderName);
	void onUpdateBusyIcons();
protected:

	void timerEvent(QTimerEvent*) override;
private:
	QMovie* m_LoadingMovie;
	QMap<QString, QModelIndex> m_searchProvidersIndexMap;
	GroupMode m_mode;
	FilterTreeItem* m_pGroupsItem;
	int m_updateTimerID;
	QMap<QUuid, FilterTreeItem*> m_groupFiltersToUid;
	QMap<QUuid, QModelIndex> m_groupIndexToUid;
	QList<FilterTreeItem*> m_rootItems;
	int m_downloadingCount;
	void AddGroups(StyleEngene* pStyleEngine, FilterTreeItem* groupsItem, QList<TorrentGroup*> groups, QModelIndex groupIndex);
	void BuildTree();
	void UpdateCounters();
};
#endif