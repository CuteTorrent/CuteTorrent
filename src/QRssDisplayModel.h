#pragma once
#include <QtCore>
#include <QtGui>
#include "RssCommon.h"
class RssFeed;
class RssFeedTreeItem;
class QRssDisplayModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum Role{ RssFeedRole = Qt::UserRole, RssFeedItemRole = Qt::UserRole + 1 };
	QRssDisplayModel(QTreeView* pItemsView, QObject* parrent = NULL);
	~QRssDisplayModel();
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	QList<RssFeed*> SelectedFeeds();
	RssFeed* SelectedIFeed();
	RssItem SelectedFeedItem();
	void retranslate();
public slots:
	void UpdateModel();
	void UpdateVisibleData();
	void contextualMenu(const QPoint&);
	void setupMenu();
	void onFeedUpdate();



private:
	QMenu* m_pMenu;
	QTreeView* m_pItemsView;
	QTimer* m_pUdpateTimer;
	RssManagerPtr m_pRssManager;
	QList<RssFeedTreeItem*> m_rootItems;
};
