#include <stddef.h>
#include "QSearchDisplayModel.h"
#include "StaticHelpers.h"
#include <filedownloader.h>
#include <gui/Dialogs/OpenTorrentDialog.h>

QSearchDisplayModel::QSearchDisplayModel(QTreeView* pTorrentListView, QObject* parent) : QAbstractListModel(parent), m_pSearchEngine(SearchEngine::getInstance()), m_pTorrentDownloader(FileDownloader::getInstance())
{
	m_pTorrentListView = pTorrentListView;
	connect(m_pSearchEngine.get(), SIGNAL(GotResults()), this, SLOT(OnNewSearchResults()));
	SearchItemsStorrage* pItems = m_pSearchEngine->GetResults();
	connect(pItems, SIGNAL(reset()), this, SLOT(OnNewSearchResults()));
	connect(m_pTorrentDownloader.get(), SIGNAL(DownloadReady(QUrl, QTemporaryFile*)), SLOT(OnTorrentDownloaded(QUrl, QTemporaryFile*)));
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
		SearchItemsStorrage* results = m_pSearchEngine->GetResults();

		if(row > results->length())
		{
			return QVariant();
		}

		SearchResult* res = (*results) [row];

		switch(role)
		{
			case Qt::DisplayRole:
			{
				switch(col)
				{
					case 0:
					{
						return qVariantFromValue(res->Name());
					}

					case 1:
					{
						return qVariantFromValue(res->Size());
					}

					case 2:
					{
						return qVariantFromValue(res->Leechers());
					}

					case 3:
					{
						return qVariantFromValue(res->Seeders());
					}

					case 4:
					{
						return qVariantFromValue(res->Engine());
					}
				}
			}

			case SearchItemRole:
			{
				return qVariantFromValue(res);
			}
		}
	}

	return QVariant();
}

void QSearchDisplayModel::downloadTorrent()
{
	if (m_pTorrentListView->model() != this)
	{
		return;
	}

	QModelIndex index = m_pTorrentListView->currentIndex();
	SearchResult* searchResult = index.data(SearchItemRole).value<SearchResult*>();
	m_pTorrentDownloader->download(searchResult->TorrentFileUrl());
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



