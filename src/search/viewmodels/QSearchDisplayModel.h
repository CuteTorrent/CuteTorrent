#pragma once
#include <QtGui>

#include "SearchEngine.h"
#include "ServiceCommon.h"
#define SEARCH_RESULTS_COLUNS_COUNT 1
#include "QSearchFilterModel.h"

class QSearchDisplayModel: public QAbstractListModel
{
	Q_OBJECT
public:
	QSearchDisplayModel(QTreeView* pTorrentListView, QSearchFilterModel* pSearchFilterModel, QObject* parent = 0);
	~QSearchDisplayModel();
	enum Role
	{
		SearchItemRole = Qt::UserRole,
		SearchItemName,
		SearchItemSize,
		SearchItemSeeders,
		SearchItemPeers
	};
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
private:
	FileDownloaderPtr m_pTorrentDownloader;
	SearchEnginePtr m_pSearchEngine;
	QMenu* m_pContextMenu;
	QTreeView* m_pTorrentListView;
	QSearchFilterModel* m_pSearchFilterModel;
private slots:
	void downloadTorrent();
	void OnNewSearchResults();
	void OnTorrentDownloaded(QUrl, QTemporaryFile*);
	void OnTorrentDownloadError(QUrl, QString);
public slots:
};

