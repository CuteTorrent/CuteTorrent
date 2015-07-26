#pragma once
#include <QtGui>

#include "SearchEngine.h"
#define SEARCH_RESULTS_COLUNS_COUNT 1
class QSearchDisplayModel: public QAbstractListModel
{
	Q_OBJECT
public:
	QSearchDisplayModel(QTreeView* pTorrentListView, QObject* parent = 0);
	~QSearchDisplayModel();
	enum Role { SearchItemRole = Qt::UserRole };
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
private:
	FileDownloaderPtr m_pTorrentDownloader;
	SearchEnginePtr m_pSearchEngine;
	QMenu* m_pContextMenu;
	QTreeView* m_pTorrentListView;
private slots:
	void downloadTorrent();
	void OnNewSearchResults();
	void OnTorrentDownloaded(QUrl, QTemporaryFile*);
public slots:
};

