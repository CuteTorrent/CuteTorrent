#pragma once
#include <QtCore>
#include <QtGui>
#include "RssCommon.h"
#include "ServiceCommon.h"
class RssItem;
class RssFeed;
class RssFeedTreeItem;

class QRssDisplayModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum Role
	{
		RssFeedRole = Qt::UserRole,
		RssItemRole = Qt::UserRole + 1
	};

	QRssDisplayModel(QTreeView* pItemsView, QObject* parrent = NULL, bool autoUpdate = true);
	~QRssDisplayModel();
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	QList<RssFeed*> SelectedFeeds();
	QList<RssItem*> SelectedRssItems();
	RssFeed* SelectedFeed();
	RssItem* SelectedRssItem();
	void retranslate();
private slots:
	void UpdateVisibleData();
	void onItemDownload();
	void onItemOpenDesc();
	void onTorrentDownloaded(QUrl url, QTemporaryFilePtr pfile);
	void onMarkAllRead();
	void onMarkAllUnread();
	void onMarkRead();
	void onMarkUnread();
	void onTorrentDownloadError(QUrl, QString);
public slots:
	void UpdateModel();
	void contextualMenu(const QPoint&);
	void onFeedUpdate();
	void onFeedRename();
	void onFeedRemove();
	signals:
	void Notify(int, QString, QVariant);
private:
	QAction *updateAction, *renameAction, *removeAction, *markAllAsReeded, *markAllAsUnread;
	QAction *downloadTorrent, *openLink, *markAsReeded, *markAsUnread;
	FileDownloaderPtr m_pTorrentDownloader;
	QMenu *m_pFeedMenu, *m_pItemMenu;
	QTreeView* m_pItemsView;
	QTimer* m_pUdpateTimer;
	RssManagerPtr m_pRssManager;
	QList<RssFeedTreeItem*> m_rootItems;
	NotificationSystemPtr m_pNotificationSystem;
	void setupFeedMenu();
	void setupItemMenu();

	void setCurrentItemUnread(bool val);
	void setCurrentFeedUnread(bool val);
};

