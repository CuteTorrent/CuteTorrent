#include <stddef.h>
#include "QSearchDisplayModel.h"
#include "StaticHelpers.h"
#include "filedownloader.h"
#include <gui/Dialogs/OpenTorrentDialog.h>
#include "SearchResult.h"
#include <NotificationSystem.h>

QSearchDisplayModel::QSearchDisplayModel(QTreeView* pTorrentListView, QSearchFilterModel* pSearchFilterModel, QObject* parent) 
	: QAbstractListModel(parent)
	, m_pTorrentDownloader(FileDownloader::getNewInstance())
	, m_pSearchEngine(SearchEngine::getInstance())
{
	m_pItemsView = pTorrentListView;
	m_pSearchFilterModel = pSearchFilterModel;
	setupContextMenu();
	connect(m_pSearchEngine.get(), SIGNAL(GotResults()), this, SLOT(OnNewSearchResults()));
	SearchItemsStorragePtr pItems = m_pSearchEngine->GetResults();
	connect(pItems.get(), SIGNAL(reset()), this, SLOT(OnNewSearchResults()));
	connect(m_pTorrentDownloader.get(), SIGNAL(DownloadReady(QUrl, QTemporaryFile*)), SLOT(OnTorrentDownloaded(QUrl, QTemporaryFile*)));
	connect(m_pTorrentDownloader.get(), SIGNAL(DownloadError(QUrl, QString)), SLOT(OnTorrentDownloadError(QUrl, QString)));
}


QSearchDisplayModel::~QSearchDisplayModel()
{
}

int QSearchDisplayModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return m_pSearchEngine->GetResults()->length();
}

int QSearchDisplayModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return SEARCH_RESULTS_COLUNS_COUNT;
}

QVariant QSearchDisplayModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if(index.isValid())
	{
		int row = index.row();
		int col = index.column();
		SearchItemsStorragePtr results = m_pSearchEngine->GetResults();

		if(row > results->length())
		{
			return QVariant();
		}

		SearchResult* res = (*results) [row];

		switch(role)
		{
			case SearchItemRole:
			{
				return qVariantFromValue(res);
			}

			case SearchItemName:
			{
				return res->Name();
			}

			case SearchItemSize:
			{
				return res->Size();
			}

			case SearchItemSeeders:
			{
				return res->Seeders();
			}

			case SearchItemPeers:
			{
				return res->Leechers();
			}
		}
	}

	return QVariant();
}

void QSearchDisplayModel::retranslate()
{
	downloadTorrentAction->setText(tr("DOWNLOAD_TORRENT"));
	openDescriptionAction->setText(tr("OPEN_TORRENT_DESCRIPTION"));
}

void QSearchDisplayModel::setupContextMenu()
{
	menu = new QMenu(m_pItemsView);
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	downloadTorrentAction = new QAction(pStyleEngine->getIcon("downloading"), tr("DOWNLOAD_TORRENT"), m_pItemsView);
	downloadTorrentAction->setObjectName("ACTION_SEARCHLIST_DOWNLOAD");
	connect(downloadTorrentAction, SIGNAL(triggered()), SLOT(downloadTorrent()));
	menu->addAction(downloadTorrentAction);
	openDescriptionAction = new QAction(pStyleEngine->getIcon("doc"), tr("OPEN_TORRENT_DESCRIPTION"), m_pItemsView);
	openDescriptionAction->setObjectName("ACTION_SEARCHLIST_OPEN_TORRENT_DESCRIPTION");
	connect(openDescriptionAction, SIGNAL(triggered()), SLOT(openDescription()));
	menu->addAction(openDescriptionAction);
}

void QSearchDisplayModel::openDescription()
{
	if (m_pItemsView->model() != m_pSearchFilterModel)
		return;

	QModelIndex index = m_pItemsView->currentIndex();
	if (index.isValid())
	{
		SearchResult* searchResult = index.data(SearchItemRole).value<SearchResult*>();
		QDesktopServices::openUrl(searchResult->TorrentDescUrl());
	}
}

void QSearchDisplayModel::contextualMenu(const QPoint& p)
{
	if (m_pItemsView->model() != m_pSearchFilterModel)
		return;

	QModelIndex index = m_pItemsView->indexAt(p);
	if (index.isValid())
	{
		SearchResult* searchResult = index.data(SearchItemRole).value<SearchResult*>();

		bool hasTorrentLink = !searchResult->TorrentFileUrl().isEmpty();
		downloadTorrentAction->setEnabled(hasTorrentLink);
		if (!hasTorrentLink)
		{
			downloadTorrentAction->setToolTip(tr("NO_LINK_AVALIABLE_IN_FEED"));
		}
		else
		{
			downloadTorrentAction->setToolTip("");
		}

		menu->exec(m_pItemsView->mapToGlobal(p));
	}
}

void QSearchDisplayModel::downloadTorrent()
{
	if (m_pItemsView->model() != m_pSearchFilterModel)
	{
		return;
	}

	QModelIndex index = m_pItemsView->currentIndex();
	if (index.isValid())
	{
		SearchResult* searchResult = index.data(SearchItemRole).value<SearchResult*>();
		QString url = searchResult->TorrentFileUrl();
		qDebug() << "Trying to download" << url;
		if (url.startsWith("magnet"))
		{
			boost::scoped_ptr<OpenTorrentDialog> pDialog(new OpenTorrentDialog());
			pDialog->SetData(url);
			pDialog->exec();
		}
		m_pTorrentDownloader->download(url);
	}
	
}

void QSearchDisplayModel::OnNewSearchResults()
{
	reset();
}

void QSearchDisplayModel::OnTorrentDownloaded(QUrl url, QTemporaryFile* pFile)
{
	boost::scoped_ptr<QTemporaryFile> pSafeFile(pFile);
	boost::scoped_ptr<OpenTorrentDialog> pDialog(new OpenTorrentDialog());
	pDialog->SetData(pSafeFile->fileName());
	pDialog->exec();
}

void QSearchDisplayModel::OnTorrentDownloadError(QUrl, QString error)
{
	NotificationSystemPtr notificationSystem = NotificationSystem::getInstance();
	notificationSystem->OnNewNotification(NotificationSystem::SYSTEM_ERROR, error, QVariant());
}


